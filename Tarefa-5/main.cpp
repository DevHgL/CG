#include "Image.h"
#include "vec.h"
#include <iostream>

// Funçao gaussiana
float G(float x, float y, float cx, float cy, float s){
    float dx = x - cx;
    float dy = y - cy;
    return 100 * exp(-(dx*dx + dy*dy) / (s*s));
}

// Funçao que combina as formas gaussianas
float F(float x, float y){
    // O valor 50 define o limite para a forma.
    return 50.0f
        - G(x, y, 300, 220, 95) // Corpo
        - G(x, y, 300, 340, 60)// Corpo inferior
        - G(x, y, 300, 450, 40)// Cabeça
        - G(x, y, 355, 120, 30)// Perna direita
        - G(x, y, 355, 70, 25)// Pé direito
        - G(x, y, 233, 130, 30)// Perna esquerda
        - G(x, y, 180, 140, 25)// Pé esquerdo
        - G(x, y, 220, 330, 20)// Ombro esquerdo
        - G(x, y, 180, 350, 20)// Cotovelo esquerda
        - G(x, y, 140, 360, 18)// Mão esquerda
        - G(x, y, 380, 330, 20)// Ombro direito
        - G(x, y, 420, 350, 20)// Cotovelo direito
        - G(x, y, 460, 360, 18);// Mão direita
}

int main(){
    int width = 600;
    int height = 600;
    ImageRGB img(width, height);
    img.fill(white);

    // Rasteriza a forma definida por F(x,y)
    for(int y = 0; y < img.height(); y++){
        for(int x = 0; x < img.width(); x++){
            if(F(x,y) <= 0){
                img(x,y) = red;
            }
        }
    }

    // Salva a imagem na pasta 'Tarefa-5/'
    img.save("Tarefa-5/output.png");
    std::cout << "Imagem salva como 'Tarefa-5/output.png'" << std::endl;

    return 0;
}