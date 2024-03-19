#include <stdio.h>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>
using namespace std;

const int MAX_SIZE = 200;
const int INF = INT_MAX;
char map[MAX_SIZE][MAX_SIZE] = {};

class Robot{
public:
    int x, y;
    int status;
    int cargos;
    int gx, gy;//target
    int aim_berth;

    //a stack to store the path, 0 means right, 1 means left, 2 means up, 3 means down
    stack<int> path;

    Robot():gx(-1), gy(-1), aim_berth(-1){}

    
    int heuristic(int mx, int my){
        return abs(mx - gx) + abs(my - gy);
    }
    int aStar_search();
};

//in the 2D map, "." means empty, "#" means obstacle, for a robot(at x, y), this function will return the shortest path to the target(gx, gy)
//use 0, 1, 2, 3 to represent the direction of the path, 0 means right, 1 means left, 2 means up, 3 means down
//store the path in the stack path, if the target is unreachable, return -1
int Robot::aStar_search(){
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    int cost[MAX_SIZE][MAX_SIZE];
    int pre[MAX_SIZE][MAX_SIZE];
    bool visited[MAX_SIZE][MAX_SIZE];
    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > pq;

    for(int i = 0; i < MAX_SIZE; i++){
        for(int j = 0; j < MAX_SIZE; j++){
            cost[i][j] = INF;
            pre[i][j] = -1;
            visited[i][j] = false;
        }
    }

    pq.push(make_pair(heuristic(x, y), make_pair(x, y)));
    cost[x][y] = 0;
    pre[x][y] = -1;

    while(!pq.empty()){
        pair<int, pair<int, int> > top = pq.top();
        pq.pop();
        int u = top.second.first;
        int v = top.second.second;
        int d = top.first - heuristic(u, v);
        
        if(u == gx && v == gy){
            int p = pre[u][v];
            while(p != -1){
                int px = p / MAX_SIZE;
                int py = p % MAX_SIZE;
                if(px == u){
                    if(py < v) path.push(0);
                    else path.push(1);
                }else{
                    if(px < u) path.push(3);
                    else path.push(2);
                }
                u = px;
                v = py;
                p = pre[u][v];
            }
            cout << "Path found! :)" << endl;
            return d;
        }
        if(visited[u][v]) continue;
        visited[u][v] = true;
        for(int i = 0; i < 4; i++){
            int ux = u + dx[i];
            int vy = v + dy[i];
            if(ux >= 0 && ux < MAX_SIZE && vy >= 0 && vy < MAX_SIZE && !visited[ux][vy] && map[ux][vy] == '.'){
                int newCost = d + 1;
                if(cost[ux][vy] > newCost){
                    cost[ux][vy] = newCost;
                    pre[ux][vy] = u * MAX_SIZE + v;
                    pq.push(make_pair(newCost + heuristic(ux, vy), make_pair(ux, vy)));
                }
            }
        }
    }
    cout << "The target is unreachable! :(" << endl;
    return -1;
}

//to do: implement the function to control the robot
// void controlRobot(Robot& robot){
//     if(robot.path.empty()){
//         robot.aStar_search();
//     }else{
//         int d = robot.path.top();
//         robot.path.pop();
//         printf("%d", d);
//     }
// }

//print the path
void controlRobot(Robot& robot){
    while(!robot.path.empty()){
        int d = robot.path.top();
        robot.path.pop();
        printf("%d", d);
    }
}

int main()
{
    Robot robot;
    robot.x = 36;
    robot.y = 173;
    robot.gx = 137;
    robot.gy = 117;

    //read the map from a .txt file
    ifstream file("map1.txt"); // 假设文件名为"data.txt"
    if (!file.is_open()) {
        cerr << "无法打开文件！" << endl;
        return -1;
    }

    int row = 0, col = 0;
    while (file && row < 200) { // 确保文件可读且没有超过数组的行数
        file.get(map[row][col]); // 读取一个字符到数组中
        if (file.eof()) break; // 如果到达文件末尾，退出循环

        if (map[row][col] == '\n') { // 如果读取到换行符，移动到下一行
            col = 0;
        } else if (col < 199) { // 如果没有到达行的末尾，移动到下一个字符
            col++;
        } else { // 如果到达行的末尾，移动到下一行的开始
            row++;
            col = 0;
        }
    }

    file.close(); // 关闭文件

    //print the map
    // for (int i = 0; i <= row; ++i) {
    //     for (int j = 0; j < 200 && map[i][j] != '\0'; ++j) {
    //         std::cout << map[i][j];
    //     }
    //     std::cout << std::endl;
    // }

    auto start = std::chrono::high_resolution_clock::now();
    robot.aStar_search();
    auto end = std::chrono::high_resolution_clock::now();
    controlRobot(robot);
    std::chrono::duration<double> elapsed = end - start;

    cout << endl << "Elapsed time: " << elapsed.count() << "s\n";

    return 0;
}