// Tarefa-6/main.cpp - CÓDIGO CORRIGIDO E LIMPO

#include "Image.h"
#include "rasterization.h" // Inclui TODAS as funções de rasterização
#include <iostream>

int main() {
    std::cout << ">>> ESTOU EXECUTANDO O CODIGO MODIFICADO <<<" << std::endl;
    int width = 500;
    int height = 500;
    ImageRGB image(width, height);
    image.fill(white);

    // Define um triângulo para testar o algoritmo scanline
    vec2 T[] = {
        {50, 50},    // Vértice A
        {450, 150},  // Vértice B
        {200, 450}   // Vértice C
    };

    // Chama a função de rasterização que está DENTRO de rasterization.h
    for (Pixel p : rasterizeTriangle(T)) {
        // Pinta o pixel na imagem, com verificação de limites
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height) {
            image(p.x, p.y) = blue;
        }
    }

    image.save("Tarefa-6/triangulo_rasterizado.png");
    std::cout << "Imagem salva como 'Tarefa-6/triangulo_rasterizado.png'" << std::endl;

    return 0;
}