

struct Point {
    int x;
    int y;
};
using f_void = void (*)();

class Rectangle {
   public:
    Point origin;
    Point topRight;
    int w;
    int h;
    Rectangle() = default;
    Rectangle(int x, int y, int w, int h) : Rectangle({x, y}, w, h) {}
    Rectangle(Point origin, int w, int h) {
        this->origin = origin;
        this->w = w;
        this->h = h;
        this->topRight = {origin.x + w, origin.y + h};
    }
    Point getMiddle() {
        return {origin.x + (w / 2), origin.y + (h / 2)};
    }
    bool checkInside(Point p) {
        Serial.printf("point: %d %d\n", p.x, p.y);
        Serial.printf("origin: %d %d\n", origin.x, origin.y);
        Serial.printf("top right: %d %d\n", topRight.x, topRight.y);
        bool arr[] = {p.x >= origin.x, p.x <= topRight.x, p.y >= origin.y, p.y <= topRight.y};
        for (size_t i = 0; i < 4; i++) {
            Serial.printf("%d ", arr[i]);
        }
        Serial.println("");
        delay(1000);
        return (p.x >= origin.x && p.x <= topRight.x && p.y >= origin.y && p.y <= topRight.y);
    }
};
