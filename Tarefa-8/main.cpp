#include "Image.h"
#include "Render2D_v1.h"
#include "polygon_triangulation.h"
#include <vector>
#include <cmath>

int main(){
    // Parâmetros da estrela (mesmo desenho da Tarefa-7)
    const int numPoints = 5;
    const float R = 0.75f; // raio externo
    const float r = 0.32f; // raio interno

    // Gera a estrela centrada e normalizada
    std::vector<vec2> Q;
    Q.reserve(2*numPoints + 1);
    float phi0 = -M_PI/2.0f;
    for(int i = 0; i < 2*numPoints; ++i){
        float radius = (i % 2 == 0) ? R : r;
        float ang = phi0 + i * (M_PI/numPoints);
        Q.push_back({ radius * cosf(ang), radius * sinf(ang) });
    }
    // centraliza e escala para 90% do viewport
    vec2 centroid = {0,0};
    for(const auto& p: Q) centroid = centroid + p;
    centroid = (1.0f/static_cast<float>(Q.size())) * centroid;
    float maxr2 = 0.0f;
    for(auto& p: Q){ p = p - centroid; maxr2 = std::max(maxr2, dot(p,p)); }
    float maxr = sqrtf(maxr2);
    if(maxr > 0.0f){ float s = 0.9f / maxr; for(auto& p: Q) p = s * p; }
    Q.push_back(Q.front());

    // Triangula e renderiza (uma instância base)
    auto indices = triangulate_polygon(Q);
    Elements<Triangles> T{indices};
    LineLoop LS{Q.size()};

    ImageRGB I(900, 600); // não quadrada
    I.fill(white);

    // Composição: repetir a mesma estrela em uma grade
    const int rows = 3;
    const int cols = 5;
    const float tileScale = 0.28f; // escala por estrela para caber na grade

    for(int rI = 0; rI < rows; ++rI){
        for(int cI = 0; cI < cols; ++cI){
            // Centros espaçados uniformemente em [-0.9, 0.9]
            float cx = (cols == 1) ? 0.0f : -0.9f + 1.8f * (float)cI/(float)(cols-1);
            float cy = (rows == 1) ? 0.0f :  0.9f - 1.8f * (float)rI/(float)(rows-1);

            // Gera uma cópia transformada (escala uniforme + translação)
            std::vector<vec2> V = Q;
            for(auto& p: V){
                vec2 u = { p[0]*tileScale, p[1]*tileScale };
                p = { u[0] + cx, u[1] + cy };
            }

            // Renderiza preenchimento e contorno alternando cores
            RGB fillColor = ((rI + cI) % 2 == 0) ? orange : magenta;
            RGB lineColor = ((rI + cI) % 2 == 0) ? purple : blue;
            render2d(V, T, fillColor, I);
            render2d(V, LS, lineColor, I);
        }
    }

    I.save("Tarefa-8/output.png");
    return 0;
}


