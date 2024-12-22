#include<math.h>
#include<vector.h>
#include<point.h>

Point p_new(
        double x     , double y, 
        double dx    , double dy, 
        double mass  , double charge, 
        double radius, uint8_t color[3]
) {
    Point p = {
        .pos = v_new(x, y),
        .disp = v_new(dx, dy),
        .mass = mass    , .charge = charge,
        .radius = radius
    };
    p.color[0] = color[0];
    p.color[1] = color[1];
    p.color[2] = color[2];
    return p;
}