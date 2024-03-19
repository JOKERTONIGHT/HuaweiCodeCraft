class Berth
{
public:
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int aim_boat;
    int current_goods;
    Berth() : aim_boat(-1), current_goods(0) {}
};