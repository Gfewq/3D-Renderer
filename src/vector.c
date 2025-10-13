#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2){
    Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result; 

}
Vec3 subtract(Vec3 v1, Vec3 v2){
    Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result; 

}
Vec3 scalarMultiply(float s, Vec3 v){
    Vec3 result;
    result.x = s*v.x;
    result.y = s*v.y;
    result.z = s*v.z;
    return result; 

}
Vec3 scalarDivide(Vec3 v, float d){
   Vec3 result;
    result.x = v.x/d;
    result.y = v.y/d;
    result.z = v.z/d;
    return result;  
}
float dot(Vec3 v1, Vec3 v2){
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}
float length(Vec3 v){
    float result = sqrt(dot(v,v));
    return result;
}
float length2(Vec3 v){
    float result = dot(v,v);
    return result;
}
float distance(Vec3 v1, Vec3 v2){
    float x = (v1.x - v2.x)*(v1.x - v2.x);
    float y = (v1.y - v2.y)*(v1.y - v2.y);
    float z = (v1.z - v2.z)*(v1.z - v2.z);
    float result = sqrt(x+y+z);
    return result;

}
float distance2(Vec3 v1, Vec3 v2){
    float x = (v1.x - v2.x)*(v1.x - v2.x);
    float y = (v1.y - v2.y)*(v1.y - v2.y);
    float z = (v1.z - v2.z)*(v1.z - v2.z);
    float result = x+y+z;
    return result;

}
Vec3 normalize(Vec3 v){
    float length = sqrt(dot(v,v));
    Vec3 result;
    result.x = v.x/length;
    result.y = v.y/length;
    result.z = v.z/length;
    return result; 

}

