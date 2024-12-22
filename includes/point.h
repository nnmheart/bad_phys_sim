#pragma once
#include<stdint.h>
#include<vector.h>

typedef struct Point {
    Vector pos;
    Vector disp;

    double mass;
    double charge;
    double radius;

    uint8_t color[3];
} Point;

Point p_new(
    double x     , double y, 
    double dx    , double dy, 
    double mass  , double charge, 
    double radius, uint8_t color[3]
);