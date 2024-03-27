class Berth
{
public:
    int x;
    int id; // ²´Î»±àºÅ
    int y;
    int transport_time;
    int loading_speed;
    int aim_boat;
    int current_goods;
    int cover_dot;
    Berth() : aim_boat(-1), current_goods(0),cover_dot(0) {}
    void operator=(Berth &b)
    {
        x = b.x;
        y = b.y;
        transport_time = b.transport_time;
        loading_speed = b.loading_speed;
        aim_boat = b.aim_boat;
        current_goods = b.current_goods;
        id = b.id;
    }
};