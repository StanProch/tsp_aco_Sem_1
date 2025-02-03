#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <vector>
struct point {
    int ID;
    double x, y;
};

struct ant {
    int starting_point;
    std::vector<int> visited_points;
    double length_of_path;
    std::vector<int> path;
};
#endif //STRUCTURES_H
