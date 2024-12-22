#pragma once

typedef struct Vector {
    double x;
    double y;
} Vector;

Vector v_new(double x, double y);
Vector v_mul(Vector a, double scale);
Vector v_diff(Vector a, Vector b);
Vector v_add(Vector a, Vector b);
double v_dist2(Vector a, Vector b);
double v_dist(Vector a, Vector b);