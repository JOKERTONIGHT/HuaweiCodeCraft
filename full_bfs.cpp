void BFS1(int id)
{
    queue<pair<int, int>> bfsQueue;
    bool visited[n][n];
    for(int i = berth[id].x; i < berth[id].x + 4; i++)
    {
        for(int j = berth[id].y; j < berth[id].y + 4; j++)
        {
            bfsQueue.push({i, j});
            steps[i][j][id] = 0;
            visited[i][j] = true;
        }
            
    }
    for (int i = 0; i < 200; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            if (maps[i][j] == '.')
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
    vector<thread> threads;
    for (int i = 0; i < 10; i++)
    {
        threads.emplace_back(BFS1, i);
    }
    for (auto &t : threads)
    {
        t.join();
    }
}

void choose_good(int d, int zhen)
{
    int dist;
    double min_weight = INF, weight = 0;
    good_point = good_init.begin();
    int index = 0;
    for (int i = 0; good_point != good_init.end(); good_point++, i++)
    {
        if (robot[d].Is_Robot_in_Berth())
        {
            dist = steps[(*good_point).x][(*good_point).y][robot[d].In_Which_Berth()] * 0.6;
        }
        else
        {
            dist = abs((*good_point).x - robot[d].x) + abs((*good_point).y - robot[d].y);
        }
        if (zhen < 3000)
        {
            weight = dist;
        }
        else
        {
            weight = dist - (*good_point).value;
        }

        if (weight < min_weight && ((*good_point).time - dist * 2 > 0))
        {
            min_weight = weight;
            index = i;
        }
    }
    Good g;
    good_point = good_init.begin();
    for(int i = 0; i < index; i++)
    {
        good_point++;
    }
    g = *good_point;
    g.aim_robot = d;
    good_chosen.push_back(g);
    good_init.erase(good_point);
    robot[d].aim_good = &good_chosen.back();
}