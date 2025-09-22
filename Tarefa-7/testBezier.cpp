#include "Image.h"
#include "Render2D_v1.h"
#include "bezier.h"
#include "polygon_triangulation.h"
#include <vector>
#include <fstream>
#include <cmath>

// Gera pontos de controle para aproximar um círculo com 4 curvas de Bézier cúbicas
// Retorna 1 + 3*k pontos (k=4), formato esperado por sample_bezier_spline<3>
std::vector<vec2> generateCircleControlPoints(float radius, vec2 center={0,0}){
    const float kappa = 0.5522847498307936f; // 4*(sqrt(2)-1)/3
    const float r = radius;
    const float k = kappa * r;
    std::vector<vec2> P;
    P.reserve(13);
    // Começa em (r,0) e vai CCW: topo, esquerda, baixo, direita, fecha
    P.push_back(center + vec2{ r,  0});
    P.push_back(center + vec2{ r,  k});
    P.push_back(center + vec2{ k,  r});
    P.push_back(center + vec2{ 0,  r});
    P.push_back(center + vec2{-k,  r});
    P.push_back(center + vec2{-r,  k});
    P.push_back(center + vec2{-r,  0});
    P.push_back(center + vec2{-r, -k});
    P.push_back(center + vec2{-k, -r});
    P.push_back(center + vec2{ 0, -r});
    P.push_back(center + vec2{ k, -r});
    P.push_back(center + vec2{ r, -k});
    P.push_back(center + vec2{ r,  0}); // fecha
    return P;
}

int main() {
    // Parâmetros da estrela
    const int numPoints = 5;   // pontas
    const float R = 0.75f;     // raio externo
    const float r = 0.32f;     // raio interno

    // Gerar vértices da estrela (polígono simples)
    std::vector<vec2> Q;
    Q.reserve(2*numPoints + 1);
    float phi0 = -M_PI/2.0f; // começa para cima
    for(int i = 0; i < 2*numPoints; ++i){
        float radius = (i % 2 == 0) ? R : r;
        float ang = phi0 + i * (M_PI/numPoints);
        Q.push_back({ radius * cosf(ang), radius * sinf(ang) });
    }
    // Centralizar e normalizar escala para caber bem na imagem
    vec2 centroid = {0,0};
    for(const auto& p: Q) centroid = centroid + p;
    centroid = (1.0f/static_cast<float>(Q.size())) * centroid;
    float maxr2 = 0.0f;
    for(auto& p: Q){
        p = p - centroid;
        maxr2 = std::max(maxr2, dot(p,p));
    }
    float maxr = sqrtf(maxr2);
    if(maxr > 0.0f){
        float s = 0.9f / maxr;
        for(auto& p: Q) p = s * p;
    }
    Q.push_back(Q.front()); // fecha

    // Triangulação
    auto indices = triangulate_polygon(Q);
    Elements<Triangles> T{indices};

    // Imagem
    ImageRGB G(600, 600);
    G.fill(white);

    // Preenchimento e contorno
    render2d(Q, T, orange, G);
    LineLoop LS{Q.size()};
    render2d(Q, LS, purple, G);

    // Salvar
    G.savePNG("Tarefa-7/estrela.png");

    return 0;
}