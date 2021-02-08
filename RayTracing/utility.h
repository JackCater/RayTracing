#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

inline double random_double() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static std::mt19937 gen;
    return dist(gen);
}

inline double random_double(double min, double max) { return min + (max - min) * random_double(); }

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers have to be included here otherwise
// random_doulbe will not be identified by vec3.h

#include "ray.h"
#include "vec3.h"

#endif