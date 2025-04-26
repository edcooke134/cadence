#ifndef INT2_H
#define INT2_H

class int2 {
public:
    int2() = default;
    int2(int x, int y);


    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    int2 operator+(int2& other) { return int2(x() + other.x(), y() + other.y()); }
    int2 operator-(int2& other) { return int2(x() - other.x(), y() - other.y()); }

private:
    int m_x = 0;
    int m_y = 0;
};

#endif //INT2_H