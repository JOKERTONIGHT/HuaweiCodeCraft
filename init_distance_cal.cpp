#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <thread>

using namespace std;

const int MAX_DIST = 1e9; // Representing infinity
const int dx[4] = {0, 1, 0, -1}; // Movement deltas
const int dy[4] = {1, 0, -1, 0}; // Up, Right, Down, Left
const int MAP_SIZE = 200; // Assuming a 200x200 map

// Barrier representation
const int BARRIER = -1;

// Modified isValid function to consider barriers
bool isValid(int x, int y, const vector<vector<int>>& mapGrid) {
    return x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && mapGrid[x][y] != BARRIER;
}

// Function to calculate distances from berths, modified to accept map grid
void calculateDistances(const vector<pair<int, int>>& berths, vector<vector<int>>& distMap, const vector<vector<int>>& mapGrid) {
    queue<pair<int, int>> q;

    // Initialize distance map
    for (int i = 0; i < MAP_SIZE; ++i) {
        fill(distMap[i].begin(), distMap[i].end(), MAX_DIST);
    }

    // Set initial distances for berths and add them to the queue
    for (const auto& berth : berths) {
        if (mapGrid[berth.first][berth.second] != BARRIER) { // Ensure berth is not a barrier
            distMap[berth.first][berth.second] = 0;
            q.push(berth);
        }
    }

    // Perform BFS from each berth
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (isValid(nx, ny, mapGrid) && distMap[nx][ny] == MAX_DIST) {
                distMap[nx][ny] = distMap[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
}

// Wrapper function for multithreading
void calculateDistancesThread(const vector<pair<int, int>>& berths, vector<vector<int>>& distMap, const vector<vector<int>>& mapGrid) {
    calculateDistances(berths, distMap, mapGrid);
}

int main() {
    // Example barriers and berths' locations
    vector<vector<int>> mapGrid(MAP_SIZE, vector<int>(MAP_SIZE, 0)); // 0 for open space, -1 for barriers
    // Example: Adding a barrier
    mapGrid[100][100] = BARRIER;

    vector<pair<int, int>> berths1 = {{10, 10}}; // Berths for the first thread
    vector<pair<int, int>> berths2 = {{190, 190}}; // Berths for the second thread
    vector<vector<int>> distMap1(MAP_SIZE, vector<int>(MAP_SIZE));
    vector<vector<int>> distMap2(MAP_SIZE, vector<int>(MAP_SIZE));

    // Launching two threads
    thread t1(calculateDistancesThread, berths1, ref(distMap1), ref(mapGrid));
    thread t2(calculateDistancesThread, berths2, ref(distMap2), ref(mapGrid));

    // Wait for the threads to finish
    t1.join();
    t2.join();

    // Combine results (for demonstration, might need more sophisticated merging based on use case)
    // Here simply outputting distance maps from each thread
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            cout << min(distMap1[i][j], distMap2[i][j]) << " ";
        }
        cout << "\n";
    }

    return 0;
}
