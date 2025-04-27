## Cadence Interview Assignment
### Author: Edward Cooke

### How to build
This program can be built using `CMake` as follows:

Unix-like and MacOS:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -S <path to source dir> -B <path to build dir>
cmake --build <path to build dir> 
```

Windows:
```bash
cmake -S <path to source dir> -B <path to build dir>
cmake --build <path to build dir> --config Release
```

To run the executable:
```bash
./<path to build dir>/OrthHitDetection <text file> <x> <y>
```

### Overview
The solution provided does the following:
* Parses the text file and stores its content appropriately.
* Performs [polygon triangulation] on the given polygons, implementing an ear clipping algorithm.
* Prints to the command line “Point X,Y is INSIDE the shape” or “Point X,Y is OUTSIDE the shape” (where X,Y are the actual coordinate values) depending upon the answer calculated.
* Returns a different exit code depending on your answer, 0 for a point outside the shape or 1 for a point inside the shape.

### Notes
* The polygon in the text file must be simple (i.e. no overlapping edges).
* Winding order does not matter.
* The order of the algorithm is O(n^2). Be warned!

[polygon triangulation]: https://en.wikipedia.org/wiki/Polygon_triangulation