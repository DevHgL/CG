#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <vector>
#include <cmath>
#include <algorithm>
#include "cg2025.2/Image.h"
#include "vec.h"

//////////////////////////////////////////////////////////////////////////////

struct Pixel{
     int x, y;
};

inline Pixel toPixel(vec2 u){
     return { (int)round(u[0]), (int)round(u[1]) };
}

inline vec2 toVec2(Pixel p){
     return {(float)p.x, (float)p.y};
}

//////////////////////////////////////////////////////////////////////////////
// ALGORITMOS DE RASTERIZAÇÃO DE LINHAS (AULA 5)
//////////////////////////////////////////////////////////////////////////////

// Algoritmo Simples (y = mx + b) - Versão corrigida e mais robusta
inline std::vector<Pixel> simple(vec2 A, vec2 B){
    std::vector<Pixel> out;
    vec2 d = B - A;

    // CORREÇÃO: Trata o caso de retas verticais para evitar divisão por zero.
    if (std::abs(d[0]) < 1e-6) {
        int x = round(A[0]);
        int y_start = round(std::min(A[1], B[1]));
        int y_end = round(std::max(A[1], B[1]));
        for (int y = y_start; y <= y_end; y++) {
            out.push_back({x, y});
        }
        return out;
    }
    
    // CORREÇÃO: Garante que o desenho seja sempre da esquerda para a direita.
    if (A[0] > B[0]) {
        std::swap(A, B);
    }

    float m = (B[1] - A[1]) / (B[0] - A[0]);
    float b = A[1] - m * A[0];

    int x0 = (int)roundf(A[0]);
    int x1 = (int)roundf(B[0]);

    for(int x = x0; x <= x1; x++){
        int y = (int)roundf(m*x + b);
        out.push_back({x, y});
    }
    return out;
}

[cite_start]// Algoritmo DDA (Digital Differential Analyzer) [cite: 222]
inline std::vector<Pixel> dda(vec2 A, vec2 B){
     vec2 dif = B - A;
     float delta = std::max(fabs(dif[0]), fabs(dif[1]));

     if (delta == 0) { // Caso os pontos sejam iguais
          out.push_back(toPixel(A));
          return out;
     }

     vec2 d = (1/delta)*dif;
     vec2 p = A;

     std::vector<Pixel> out;
     for(int i = 0; i <= delta; i++){
          out.push_back(toPixel(p));
          p = p + d;
     }
     return out;
}

[cite_start]// Algoritmo de Bresenham [cite: 346]
inline std::vector<Pixel> bresenham_base(Pixel p0, Pixel p1){
     std::vector<Pixel> out;
     int dx = p1.x - p0.x;
     int dy = p1.y - p0.y;
     int D = 2*dy - dx; 
     int y = p0.y;
     for(int x = p0.x; x <= p1.x; x++){
          out.push_back({x, y});
          if(D > 0){
               y++;
               D -= 2*dx;
          }
          D += 2*dy;
     }
     return out;
}

inline std::vector<Pixel> bresenham(Pixel p0, Pixel p1){
     bool flip = abs(p1.y - p0.y) > abs(p1.x - p0.x);
     if(flip){
          std::swap(p0.x, p0.y);
          std::swap(p1.x, p1.y);
     }

     if(p0.x > p1.x)
          std::swap(p0, p1);

     std::vector<Pixel> out;
     int dx = p1.x - p0.x;
     int dy = abs(p1.y - p0.y);
     int D = 2*dy - dx; 
     int ystep = (p1.y > p0.y) ? 1 : -1;
     int y = p0.y;
     
     for(int x = p0.x; x <= p1.x; x++){
          if(flip)
               out.push_back({y, x});
          else
               out.push_back({x, y});

          if(D > 0){
               y += ystep;
               D -= 2*dx;
          }
          D += 2*dy;
     }
     return out;
}

//////////////////////////////////////////////////////////////////////////////
// FUNÇÃO PRINCIPAL DE RASTERIZAÇÃO DE LINHA
// Descomente a linha do algoritmo que você deseja testar.
//////////////////////////////////////////////////////////////////////////////

template<class Line>
std::vector<Pixel> rasterizeLine(const Line& P){
     //return simple(P[0], P[1]);
     //return dda(P[0], P[1]);
     return bresenham(toPixel(P[0]), toPixel(P[1]));
}

// ... (Resto do seu arquivo com a rasterização de triângulos) ...

#endif