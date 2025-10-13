#include "color.h"
void writeColour(FILE *ppmFile, Vec3 color) {
    fprintf(ppmFile, "%d %d %d\n",
            (int)(color.x * 255.0f),
            (int)(color.y * 255.0f),
            (int)(color.z * 255.0f));
}
Vec3 unpackRGB(unsigned int packedRGB) {
    unsigned int r = (packedRGB >> 16) & 0xFF;
    unsigned int g = (packedRGB >> 8) & 0xFF;
    unsigned int b = packedRGB & 0xFF;
    return (Vec3){r / 255.0f, g / 255.0f, b / 255.0f};
}


int compareColor(const void *a, const void *b) {
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++) {
        a1 |= (*((unsigned char *)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char *)b + i) & 0x0F) << (i * 8);
    }
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int *)a) < *((int *)b)) ? -1 : (*((int *)a) > *((int *)b)) ? 1 : 0;
}

