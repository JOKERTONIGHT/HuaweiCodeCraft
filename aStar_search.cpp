#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;

const int MAX_SIZE = 200;
const int INF = INT_MAX;
char map[MAX_SIZE][MAX_SIZE];

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
        if(map[mx][my] == '#') return INF;
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
        int d = top.first - heuristic(top.second.first, top.second.second);
        int u = top.second.first;
        int v = top.second.second;
        if(u == gx && v == gy){
            int p = pre[u][v];
            while(p != -1){
                int px = p / MAX_SIZE;
                int py = p % MAX_SIZE;
                if(px == u){
                    if(py < v) path.push(0);
                    else path.push(1);
                }else{
                    if(px < u) path.push(2);
                    else path.push(3);
                }
                u = px;
                v = py;
                p = pre[u][v];
            }
            return d;
        }
        if(visited[u][v]) continue;
        visited[u][v] = true;
        for(int i = 0; i < 4; i++){
            int ux = u + dx[i];
            int vy = v + dy[i];
            if(ux >= 0 && ux < MAX_SIZE && vy >= 0 && vy < MAX_SIZE && !visited[ux][vy] && map[ux][vy] != '#'){
                int newCost = d + 1;
                if(cost[ux][vy] > newCost){
                    cost[ux][vy] = newCost;
                    pre[ux][vy] = u * MAX_SIZE + v;
                    pq.push(make_pair(newCost + heuristic(ux, vy), make_pair(ux, vy)));
                }
            }
        }
    }
    return -1;
}