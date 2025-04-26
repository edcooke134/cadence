#include "impl.h"
#include <iostream>
#include <stdlib.h>
#include <fstream>

Impl::Impl(const std::string& fileName) : m_fileName(fileName) {
    readFile();
    // OUTPUT TEST
    std::cout << "OUTLINE: ";
    for (auto& el : m_outline)
        std::cout << el.x() << " " << el.y() << " ";
    std::cout << std::endl;

    std::cout << "CUTS:" << std::endl;
    for (auto& cut : m_cuts) {
        std::cout << "CUT: ";
        for (auto& el : cut) {
            std::cout << el.x() << " " << el.y() << " ";
        }
    } 
    std::cout << std::endl;
}

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
                        }
                        else {
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
                    std::cout << "SHOULDnt HIT THIS!!!!!!" << std::endl;
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

bool Impl::contains(const int2& point) {
    // check whether the point is inside the cuts, ignore edges
    for (auto& cut : m_cuts) {
        if (detectWithEarClipping(cut, point, true))
            return false;
    }

    // check whether the point is inside the outline
    return detectWithEarClipping(m_outline, point);
}

// use ear clipping to split the (simple) polygon, checking if the point is contained
// in any of the ears. Return true if it contained within the polygon. 
bool Impl::detectWithEarClipping(Polygon poly, const int2& point, bool ignoreEdges) const {
    if (poly.size() < 3)
        return false;
    // if we do not want to include the edges, check that the point is not on the edge
    if (ignoreEdges) {
        for (int i = 0; i + 1 < poly.size(); ++i) {
            if (colinearCheck(poly[i], point, poly[i + 1]))
                return false;
        }
        // check the closing edge of the polygon
        if (colinearCheck(poly.front(), point, poly.back()))
            return false;
    }

    int count = 0;
    // ear clipping
    while (poly.size() > 2 && count < 10000) {
        // iterate through points until we find an ear
        for (int i = 0; i < poly.size(); ++i) {
            // find the indices before and after i, wrapping if necessary
            const size_t firstIdx = (i == 0 ? poly.size() - 1 : i - 1);
            const size_t lastIdx = (i == poly.size() - 1 ? 0 : i + 1);

            const Polygon ear {poly[firstIdx], poly[i], poly[lastIdx]};

            // check no other points are in the ear
            bool hit = false;
            for (size_t j = 0; j < poly.size(); ++j ) {
                // don't check points that make up the ear
                if (j != firstIdx && j != i && j != lastIdx) {
                    if (earContains(ear, poly[j])) {
                        hit = true;
                        break;
                    }
                }
            }
            // if no other points of the polygon lie inside, then we have found an ear
            if (!hit) {
                // perform hit detection
                if (earContains(ear, point))
                    return true;
                poly.erase(poly.begin() + i);
                break;
            }
        }
        // prevent an inf loop
        count++;
    }
    return false;
}

// This algorithm returns true if the point is within the ear OR on the boundary
bool Impl::earContains(const Polygon& ear, const int2& point) const {
    if (ear.size() != 3)
        return false;
    const int2 A = ear[0];
    const int2 B = ear[1];
    const int2 C = ear[2];

    // ABC area x2
    const int area = earAreaDoubled(A, B, C);
    // PAB area x2
    const int PAB = earAreaDoubled(point, A, B);
    // PBC area x2
    const int PBC = earAreaDoubled(point, B, C);
    // PAC area x2
    const int PAC = earAreaDoubled(point, A, C);
    
   return area == PAB + PBC + PAC;
}