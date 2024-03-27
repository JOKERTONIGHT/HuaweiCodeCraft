class Good
{
public:
    int x, y, value;
    int time;
    int aim_robot;
    double weight;
    Good() : time(1001), aim_robot(-1), weight(-1) {}
    void operator=(Good &g)
    {
        x = g.x;
        y = g.y;
        value = g.value;
        time = g.time;
        aim_robot = g.aim_robot;
        weight = g.weight;
    }
};