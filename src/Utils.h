#pragma once

#include <raylib.h>

static inline bool ColorCmp(Color color1, Color color2) {
    return (color1.r == color2.r) &&
           (color1.g == color2.g) &&
           (color1.b == color2.b) &&
           (color1.a == color2.a);
}
