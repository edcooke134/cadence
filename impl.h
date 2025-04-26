#ifndef IMPL_H
#define IMPL_H

#include <vector>
#include "int2.h"
#include <string>
#include <cmath>

using Polygon = std::vector<int2>;

class Impl {
public:
    Impl(const std::string& fileName);
    bool readFile();
    bool contains(const int2& point);

private:
    bool detectWithEarClipping(Polygon poly, const int2& point, bool ignoreEdges = false) const;
    bool earContains(const Polygon& ear, const int2& point) const;

    inline int earAreaDoubled(const int2& A, const int2& B, const int2& C) const {
        return std::abs(A.x() * (B.y() - C.y()) + B.x() * (C.y() - A.y()) + C.x() * (A.y() - B.y()));
    }
    inline bool colinearCheck(const int2& A, const int2& B, const int2& C) const {
        return (B.y() - A.y()) * (C.x() - B.x()) == (C.y() - B.y()) * (B.x() - A.x());
    }

    std::string m_fileName;
    Polygon m_outline;
    std::vector<Polygon> m_cuts;
};

#endif //IMPL_H