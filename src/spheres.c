#include "spheres.h"
#include <stdlib.h>
#include <math.h>


void worldInit(World *world) {
    world->size = 0;
    world->capacity = 1;
    world->spheres = malloc(sizeof(Sphere*));
}

void freeWorld(World *world) {
    for (int i = 0; i < world->size; i++) {
        free(world->spheres[i]);
    }
    free(world->spheres);

    world->size = 0;
    world->capacity = 0;
}

void addSphere(World *world, Sphere *sphere) {
    if (world->size >= world->capacity) {
        world->capacity = world->size+1;
        Sphere **newArray = realloc(world->spheres, world->capacity * sizeof(Sphere*));
        world->spheres = newArray;
    }
    
    world->spheres[world->size++] = sphere;
}

Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    
    return sphere;
}
int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t){
  

    // Get a, b, c for the quadratic formula
    Vec3 V = subtract(rayPos, sphere->pos);
    float a = dot(rayDir, rayDir);
    float b = 2*dot(rayDir, V);
    float c = dot(V, V) - sphere->r * sphere->r;
    float discriminant = (b*b)-4*a*c;

    // Check the discriminant.
    // This will tell us if there are any intersections.
    // Does not intersect (complex roots)
    if (discriminant <  0){
        return 0;
    }
    // Does intersect (real root(s))

    // Solve for t using the quadratic formula
    float t1, t2;
    float sqrtDiscriminant = sqrt(discriminant);
    t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    t2 = (-b + sqrtDiscriminant) / (2.0f * a);
   
   

     if (t1 > 0 && t2 > 0) {
        *t = fminf(t1, t2); // Take the smaller t value
        return 1;
    } else if (t1 > 0) {
        *t = t1; 
        return 1;
    } else if (t2 > 0) {
        *t = t2; 
        return 1;
    }
    
    return 0;
}