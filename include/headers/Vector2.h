#pragma once
#include <math.h>
class Vector2{
public:
    Vector2(){};
    Vector2(float x, float y){
        this->x = x;
        this->y = y;
    }

    Vector2 operator+(Vector2 b){
        return Vector2(x+b.x, y+b.y);
    }

    Vector2 operator-(Vector2 b){
        return Vector2(x-b.x, y-b.y);
    }

    static float getDist(Vector2 a, Vector2 b){
        return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-a.y)*(a.y-a.y));
    }
    float x=0;
    float y=0;
};