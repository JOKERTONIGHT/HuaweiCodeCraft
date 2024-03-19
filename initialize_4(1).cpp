#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>

using namespace std;

const int rows = 200;
const int cols = 200;
const int MAX = 40000;

class Map
{
public:
    int steps[rows][cols];
    int id[rows][cols];
    bool visited[rows][cols];

    Map(const int dock_id[10][5])
    {
        for (int k = 0; k < 10; k++)
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    id[dock_id[k][1] + i - 1][dock_id[k][2] + j - 1] = k;
    }

    void BFS(const char data[rows][cols])
    {
        queue<pair<int, int>> bfsQueue;

        // ≥ı ºªØ
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (data[i][j] == 'B')
                {
                    bfsQueue.push({i, j});
                    steps[i][j] = 0;
                    visited[i][j] = true;
                }
                else if (data[i][j] == '*' || data[i][j] == '#')
                {
                    steps[i][j] = -1;
                    visited[i][j] = true;
                }
                else
                {
                    steps[i][j] = MAX;
                    visited[i][j] = false;
                }
            }
        }

        // ÷¥––BFS
        while (!bfsQueue.empty())
        {
            int row = bfsQueue.front().first;
            int col = bfsQueue.front().second;
            visited[row][col] = true;
            bfsQueue.pop();

            int dx[] = {-1, 0, 1, 0};
            int dy[] = {0, 1, 0, -1};

            for (int k = 0; k < 4; ++k)
            {
                int newRow = row + dx[k];
                int newCol = col + dy[k];

                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                    visited[newRow][newCol] == false)
                {
                    steps[newRow][newCol] = steps[row][col] + 1;
                    id[newRow][newCol] = id[row][col];
                    visited[newRow][newCol] = true;
                    bfsQueue.push({newRow, newCol});
                }
            }
        }
    }
};

int main()
{
    ifstream dataFile("D:/huaweifight/WindowsRelease/maps/map1.txt");
    char data[rows][cols] = {};
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            dataFile >> data[i][j];
    dataFile.close();

    ifstream dockFile("D:/huaweifight/WindowsRelease/maps/dock1.txt");
    int dock_id[10][5] = {};
    for (int i = 0; i < 2; ++i) //
        for (int j = 0; j < 5; ++j)
            dockFile >> dock_id[i][j];
    dockFile.close();

    Map map(dock_id);
    map.BFS(data);

    ofstream outFile("D:/huaweifight/WindowsRelease/maps/cout.txt");
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            cout << map.steps[i][j] << ' ';
        outFile << endl;
    }
    outFile.close();

    return 0;
}