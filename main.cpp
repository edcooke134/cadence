#include <iostream>
#include <stdlib.h>
#include "int2.h"
#include "impl.h"

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cout << "Please use the input format <program> <text file> <x> <y>." << std::endl;
        return 0;
    }

    const std::string fileName = argv[1];
    const int2 point(atoi(argv[2]), atoi(argv[3])); 

    Impl impl(fileName);
    if(!impl.readFile()) {
        std::cout << "Failed to read file " << fileName << "." << std::endl;
        return 0;
    }

    const bool ret = impl.contains(point);

    std::cout << "Point " << point.x() << "," << point.y() << " is "
                        << (ret ? "INSIDE" : "OUTSIDE") << " the shape" << std::endl;

    return ret;
}