
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"
#include <stdbool.h>
#include <math.h>
Vec3 calculate_ray_direction(int pixel_x, int pixel_y, int image_width, int image_height);
Vec3 calculate_lighting(Vec3 point, Vec3 normal, Sphere sphere, Vec3 light_pos, float brightness);
bool is_in_shadow(Vec3 point, Vec3 light_dir, Sphere *spheres, int nospheres);
void renderPPM(Sphere *spheres, int nospheres, char *ppmfile);
void assignColorsToSpheres(Sphere *spheres, int numSpheres,int *colorHex, int numColors, int *ok);

Vec3 cameraPos = {.x = 0,.y = 0,.z = 0};
int imgWidth,imgHeight;
float vportHeight,focalLength;
float vportWidth;
Vec3 lightpos;
float lightBrightness;
int noColours;
int bgcolor;
int nospheres;
Vec3 backgroundColor;

void assignColorsToSpheres(Sphere *spheres, int numSpheres,int *colorHex, int numColors, int *ok) {

    Vec3 *colorRGB = malloc(numColors * sizeof(Vec3));

    for (int i = 0; i < numColors; i++) {
        colorRGB[i] = unpackRGB(colorHex[i]);
    }

    for (int i = 0; i < numSpheres; i++) {
        int colorIndex = ok[i];
        if (colorIndex >= 0 && colorIndex < numColors) {

            spheres[i].color = colorRGB[colorIndex];
        } else {
            spheres[i].color = (Vec3){1.0f, 1.0f, 1.0f};
        }
    }

    free(colorRGB);
}

void renderPPM(Sphere *spheres, int nospheres, char *ppmfile) {
    FILE *ppmFile = fopen(ppmfile, "w");
    if (!ppmFile) {
        fprintf(stderr, "Error: Unable to open PPM file for writing.\n");
        return;
    }

    fprintf(ppmFile, "P3\n%d %d\n255\n", imgWidth, imgHeight);

    for (int y = 0; y < imgHeight; y++) {
        for (int x = 0; x < imgWidth; x++) {

            Vec3 final_color = {0.0f, 0.0f, 0.0f}; // Initialize the final pixel color to black

            for (int grid_y = 0; grid_y < 3; grid_y++) {
                for (int grid_x = 0; grid_x < 3; grid_x++) {

                    float sub_x = (x + (grid_x - 1) + 0.5f) / imgWidth; // Adjusted center of each grid point
                    float sub_y = (y + (grid_y - 1) + 0.5f) / imgHeight;

                    Vec3 ray_dir = calculate_ray_direction(sub_x * imgWidth, sub_y * imgHeight, imgWidth, imgHeight);
                    Vec3 rayPos = cameraPos;
                    
                    float closest_t = INFINITY;
                    Sphere *closest_sphere = NULL;

                    for (int i = 0; i < nospheres; i++) {
                        float t;
                        if (doesIntersect(&spheres[i], rayPos, ray_dir, &t) && t < closest_t) {
                            closest_t = t;
                            closest_sphere = &spheres[i];
                        }
                    }

                    // If an intersection is found, calculate the lighting for the intersection point
                    if (closest_sphere) {
                        Vec3 intersection_point = add(rayPos, scalarMultiply(closest_t, ray_dir));
                        Vec3 normal = normalize(subtract(intersection_point, closest_sphere->pos));
                        Vec3 pixel_color = calculate_lighting(intersection_point, normal, *closest_sphere, lightpos, lightBrightness);

                        Vec3 light_dir = normalize(subtract(lightpos, intersection_point));
                        if (is_in_shadow(intersection_point, light_dir, spheres, nospheres)) {
                            pixel_color = scalarMultiply(0.1f, pixel_color); // Darken the color if in shadow
                        }

                        final_color = add(final_color, pixel_color);
                    } else {
#ifdef MS2
                        Vec3 pixel_color = {0.0f,0.0f,0.0f};
#else   
                        Vec3 pixel_color = backgroundColor;
                       
#endif
                        final_color = add(final_color, pixel_color);
                    }
                }
            }

            final_color = scalarMultiply(1.0f / 9.0f, final_color);

            writeColour(ppmFile, final_color);
        }
    }

    fclose(ppmFile);
}



int main(int argc,char *argv[]){
    (void)argc;
    FILE *fp;
    fp = fopen(argv[1], "r");
    fscanf(fp,"%d %d",&imgWidth,&imgHeight);
    fscanf(fp,"%f",&vportHeight);
    fscanf(fp,"%f",&focalLength);
    fscanf(fp, "%f %f %f %f", &lightpos.x, &lightpos.y, &lightpos.z, &lightBrightness);
    fscanf(fp,"%d",&noColours);
    int *hexColors = malloc(noColours * sizeof(int));
    for (int i = 0; i < noColours; i++) {
        fscanf(fp, "%x", &hexColors[i]);
    }
   
    qsort(hexColors, noColours, (int)sizeof(int), compareColor);
    
    fscanf(fp, "%d", &bgcolor);
    fscanf(fp, "%d", &nospheres);
    Sphere *spheres = malloc(nospheres * sizeof(Sphere));
    int *ok = malloc(nospheres * sizeof(int));
    for (int i = 0; i < nospheres; i++) {
        fscanf(fp, "%f %f %f %f %d",
               &spheres[i].pos.x,
               &spheres[i].pos.y,
               &spheres[i].pos.z,
               &spheres[i].r,
               &ok[i]); 
    }
    fclose(fp);

    backgroundColor = unpackRGB(hexColors[bgcolor]);
    #ifdef FS
    assignColorsToSpheres(spheres, nospheres, hexColors, noColours, ok);
    #endif

    vportWidth = vportHeight * (float)imgWidth / imgHeight;
    #ifdef MS1
    Vec3 bgcolor = {.x = 0,.y = 0,.z = 0};
    Vec3 spherecolor = {.x = 1,.y = 1,.z = 1};
    FILE *fp1;
    fp1 = fopen(argv[2], "w");

    Vec3 resultadd = add(bgcolor,lightpos);
    Vec3 resultsubtract = subtract(bgcolor,lightpos);
    Vec3 resultmultiply = scalarMultiply(vportWidth,lightpos);
    Vec3 resultnormalize = normalize(lightpos);

    typedef struct {
    Vec3 resultdivide;   
    float resultdot;          
    float resultdistance;     
    float resultlength;      
    } SphereResults;
    SphereResults *result = malloc(nospheres * sizeof(SphereResults));
    
    for (int i = 0; i < nospheres; i++) {
        result[i].resultdivide = scalarDivide(spherecolor,spheres[i].r);
        result[i].resultdot = dot(lightpos,spheres[i].pos);
        result[i].resultdistance = distance(lightpos,spheres[i].pos);
        result[i].resultlength = length(spheres[i].pos);
    }
    fprintf(fp1,"(0.0, 0.0, 0.0) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightpos.x,lightpos.y,lightpos.z,resultadd.x,resultadd.y,resultadd.z);
    fprintf(fp1,"(0.0, 0.0, 0.0) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightpos.x,lightpos.y,lightpos.z,resultsubtract.x,resultsubtract.y,resultsubtract.z);
    fprintf(fp1,"%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",vportWidth,lightpos.x,lightpos.y,lightpos.z,resultmultiply.x,resultmultiply.y,resultmultiply.z);
    fprintf(fp1,"normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",lightpos.x,lightpos.y,lightpos.z,resultnormalize.x,resultnormalize.y,resultnormalize.z);
    for (int i = 0; i < nospheres; i++){
        fprintf(fp1,"\n(1.0, 1.0, 1.0) / %.1f = (%.1f, %.1f, %.1f)\n",spheres[i].r,result[i].resultdivide.x,result[i].resultdivide.y,result[i].resultdivide.z);
        fprintf(fp1,"dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",lightpos.x,lightpos.y,lightpos.z,spheres[i].pos.x,spheres[i].pos.y,spheres[i].pos.z,result[i].resultdot);
        fprintf(fp1,"distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",lightpos.x,lightpos.y,lightpos.z,spheres[i].pos.x,spheres[i].pos.y,spheres[i].pos.z,result[i].resultdistance);
        fprintf(fp1,"length(%.1f, %.1f, %.1f) = %.1f\n",spheres[i].pos.x,spheres[i].pos.y,spheres[i].pos.z,result[i].resultlength);
        
    }
    fclose(fp1);
    free(result);
    #endif
    #ifdef MS2
     for (int i = 0; i < nospheres; i++) {
        spheres[i].color = (Vec3){1.0f, 1.0f, 1.0f}; 
    }
    renderPPM(spheres,nospheres,argv[2]);
    #endif
    #ifdef FS
    renderPPM(spheres,nospheres,argv[2]);
    #endif
    free(hexColors);
    free(ok);
    free(spheres); 
   
    
    return 0;
}

Vec3 calculate_ray_direction(int pixel_x, int pixel_y, int image_width, int image_height) {
   
    float u = (float)(pixel_x+0.5f ) / (float)image_width;
    float v = (float)(pixel_y+0.5f ) / (float)image_height;
    
    float viewport_x = (u-0.5f) * vportWidth;
    float viewport_y = (.5f-v) * vportHeight;
    float viewport_z = -focalLength; 

    Vec3 direction = {viewport_x, viewport_y, viewport_z};
    

    return normalize(direction); 
}
Vec3 calculate_lighting(Vec3 point, Vec3 normal, Sphere sphere, Vec3 light_pos, float brightness) {
    Vec3 light_dir = normalize(subtract(light_pos, point));
    float intensity = brightness * fmaxf(0.0f, dot(light_dir, normal)) /(distance2(light_pos, point));
    intensity = fminf(intensity,1.0f);
   
    Vec3 color = scalarMultiply(intensity, sphere.color);
    
    return color;
}

bool is_in_shadow(Vec3 point, Vec3 light_dir, Sphere *spheres, int nospheres) {
    Vec3 shadow_origin = add(point, scalarMultiply(0.001f, light_dir));
    float light_distance = length(subtract(lightpos, point)); 
    float t;

    for (int i = 0; i < nospheres; i++) {
        if (doesIntersect(&spheres[i], shadow_origin, light_dir, &t)) {
           
            if (t > 0 && t < light_distance) {
                return true; 
            }
        }
    }
    return false; 
}
