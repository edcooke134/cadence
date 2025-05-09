#include "impl.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>

Impl::Impl(const std::string& fileName) : m_fileName(fileName) {}

// Method for reading in a file in the given format
// The format is assumed to be correct, exit if it is not
bool Impl::readFile() {
    m_outline.clear();
    m_cuts.clear();

    const auto exit = [this]() {
        m_outline.clear();
        m_cuts.clear();
        std::cout << "Text file has the wrong format." << std::endl;
    };

    std::string buf;
    size_t count = 0;

    std::ifstream file(m_fileName);
    if (file.is_open()) {
        std::string first;
        std::string second;

        while (std::getline(file, buf)) {
            if (size_t found = buf.find(' '); found != std::string::npos) {
                // Parse the line into two strings and handle accordingly
                first = buf.substr(0, found);
                second = buf.substr(found + 1);

                if (first == "OUTLINE") {
                    const int count = atoi(second.c_str());
                    for (int i = 0; i < count; ++i) {
                        if (!std::getline(file, buf)) {
                            exit();
                            return false;
                        }
                        if (size_t found = buf.find(' '); found != std::string::npos) {
                            first = buf.substr(0, found);
                            second = buf.substr(found + 1);
                            m_outline.emplace_back(atoi(first.c_str()), atoi(second.c_str()));
                        } else {
                            exit();
                            return false;
                        }
                    }
                } else if (first == "CUT") {
                    const int count = atoi(second.c_str());
                    Polygon cut;
                    for (int i = 0; i < count; ++i) {
                        if (!std::getline(file, buf)) {
                            exit();
                            return false;
                        }
                        if (size_t found = buf.find(' '); found != std::string::npos) {
                            first = buf.substr(0, found);
                            second = buf.substr(found + 1);
                            cut.emplace_back(atoi(first.c_str()), atoi(second.c_str()));
                        }
                        else {
                            exit();
                            return false;
                        }
                    }
                    m_cuts.push_back(cut);
                } else {
                    exit();
                    return false;
                }

            } else {
                exit();
                return false;
            }
        }
        file.close();
    } else { 
        std::cout << "Cannot open file." << std::endl;
        return false;
    }

    return true;
}

// Method to check whether a point is inside the polygon from the file
bool Impl::contains(const int2& point) {
    // Check whether the point is inside the cuts, ignore edges
    for (const auto& cut : m_cuts) {
        if (detectWithEarClipping(cut, point, true))
            return false;
    }
    // Check whether the point is inside the outline
    return detectWithEarClipping(m_outline, point);
}

// Use ear clipping to split the (simple) polygon, checking if the point is contained
// in any of the ears. Return true if it contained within the polygon
bool Impl::detectWithEarClipping(Polygon poly, const int2& point, bool ignoreEdges) const {
    if (poly.size() < 3)
        return false;

    // If we do not want to include the edges, check that the point is not on the edge
    if (ignoreEdges) {
        for (int i = 0; i + 1 < poly.size(); ++i)
            if (isColinear(poly[i], point, poly[i + 1]))
                return false;

        // Check the closing edge of the polygon
        if (isColinear(poly.front(), point, poly.back()))
            return false;
    }

    int infCheck = 0;
    const bool clockwise = isClockwise(poly);

    // Ear clipping
    while (poly.size() > 2 && infCheck < 10000) {
        // Iterate through points until we find an ear
        for (size_t i = 0; i < poly.size(); ++i) {
            // Find the indices before and after i, wrapping if necessary
            const size_t firstIdx = (i == 0 ? poly.size() - 1 : i - 1);
            const size_t lastIdx = (i == poly.size() - 1 ? 0 : i + 1);

            const Polygon ear {poly[firstIdx], poly[i], poly[lastIdx]};

            // Check the point isn't colinear (and therefore the ear has no area)
            if (isColinear(ear[0], ear[1], ear[2])) {
                poly.erase(poly.begin() + i);
                break;
            }

            // Check the angle at the vertex at index i is convex
            if(!isAngleConvex(ear[0], ear[1], ear[2], clockwise))
                continue;

            // Check no other points are in the ear
            bool hit = false;
            for (size_t j = 0; j < poly.size(); ++j ) {
                // Don't check points that make up the ear
                if (j != firstIdx && j != i && j != lastIdx) {
                    if (earContains(ear, poly[j])) {
                        hit = true;
                        break;
                    }
                }
            }

            // If no other points of the polygon lie inside, then we have found an ear
            if (!hit) {
                // Perform hit detection
                if (earContains(ear, point))
                    return true;

                poly.erase(poly.begin() + i);
                break;
            }
        }
        // Prevent an inf loop
        infCheck++;
    }
    return false;
}

// This algorithm returns true if the point is within the ear OR on the boundary
// Note that the area calculation does not half the result to maintain integer precision
bool Impl::earContains(const Polygon& ear, const int2& point) const {
    if (ear.size() != 3)
        return false;

    const int2 A = ear[0];
    const int2 B = ear[1];
    const int2 C = ear[2];

    // ABC area x2
    const int area = triangleAreaDoubled(A, B, C);
    // PAB area x2
    const int PAB = triangleAreaDoubled(point, A, B);
    // PBC area x2
    const int PBC = triangleAreaDoubled(point, B, C);
    // PAC area x2
    const int PAC = triangleAreaDoubled(point, A, C);

    return area == PAB + PBC + PAC;
}

// Check whether the angle between BA and BC is convex
bool Impl::isAngleConvex(const int2& A, const int2& B, const int2& C, bool clockwise) const {
    const int xProduct = (A.x() - B.x()) * (C.y() - B.y()) - (A.y() - B.y()) * (C.x() - B.x());
    return clockwise ? xProduct > 0 : xProduct < 0;
}

// Check the winding order of the polygon
bool Impl::isClockwise(const Polygon& poly) const {
    if (poly.size() < 3)
        return false;

    int count = 0;
    for (int i = 0; i + 1 < poly.size(); ++i)
        count += (poly[i + 1].x() - poly[i].x()) * (poly[i + 1].y() + poly[i].y());

    // check the closing edge
    count += (poly.front().x() - poly.back().x()) * (poly.front().y() + poly.back().y());

    return count > 0;
}