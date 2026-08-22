#ifndef NOISE_H
#define NOISE_H

#include "vec2.h"

#define PERMUTATION_SIZE 256

void shuffle(int *arr, int arr_n) {
    for (int i = arr_n - 1; i > 0; i--) {
        int index = round(rand() % i);
        int temp = arr[i];

        arr[i] = arr[index];
        arr[index] = temp;
    }
}

int *make_permutation() {
    int *permutation = (int *)malloc(sizeof(int) * PERMUTATION_SIZE * 2);

    for (size_t i = 0; i < PERMUTATION_SIZE; i++)
        permutation[i] = i;

    shuffle(permutation, PERMUTATION_SIZE);

    for (size_t i = 0; i < PERMUTATION_SIZE; i++)
        permutation[PERMUTATION_SIZE + i] = permutation[i];

    return permutation;
}

Vec2 get_constant_vector(int value_of_perm_table) {
    int h = value_of_perm_table & 3;

    if (h == 0)
        return Vec2(1.0, 1.0);
    else if (h == 1)
        return Vec2(-1.0, 1.0);
    else if (h == 2)
        return Vec2(-1.0, -1.0);
    else
        return Vec2(1.0, -1.0);
}

float fade_curve(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

float lerp(float t, float a1, float a2) {
    return a1 + t * (a2 - a1);
}

float dot_product(Vec2 p, Vec2 p2) {
    return p.x * p2.x + p.y * p2.y;
}

float noise_2D(Vec2 vec, int *perm_table) {
    int X = (int)floor(vec.x) & 255;
    int Y = (int)floor(vec.y) & 255;

    float xf = vec.x - floor(vec.x);
    float yf = vec.y - floor(vec.y);

    Vec2 topRight = Vec2(xf - 1.0, yf - 1.0);
    Vec2 topLeft = Vec2(xf, yf - 1.0);
    Vec2 bottomRight = Vec2(xf - 1.0, yf);
    Vec2 bottomLeft = Vec2(xf, yf);

    int valueTopRight = perm_table[perm_table[X + 1] + Y + 1];
    int valueTopLeft = perm_table[perm_table[X] + Y + 1];
    int valueBottomRight = perm_table[perm_table[X + 1] + Y];
    int valueBottomLeft = perm_table[perm_table[X] + Y];

    float dotTopRight = dot_product(topRight, get_constant_vector(valueTopRight));
    float dotTopLeft = dot_product(topLeft, get_constant_vector(valueTopLeft));
    float dotBottomRight = dot_product(bottomRight, get_constant_vector(valueBottomRight));
    float dotBottomLeft = dot_product(bottomLeft, get_constant_vector(valueBottomLeft));

    float u = fade_curve(xf);
    float v = fade_curve(yf);

    return lerp(u, lerp(v, dotBottomLeft, dotTopLeft), lerp(v, dotBottomRight, dotTopRight));
}

float grad_3D(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x
                                             : z;

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise_3D(float x, float y, float z, int *perm_table) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = fade_curve(x);
    float v = fade_curve(y);
    float w = fade_curve(z);

    int A = perm_table[X] + Y;
    int AA = perm_table[A] + Z;
    int AB = perm_table[A + 1] + Z;
    int B = perm_table[X + 1] + Y;
    int BA = perm_table[B] + Z;
    int BB = perm_table[B + 1] + Z;

    return lerp(w,
                lerp(v,
                     lerp(u,
                          grad_3D(perm_table[AA], x, y, z),
                          grad_3D(perm_table[BA], x - 1, y, z)),
                     lerp(u,
                          grad_3D(perm_table[AB], x, y - 1, z),
                          grad_3D(perm_table[BB], x - 1, y - 1, z))),
                lerp(v,
                     lerp(u,
                          grad_3D(perm_table[AA + 1], x, y, z - 1),
                          grad_3D(perm_table[BA + 1], x - 1, y, z - 1)),
                     lerp(u,
                          grad_3D(perm_table[AB + 1], x, y - 1, z - 1),
                          grad_3D(perm_table[BB + 1], x - 1, y - 1, z - 1))));
}

#endif