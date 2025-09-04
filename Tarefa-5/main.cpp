#include "Image.h"
#include "rasterization.h"

int main() {
    int width = 500;
    int height = 500;
    ImageRGB image(width, height);
    image.fill(white);

    // Define os pontos das linhas a serem desenhadas
    vec2 p1 = {50, 50};
    vec2 p2 = {450, 450}; // Linha diagonal principal

    vec2 p3 = {20, 480};
    vec2 p4 = {480, 400}; // Linha com inclinação suave

    vec2 p5 = {480, 20};
    vec2 p6 = {400, 480}; // Linha com inclinação íngreme
    
    // Rasteriza e desenha as linhas
    for (Pixel p : rasterizeLine(std::array<vec2, 2>{p1, p2})) {
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height)
            image(p.x, p.y) = red;
    }

    for (Pixel p : rasterizeLine(std::array<vec2, 2>{p3, p4})) {
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height)
            image(p.x, p.y) = green;
    }

    for (Pixel p : rasterizeLine(std::array<vec2, 2>{p5, p6})) {
        if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height)
            image(p.x, p.y) = blue;
    }

    image.save("Tarefa-5/linhas_rasterizadas.png");
    std::cout << "Imagem salva como 'linhas_rasterizadas.png'" << std::endl;

    return 0;
}