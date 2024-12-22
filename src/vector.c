#include<math.h>
#include<vector.h>

Vector v_new(double x, double y) {
    Vector v = {.x = x, .y = y};
    return v;
}

Vector v_mul(Vector a, double scale) {
    Vector v = {
        .x = a.x * scale,
        .y = a.y * scale
    };
    return v;
}

Vector v_diff(Vector a, Vector b) {
    Vector v = {
        .x = a.x - b.x, 
        .y = a.y - b.y
    };
    return v;
}

Vector v_add(Vector a, Vector b) {
    Vector v = {
        .x = a.x + b.x, 
        .y = a.y + b.y
    };
    return v;
}

double v_dist2(Vector a, Vector b) {
    Vector v = v_diff(a, b);
    return pow(v.x, 2) + pow(v.y, 2);
}

double v_dist(Vector a, Vector b) {
    return sqrt(v_dist2(a, b));
}