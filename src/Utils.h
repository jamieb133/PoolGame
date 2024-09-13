#pragma once

#include <raylib.h>
#include <math.h>

static inline Vector2 MagnitudeToVector(float magnitude, float angle_deg)
{
    return (Vector2) {
        .x = cos(angle_deg * DEG2RAD) * magnitude,
        .y = sin(angle_deg * DEG2RAD) * magnitude
    };
}

static inline float VectorToMagnitude(Vector2 vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

static inline Vector2 Vector2_Normalise(Vector2 v)
{
    float magnitude = VectorToMagnitude(v);
    
    if (magnitude == 0) 
    {
        return (Vector2){ 0.0f, 0.0f };  // Prevent division by zero
    }   
    
    return (Vector2){ v.x / magnitude, v.y / magnitude };
}

static inline bool Vector2_Eq(Vector2 v1, Vector2 v2)
{
    return (v1.x == v2.x) && (v1.y == v2.y);
}

static inline bool Vector2_GT(Vector2 v1, Vector2 v2)
{
    return VectorToMagnitude(v1) > VectorToMagnitude(v2); 
}

static inline Vector2 Vector2_Add(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        v1.x + v2.x,
        v1.y + v2.y
    };
}

static inline Vector2 Vector2_AddMagnitude(Vector2 v, float mag)
{
    Vector2 normalised = Vector2_Normalise(v);
    Vector2 addedVector = (Vector2){ normalised.x * mag, normalised.y * mag };
    return (Vector2){ v.x + addedVector.x, v.y + addedVector.y };
}

static inline Vector2 Vector2_Subtract(Vector2 v1, Vector2 v2)
{
    return (Vector2) {
        v1.x - v2.x,
        v1.y - v2.y
    };
}

static inline bool ColorCmp(Color color1, Color color2) {
    return (color1.r == color2.r) &&
           (color1.g == color2.g) &&
           (color1.b == color2.b) &&
           (color1.a == color2.a);
}
