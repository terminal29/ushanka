#pragma once
#include <cstdint>

struct RGBColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static float hue2rgb(float p, float q, float t) {

    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if (t < 1. / 6)
        return p + (q - p) * 6 * t;
    if (t < 1. / 2)
        return q;
    if (t < 2. / 3)
        return p + (q - p) * (2. / 3 - t) * 6;

    return p;

}

inline RGBColor HSLToRGB(float h, float s, float l) {
    RGBColor result;
    float r;
    float g;
    float b;

    if (0 == s) {
        r = g = b = l; // achromatic
    }
    else {
        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        r = hue2rgb(p, q, h + 1. / 3) * 255;
        g = hue2rgb(p, q, h) * 255;
        b = hue2rgb(p, q, h - 1. / 3) * 255;
    }

    return RGBColor{ static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
}