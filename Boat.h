class Boat
{
public:
    int aim_pos;
    int status;
    int current_goods;
    Boat() : current_goods(0) {}
    void getShip(int id);
    void go(int id);
};