#include <iostream>
#include <stdlib.h>
#include <vector>
#include "int2.h"
#include "impl.h"

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cout << "Please use the input format <program> <text file> <x> <y>." << std::endl;
        return 0;
    }

    const std::string fileName = argv[1];
    const int x = atoi(argv[2]);
    const int y = atoi(argv[3]);

    std::cout << "file name: " << fileName << ", X: " << x << ", Y: " << y << std::endl;

    Impl impl(fileName);
    const bool ret = impl.contains({x, y});

    std::cout << "Point " << x << "," << y << " is "
                        << (ret ? "INSIDE" : "OUTSIDE") << " the shape" << std::endl;

    return ret;
}