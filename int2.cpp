#include "int2.h"

    int2::int2(int x, int y) : m_x(x), m_y(y) {}


    int int2::x() const {
        return m_x;
    }

    void int2::setX(int x) {
        m_x = x;
    }

    int int2::y() const {
        return m_y;
    }

    void int2::setY(int y) {
        m_y = y;
    }