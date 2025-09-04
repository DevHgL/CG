#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include "Image.h"
#include "vec.h"

struct Pixel{
    int x, y;
};

inline Pixel toPixel(vec2 u){
    return { (int)round(u[0]), (int)round(u[1]) };
}

inline vec2 toVec2(Pixel p){
    return {(float)p.x, (float)p.y};
}

// Funções helper para ignorar NAN, necessárias para o scanline.
inline float nan_min(float a, float b) {
    if (std::isnan(a)) return b;
    if (std::isnan(b)) return a;
    return std::min(a, b);
}

inline float nan_max(float a, float b) {
    if (std::isnan(a)) return b;
    if (std::isnan(b)) return a;
    return std::max(a, b);
}

// Versão FINALMENTE CORRIGIDA da função intersection
inline float intersection(const vec2& A, const vec2& B, int ys_int) {
    const float epsilon = 1e-6f;
    float ys = static_cast<float>(ys_int);

    float y_min = std::min(A[1], B[1]);
    float y_max = std::max(A[1], B[1]);

    // Caso 1: A linha de varredura está fora dos limites verticais da aresta.
    if (ys < y_min - epsilon || ys > y_max + epsilon) {
        return std::numeric_limits<float>::quiet_NaN();
    }

    // Caso 2: Aresta horizontal.
    if (std::abs(A[1] - B[1]) < epsilon) {
        // O teste espera a coordenada x do primeiro ponto passado como parâmetro.
        // Isso permite ao scanline obter os dois pontos da aresta horizontal
        // (um da aresta AB e outro da BC, por exemplo).
        return A[0];
    }

    // Caso 3: Interpolação Linear para o caso geral.
    float t = (ys - A[1]) / (B[1] - A[1]);
    return A[0] + t * (B[0] - A[0]);
}


// A função scanline usa a intersection corrigida.
template<class Tri>
std::vector<Pixel> scanline(const Tri& P){
    std::vector<Pixel> out;
    vec2 A = P[0], B = P[1], C = P[2];
    
    int ymin = static_cast<int>(ceil(std::min({A[1], B[1], C[1]})));
    int ymax = static_cast<int>(floor(std::max({A[1], B[1], C[1]})));
    
    for (int ys = ymin; ys <= ymax; ++ys) {
        float iAB = intersection(A, B, ys);
        float iBC = intersection(B, C, ys);
        float iCA = intersection(C, A, ys);
        
        float xmin_f = nan_min(iAB, nan_min(iBC, iCA));
        float xmax_f = nan_max(iAB, nan_max(iBC, iCA));
        
        if(std::isnan(xmin_f) || std::isnan(xmax_f)) continue;
            
        for (int x = static_cast<int>(ceil(xmin_f)); x <= static_cast<int>(floor(xmax_f)); ++x) {
            out.push_back({x, ys});
        }
    }
    return out;
}

template<class Tri>
std::vector<Pixel> rasterizeTriangle(const Tri& P){
    return scanline(P);
}

#endif