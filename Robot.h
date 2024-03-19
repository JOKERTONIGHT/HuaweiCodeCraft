#include "Good.h"
#include <queue>

class Robot
{
public:
    int x, y, good_status;
    int status;
    Good *aim_good;
    queue<int> path;
    Robot() : aim_good(nullptr) {}
    int A_Search();
    bool Is_Robot_in_Berth();
    int Move_to_Berth();
    bool Is_Robot_on_Good();
    int heuristic(int mx, int my);
    int In_Which_Berth();
};