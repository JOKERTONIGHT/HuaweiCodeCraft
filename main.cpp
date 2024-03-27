#include <bits/stdc++.h>
using namespace std;
#include "Berth.h"
#include "Robot.h"
#include <set>
#include <list>
#include <climits>
#include "Boat.h"
#include <thread>

struct Dot
{
    int x, y;
    bool operator<(const Dot &a) const
    {
        return x < a.x || (x == a.x && y < a.y);
    }
};

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int INF = INT_MAX;

Robot robot[robot_num];
Berth berth[berth_num];
Boat boat[5];

list<Good> good_init;
list<Good> good_chosen;
list<Good>::iterator good_point;

set<Dot> dot_set;

int money, boat_capacity, time_index;
char maps[n][n];
int step[n][n];
int steps[n][n][10];
int cover[n];

void BFS()
{
    queue<pair<int, int>> bfsQueue;
    bool visited[n][n];
    for (int i = 0; i < 200; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            if (maps[i][j] == 'B')
            {
                bfsQueue.push({i, j});
                step[i][j] = 0;
                visited[i][j] = true;
            }
            else if (maps[i][j] == '*' || maps[i][j] == '#')
            {
                step[i][j] = -1;
                visited[i][j] = true;
            }
            else
            {
                step[i][j] = -1;
                visited[i][j] = false;
            }
        }
    }

    while (!bfsQueue.empty())
    {
        int row = bfsQueue.front().first;
        int col = bfsQueue.front().second;
        bfsQueue.pop();

        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};

        for (int k = 0; k < 4; ++k)
        {
            int newRow = row + dx[k];
            int newCol = col + dy[k];

            if (newRow >= 0 && newRow < 200 && newCol >= 0 && newCol < 200 &&
                visited[newRow][newCol] == false)
            {
                step[newRow][newCol] = step[row][col] + 1;
                bfsQueue.push({newRow, newCol});
                visited[newRow][newCol] = true;
            }
        }
    }
}
void BFS1(int id)
{
    queue<pair<int, int>> bfsQueue;
    bool visited[n][n];
    for (int i = 0; i < 200; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            if (maps[i][j] == '.' || maps[i][j] == 'A')
            {
                steps[i][j][id] = -1;
                visited[i][j] = false;
            }
            else
            {
                steps[i][j][id] = -1;
                visited[i][j] = true;
            }
        }
    }
    for (int i = berth[id].x; i < berth[id].x + 4; i++)
    {
        for (int j = berth[id].y; j < berth[id].y + 4; j++)
        {
            bfsQueue.push({i, j});
            steps[i][j][id] = 0;
            visited[i][j] = true;
        }
    }

    while (!bfsQueue.empty())
    {
        int row = bfsQueue.front().first;
        int col = bfsQueue.front().second;
        bfsQueue.pop();

        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};

        for (int k = 0; k < 4; ++k)
        {
            int newRow = row + dx[k];
            int newCol = col + dy[k];

            if (newRow >= 0 && newRow < 200 && newCol >= 0 && newCol < 200 &&
                visited[newRow][newCol] == false)
            {
                steps[newRow][newCol][id] = steps[row][col][id] + 1;
                bfsQueue.push({newRow, newCol});
                visited[newRow][newCol] = true;
            }
        }
    }
}
void performBFS1()
{
    // vector<thread> threads;
    // for (int i = 0; i < 5; i++)
    // {
    //     threads.emplace_back(BFS1, i);
    // }
    // for (auto &t : threads)
    // {
    //     t.join();
    // }
    for (int i = 0; i < 5; i++)
    {
        BFS1(i);
    }
}

void Init()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> maps[i][j];
        }
    }
    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &berth[i].id);
        scanf("%d%d%d%d", &berth[i].x, &berth[i].y, &berth[i].transport_time, &berth[i].loading_speed);
    }
    for (int i = 0; i < 10; i++)
    {
        int index = i;
        for (int j = i + 1; j < 10; j++)
        {
            if (berth[j].transport_time < berth[index].transport_time)
            {
                index = j;
            }
        }
        Berth b;
        b = berth[index];
        berth[index] = berth[i];
        berth[i] = b;
    }
    for (int i = 5; i < berth_num; i++)
    {
        for (int j = berth[i].x; j < berth[i].x + 4; j++)
        {
            for (int q = berth[i].y; q < berth[i].y + 4; q++)
            {
                maps[j][q] = '#';
            }
        }
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    BFS();
    performBFS1();
    fflush(stdout);
}

int Input()
{
    if (cin.eof())
    {
        exit(-1);
    }
    scanf("%d%d", &time_index, &money);
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++)
    {
        Good g;
        scanf("%d%d%d", &g.x, &g.y, &g.value);
        good_init.push_back(g);
    }
    for (int i = 0; i < robot_num; i++)
    {
        scanf("%d%d%d%d", &robot[i].good_status, &robot[i].x, &robot[i].y, &robot[i].status);
    }
    for (int i = 0; i < 5; i++)
        scanf("%d%d\n", &boat[i].status, &boat[i].aim_pos);
    char okk[100];
    scanf("%s", okk);
    return time_index;
}

bool Robot::Is_Robot_in_Berth()
{
    if (step[x][y] == 0)
        return true;
    return false;
}

bool Robot::Is_Robot_on_Good()
{
    if (aim_good != nullptr && x == aim_good->x && y == aim_good->y)
        return true;
    return false;
}

void choose_good(int d)
{
    int dist;
    double min_weight = 10000, weight = 0;
    good_point = good_init.begin();
    int index = 0;
    for (int i = 0; good_point != good_init.end(); good_point++, i++)
    {
        if (robot[d].Is_Robot_in_Berth())
        {
            dist = steps[(*good_point).x][(*good_point).y][robot[d].In_Which_Berth()];
            if (dist == -1)
                continue;
        }
        else
        {
            dist = (abs((*good_point).x - robot[d].x) + abs((*good_point).y - robot[d].y)) * 1.6;
        }
        // dist += step[(*good_point).x][(*good_point).y];
        if (time_index<3000){
            weight = dist - (*good_point).value * 1.2;
        }
        else if(time_index<9000){
            weight = dist - (*good_point).value * 2.4;
        }
        else if(time_index<12000){
            weight = dist - (*good_point).value * 1.8;
        }
        else {
            weight = dist - (*good_point).value * 1.5;
        }

        if (weight < min_weight && ((*good_point).time - dist > 12))
        {
            min_weight = weight;
            index = i;
        }
    }
    Good g;
    good_point = good_init.begin();
    for (int i = 0; i < index; i++)
    {
        good_point++;
    }
    g = *good_point;
    g.aim_robot = d;
    good_chosen.push_back(g);
    good_init.erase(good_point);
    robot[d].aim_good = &good_chosen.back();

    // if (robot[d].Is_Robot_in_Berth())
    // {
    //     int berth_id = robot[d].In_Which_Berth();
    //     int dx[4] = {0, 0, 1, -1};
    //     int dy[4] = {-1, 1, 0, 0};
    //     int gx = robot[d].aim_good->x;
    //     int gy = robot[d].aim_good->y;
    //     while (steps[gx][gy][berth_id] != 0)
    //     {
    //         for (int i = 0; i < 4; i++)
    //         {
    //             int ux = gx + dx[i];
    //             int vy = gy + dy[i];
    //             if (ux >= 0 && ux < n && vy >= 0 && vy < n && steps[ux][vy][berth_id] == steps[gx][gy][berth_id] - 1)
    //             {
    //                 robot[d].path.push(i);
    //                 gx = ux;
    //                 gy = vy;
    //             }
    //         }
    //     }
    //     //from (gx, gy) to robot
    //     while (gx != robot[d].x || gy != robot[d].y)
    //     {
    //         if(gx > robot[d].x)
    //         {
    //             robot[d].path.push(3);
    //             gx--;
    //         }
    //         else if(gx < robot[d].x)
    //         {
    //             robot[d].path.push(2);
    //             gx++;
    //         }
    //         else if(gy > robot[d].y)
    //         {
    //             robot[d].path.push(0);
    //             gy--;
    //         }
    //         else
    //         {
    //             robot[d].path.push(1);
    //             gy++;
    //         }
    //     }
    // }
}

int choose_berth()
{
    double max_weight = 0, weight;
    int ret_id = -1;
    for (int i = 0; i < 5; i++)
    {
        if (berth[i].aim_boat == -1)
        {
            weight = berth[i].current_goods * 0.45 + berth[i].loading_speed * 0.8 + 80000.0 / berth[i].transport_time;
            if (weight > max_weight)
            {
                max_weight = weight;
                ret_id = i;
            }
        }
    }
    return ret_id;
}

int Robot::heuristic(int mx, int my)
{
    return abs(mx - aim_good->x) + abs(my - aim_good->y);
}

int Robot::A_Search()
{
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    int cost[n][n];
    int pre[n][n];
    bool visited[n][n];
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cost[i][j] = INF;
            pre[i][j] = -1;
            visited[i][j] = false;
        }
    }

    pq.push(make_pair(heuristic(x, y), make_pair(x, y)));
    cost[x][y] = 0;
    pre[x][y] = -1;

    while (!pq.empty())
    {
        pair<int, pair<int, int>> top = pq.top();
        pq.pop();
        int u = top.second.first;
        int v = top.second.second;
        int d = top.first - heuristic(u, v);

        if (u == aim_good->x && v == aim_good->y)
        {
            int p = pre[u][v];
            while (p != -1)
            {
                int px = p / n;
                int py = p % n;
                if (px == u)
                {
                    if (py < v)
                        path.push(0);
                    else
                        path.push(1);
                }
                else
                {
                    if (px < u)
                        path.push(3);
                    else
                        path.push(2);
                }
                u = px;
                v = py;
                p = pre[u][v];
            }
            return d;
        }
        if (visited[u][v])
            continue;
        visited[u][v] = true;
        for (int i = 0; i < 4; i++)
        {
            int ux = u + dx[i];
            int vy = v + dy[i];
            if (ux >= 0 && ux < n && vy >= 0 && vy < n && !visited[ux][vy] && maps[ux][vy] == '.')
            {
                int newCost = d + 1;
                if (cost[ux][vy] > newCost)
                {
                    cost[ux][vy] = newCost;
                    pre[ux][vy] = u * n + v;
                    pq.push(make_pair(newCost + heuristic(ux, vy), make_pair(ux, vy)));
                }
            }
        }
    }
    return -1;
}

int Robot::Move_to_Berth()
{
    int now_step = step[x][y];
    if (y != 0 && step[x][y - 1] < now_step && step[x][y - 1] != -1 && dot_set.find({x, y - 1}) == dot_set.end())
    {
        dot_set.erase({x, y});
        dot_set.insert({x, y - 1});
        y--;
        return 1;
    }
    if (x != 0 && step[x - 1][y] < now_step && step[x - 1][y] != -1 && dot_set.find({x - 1, y}) == dot_set.end())
    {
        dot_set.erase({x, y});
        dot_set.insert({x - 1, y});
        x--;
        return 2;
    }
    if (y != 199 && step[x][y + 1] < now_step && step[x][y + 1] != -1 && dot_set.find({x, y + 1}) == dot_set.end())
    {
        dot_set.erase({x, y});
        dot_set.insert({x, y + 1});
        y++;
        return 0;
    }
    if (x != 199 && step[x + 1][y] < now_step && step[x + 1][y] != -1 && dot_set.find({x + 1, y}) == dot_set.end())
    {
        dot_set.erase({x, y});
        dot_set.insert({x + 1, y});
        x++;
        return 3;
    }
    return -1;
}

int Robot::In_Which_Berth()
{
    for (int i = 0; i < 5; i++)
    {
        if (x >= berth[i].x && x <= berth[i].x + 3 && y >= berth[i].y && y <= berth[i].y + 3)
            return i;
    }
    return -1;
}

Dot move_dot(int move_pos, Dot dot)
{
    int dx = 0, dy = 0;
    if (move_pos == 0)
    {
        dy++;
    }
    else if (move_pos == 1)
    {
        dy--;
    }
    else if (move_pos == 2)
    {
        dx--;
    }
    else
    {
        dx++;
    }
    return {dot.x + dx, dot.y + dy};
}

int ret_dot(int move_pos)
{
    if (move_pos == 0)
    {
        return 1;
    }
    else if (move_pos == 1)
    {
        return 0;
    }
    else if (move_pos == 2)
    {
        return 3;
    }
    else
    {
        return 2;
    }
}

int main()
{
    Init();
    // std::ofstream outfile("out1.txt", std::ios::out);
    for (int frame = 1; frame <= 15000; frame++)
    {
        Input();
        // outfile << time_index << std::endl;
        dot_set.clear();
        good_point = good_init.begin();
        while (good_point != good_init.end())
        {
            (*good_point).time -= 1;
            if ((*good_point).time <= 0)
            {
                good_point = good_init.erase(good_point);
                continue;
            }
            // outfile << (*good_point).x << " " << (*good_point).y << " " << (*good_point).value << std::endl;
            good_point++;
        }
        good_point = good_chosen.begin();
        while (good_point != good_chosen.end())
        {
            (*good_point).time -= 1;
            if ((*good_point).time <= 0)
            {
                robot[(*good_point).aim_robot].aim_good = nullptr;
                robot[(*good_point).aim_robot].status = 0;
                good_point = good_chosen.erase(good_point);
                continue;
            }
            // outfile << (*good_point).x << " " << (*good_point).y << " " << (*good_point).value << std::endl;
            good_point++;
        }
        for (int i = 0; i < robot_num; i++)
        {
            dot_set.insert({robot[i].x, robot[i].y});
        }
        for (int i = 0; i < robot_num; i++)
        {
            // outfile << i << " " << robot[i].good_status << " " << robot[i].x << " " << robot[i].y << endl;
            if (robot[i].status == 1) // 正常运行状态
            {
                int move_pos = -1;
                if (robot[i].good_status == 1) // 携带货物
                {
                    if (!robot[i].Is_Robot_in_Berth()) // 未到达泊位
                    {
                        move_pos = robot[i].Move_to_Berth();
                        if (move_pos != -1)
                            printf("move %d %d\n", i, move_pos);
                    }
                    if (robot[i].Is_Robot_in_Berth())
                    {
                        printf("pull %d\n", i);
                        berth[robot[i].In_Which_Berth()].current_goods++;
                        robot[i].aim_good = nullptr;
                        good_point = good_chosen.begin();
                        while (good_point != good_chosen.end())
                        {
                            if ((*good_point).aim_robot == i)
                            {
                                good_chosen.erase(good_point);
                                break;
                            }
                            else
                                good_point++;
                        }
                    }
                }
                if (robot[i].good_status == 0 || (move_pos == -1 && robot[i].Is_Robot_in_Berth()))
                {
                    if (robot[i].aim_good == nullptr && !good_init.empty()) // 未选择货物
                    {
                        choose_good(i);
                    }
                    if (robot[i].aim_good != nullptr)
                    {
                        // outfile << robot[i].aim_good->x << " " << robot[i].aim_good->y << " " << robot[i].aim_good->aim_robot << " " << robot[i].aim_good->time << endl;
                        if (!robot[i].Is_Robot_on_Good()) // 未到达货物
                        {
                            if (robot[i].path.empty())
                            {
                                move_pos = robot[i].A_Search();

                                if (move_pos == -1) // 不可达
                                {
                                    robot[i].aim_good = nullptr;
                                    good_point = good_chosen.begin();
                                    while (good_point != good_chosen.end())
                                    {
                                        if ((*good_point).aim_robot == i)
                                        {
                                            good_chosen.erase(good_point);
                                            break;
                                        }
                                        else
                                            good_point++;
                                    }
                                }
                            }
                            if (!robot[i].path.empty())
                            {
                                move_pos = robot[i].path.top();
                                Dot d = move_dot(move_pos, {robot[i].x, robot[i].y});
                                if (dot_set.find(d) == dot_set.end()) // 可移动
                                {
                                    printf("move %d %d\n", i, move_pos);
                                    dot_set.erase({robot[i].x, robot[i].y});
                                    robot[i].x = d.x;
                                    robot[i].y = d.y;
                                    dot_set.insert({robot[i].x, robot[i].y});
                                    robot[i].path.pop();
                                }
                                else
                                {
                                    if (move_pos == 0 || move_pos == 1)
                                    {
                                        for (int q = 3; q >= 0; q--)
                                        {
                                            if (q != move_pos)
                                            {
                                                Dot d = move_dot(q, {robot[i].x, robot[i].y});
                                                if (dot_set.find(d) == dot_set.end() && d.x >= 0 && d.x <= 199 && d.y >= 0 && d.y <= 199 && step[d.x][d.y] != -1) // 可移动
                                                {
                                                    printf("move %d %d\n", i, q);
                                                    dot_set.erase({robot[i].x, robot[i].y});
                                                    robot[i].x = d.x;
                                                    robot[i].y = d.y;
                                                    dot_set.insert({robot[i].x, robot[i].y});
                                                    robot[i].path.push(ret_dot(q));
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for (int q = 0; q < 4; q++)
                                        {
                                            if (q != move_pos)
                                            {
                                                Dot d = move_dot(q, {robot[i].x, robot[i].y});
                                                if (dot_set.find(d) == dot_set.end() && d.x >= 0 && d.x <= 199 && d.y >= 0 && d.y <= 199 && step[d.x][d.y] != -1) // 可移动
                                                {
                                                    printf("move %d %d\n", i, q);
                                                    dot_set.erase({robot[i].x, robot[i].y});
                                                    robot[i].x = d.x;
                                                    robot[i].y = d.y;
                                                    dot_set.insert({robot[i].x, robot[i].y});
                                                    robot[i].path.push(ret_dot(q));
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (robot[i].Is_Robot_on_Good())
                        {
                            printf("get %d\n", i);
                            robot[i].aim_good->time = 100000;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 5; i++)
        {
            // if (boat[i].status == 2)
            // {
            //     if (boat[i].aim_pos != -1 && berth[boat[i].aim_pos].aim_boat == -1)
            //     {
            //         printf("ship %d %d\n", i, berth[boat[i].aim_pos].id);
            //     }
            // }
            // else
            if (boat[i].status == 1)
            {
                if (boat[i].aim_pos == -1)
                {
                    boat[i].current_goods = 0;
                    boat[i].aim_pos = i;
                    printf("ship %d %d\n", i, berth[boat[i].aim_pos].id);
                    // berth[boat[i].aim_pos].aim_boat = i;
                }
                else
                {
                    boat[i].aim_pos = i;
                    if (berth[boat[i].aim_pos].transport_time + time_index > 14999 ||
                        time_index == 14998 - berth[boat[i].aim_pos].transport_time * 3 - boat_capacity / berth[boat[i].aim_pos].loading_speed - 1 ||
                        (boat[i].current_goods == boat_capacity))
                    {
                        printf("go %d\n", i);
                        // berth[boat[i].aim_pos].aim_boat = -1;
                    }
                    else
                    {
                        for (int k = 0; k < berth[boat[i].aim_pos].loading_speed; k++)
                        {
                            if (berth[boat[i].aim_pos].current_goods > 0 && boat[i].current_goods < boat_capacity)
                            {
                                berth[boat[i].aim_pos].current_goods--;
                                boat[i].current_goods++;
                            }
                        }
                    }
                }
            }
        }
        puts("OK");
        fflush(stdout);
    }
    // outfile.close();
    return 0;
}
