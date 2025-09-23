#include "Image.h"
#include "Render2D_v1.h"
#include "polygon_triangulation.h"
#include "transforms2d.h"
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

    // Composição: círculo de estrelas rotacionadas
    const int count = 12;         // número de estrelas no anel
    const float ringR = 0.65f;    // raio do anel
    const float starScale = 0.25f;// escala uniforme de cada estrela

    for(int i = 0; i < count; ++i){
        float ang = i * 2.0f * M_PI / count;
        vec2 c = { ringR * cosf(ang), ringR * sinf(ang) };

        // M = T(c) * R(ang) * S (estrela aponta para fora do círculo)
        mat3 S = scale(starScale, starScale);
        mat3 Rz = rotate_2d(ang);
        mat3 Tc = translate(c);
        mat3 M = Tc * Rz * S;

        // aplica transformação na estrela base
        auto V = M * Q; // retorna std::vector<vec3>

        // alterna cores
        RGB fillColor = (i % 2 == 0) ? orange : magenta;
        RGB lineColor = (i % 2 == 0) ? purple : blue;
        render2d(V, T, fillColor, I);
        render2d(V, LS, lineColor, I);
    }

    I.save("Tarefa-8/output.png");
    return 0;
}


