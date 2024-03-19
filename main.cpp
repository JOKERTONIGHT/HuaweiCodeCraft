#include <bits/stdc++.h>
#include "Berth.h"
#include "Robot.h"
#include "Boat.h"
#include <set>
#include <list>
#include <climits>
using namespace std;

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

void Init()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> maps[i][j];
        }
    }
    for (int i = 0; i < berth_num; i++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    BFS();
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

void choose_good(int i)
{
    double dist, max_weight = 0;
    good_point = good_init.begin();
    while (good_point != good_init.end())
    {
        dist = pow((*good_point).x - robot[i].x, 2) + pow((*good_point).y - robot[i].y, 2);
        dist = sqrt(dist);
        (*good_point).weight = 200.0 / dist * 0.6 + (*good_point).time * 0.01 * 0.1 + (*good_point).value * 0.3;
        if ((*good_point).weight > max_weight)
        {
            max_weight = (*good_point).weight;
        }
        good_point++;
    }
    Good g;
    good_point = good_init.begin();
    while (good_point != good_init.end())
    {
        if ((*good_point).weight - max_weight < 0.000001)
        {
            g = *good_point;
            g.aim_robot = i;
            good_chosen.push_back(g);
            good_init.erase(good_point);
            break;
        }
        good_point++;
    }
    robot[i].aim_good = &good_chosen.back();
}

int choose_berth(int boat_id)
{
    double max_weight = 0, weight;
    int ret_id = -1;
    for (int i = 0; i < berth_num; i++)
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
        dot_set.insert({x, y - 1});
        y--;
        return 1;
    }
    if (x != 0 && step[x - 1][y] < now_step && step[x - 1][y] != -1 && dot_set.find({x - 1, y}) == dot_set.end())
    {
        dot_set.insert({x - 1, y});
        x--;
        return 2;
    }
    if (y != 199 && step[x][y + 1] < now_step && step[x][y + 1] != -1 && dot_set.find({x, y + 1}) == dot_set.end())
    {
        dot_set.insert({x, y + 1});
        y++;
        return 0;
    }
    if (x != 199 && step[x + 1][y] < now_step && step[x + 1][y] != -1 && dot_set.find({x + 1, y}) == dot_set.end())
    {
        dot_set.insert({x + 1, y});
        x++;
        return 3;
    }
    return -1;
}

int Robot::In_Which_Berth()
{
    for (int i = 0; i < berth_num; i++)
    {
        if (x >= berth[i].x && x <= berth[i].x + 3 && y >= berth[i].y && y <= berth[i].y + 3)
            return i;
    }
    return -1;
}

int main()
{
    Init();
    std::ofstream outfile("out1.txt", std::ios::out);
    for (int zhen = 1; zhen <= 15000; zhen++)
    {
        Input();
        outfile << time_index << std::endl;
        dot_set.clear();
        good_point = good_init.begin();
        while (good_point != good_init.end())
        {
            (*good_point).time -= 20;
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
            (*good_point).time -= 20;
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
            outfile << i << " " << robot[i].good_status << " " << robot[i].x << " " << robot[i].y << endl;
            if (robot[i].status == 1) // ��������״̬
            {
                int move_pos = -1;
                if (robot[i].good_status == 1) // Я������
                {
                    if (!robot[i].Is_Robot_in_Berth()) // δ���ﲴλ
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
                    if (robot[i].aim_good == nullptr && !good_init.empty()) // δѡ�����
                    {
                        choose_good(i);
                    }
                    if (robot[i].aim_good != nullptr)
                    {
                        outfile << robot[i].aim_good->x << " " << robot[i].aim_good->y << " " << robot[i].aim_good->aim_robot << endl;
                        if (!robot[i].Is_Robot_on_Good()) // δ�������
                        {
                            if (robot[i].path.empty())
                            {
                                move_pos = robot[i].A_Search();

                                if (move_pos == -1) // ���ɴ�
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
                                if (dot_set.find({robot[i].x + dx, robot[i].y + dy}) == dot_set.end())
                                {
                                    printf("move %d %d\n", i, move_pos);
                                    robot[i].x += dx;
                                    robot[i].y += dy;
                                    dot_set.insert({robot[i].x, robot[i].y});
                                    robot[i].path.pop();
                                }
                            }
                        }
                        if (robot[i].Is_Robot_on_Good())
                        {
                            printf("get %d\n", i);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if (boat[i].status == 2)
            {
                if (boat[i].aim_pos != -1 && berth[boat[i].aim_pos].aim_boat == -1)
                {
                    printf("ship %d %d\n", i, boat[i].aim_pos);
                }
            }
            else if (boat[i].status == 1)
            {
                if (boat[i].aim_pos == -1)
                {
                    boat[i].current_goods = 0;
                    boat[i].aim_pos = choose_berth(i);
                    printf("ship %d %d\n", i, boat[i].aim_pos);
                    berth[boat[i].aim_pos].aim_boat = i;
                }
                else
                {
                    if (boat[i].current_goods == boat_capacity ||
                        berth[boat[i].aim_pos].transport_time + time_index > 14990)
                    {
                        printf("go %d\n", i);
                        berth[boat[i].aim_pos].aim_boat = -1;
                    }
                    else
                    {
                        if (berth[boat[i].aim_pos].current_goods <= berth[boat[i].aim_pos].loading_speed)
                        {
                            boat[i].current_goods += berth[boat[i].aim_pos].current_goods;
                            berth[boat[i].aim_pos].current_goods = 0;
                        }
                        else
                        {
                            berth[boat[i].aim_pos].current_goods -= berth[boat[i].aim_pos].loading_speed;
                            boat[i].current_goods += berth[boat[i].aim_pos].loading_speed;
                        }
                    }
                }
            }
        }
        puts("OK");
        fflush(stdout);
    }
    outfile.close();
    return 0;
}