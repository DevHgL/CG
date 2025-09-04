#include <iostream>
#include <vector>
#include <string>

// Inclui os cabeçalhos da biblioteca do curso.
#include "Image.h"
#include "Color.h"
#include "vec.h"

// Função para redimensionar um sprite usando o algoritmo "Nearest Neighbor".
// Recebe a imagem original e um fator de escala (ex: 0.8 para 80%, 1.5 para 150%).
ImageRGBA resize_sprite(const ImageRGBA& original, float scale) {
    if (scale <= 0) {
        return original; // Retorna o original se a escala for inválida
    }
    
    int new_width = static_cast<int>(original.width() * scale);
    int new_height = static_cast<int>(original.height() * scale);

    ImageRGBA resized(new_width, new_height);

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Encontra o pixel correspondente na imagem original
            int orig_x = static_cast<int>(x / scale);
            int orig_y = static_cast<int>(y / scale);
            
            // Copia a cor do pixel original para a nova imagem
            resized(x, y) = original(orig_x, orig_y);
        }
    }
    return resized;
}

// Sobrepõe uma cor RGBA (C) sobre uma cor de fundo RGB (Bg).
void over(RGB& Bg, RGBA C) {
    vec4 vc = toVec(C);
    float alpha = vc[3];

    if (alpha == 0) {
        return;
    }

    vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
    Bg = toColor(col);
}

// Posiciona um sprite (imagem RGBA) em uma posição (x, y) do cenário.
void place_sprite(ImageRGB& background, ImageRGBA& sprite, int x, int y) {
    for (int j = 0; j < sprite.height(); j++) {
        for (int i = 0; i < sprite.width(); i++) {
            int screen_x = x + i;
            int screen_y = y + j;

            if (screen_x >= 0 && screen_x < background.width() && screen_y >= 0 && screen_y < background.height()) {
                over(background(screen_x, screen_y), sprite(i, j));
            }
        }
    }
}

int main() {
    // 1. Carregar a imagem de fundo principal
    std::string fundo_path = "BG.jpg";
    ImageRGB cenario(fundo_path);
    if (cenario.size() == 0) {
        std::cerr << "Erro: Nao foi possivel carregar a imagem de fundo '" << fundo_path << "'.\n";
        return 1;
    }

    // 2. Carregar os sprites originais dos personagens
    std::string p1_path = "Images/png/female/Walk2.png";
    std::string p2_path = "Images/png/Run15.png";

    ImageRGBA personagem1_original(p1_path);
    ImageRGBA personagem2_original(p2_path);

    if (personagem1_original.size() == 0 || personagem2_original.size() == 0) {
        std::cerr << "Erro: Nao foi possivel carregar um ou mais sprites de personagem.\n";
        return 1;
    }
    
    // --- Redimensionar os personagens ---
    // AJUSTE OS VALORES DE ESCALA AQUI
    ImageRGBA personagem1 = resize_sprite(personagem1_original, 0.3f); // Personagem 1 com 20% do tamanho
    ImageRGBA personagem2 = resize_sprite(personagem2_original, 0.3f); // Personagem 2 com 40% do tamanho

    // 3. Montagem da Cena e Posicionamento Coerente
    // AJUSTE OS VALORES DE POSIÇÃO ABAIXO
    int altura_chao = 450; 

    // --- Personagem 1 ---
    int p1_x = 250;
    // A fórmula continua a mesma, mas agora usa a altura da imagem redimensionada
    int p1_y = 65;
    place_sprite(cenario, personagem1, p1_x, p1_y);

    // --- Personagem 2 ---
    int p2_x = 375;
    int p2_y = 45;
    place_sprite(cenario, personagem2, p2_x, p2_y);

    // 4. Salvar a Imagem Final
    std::string output_dir = "Tarefa-4";
    std::string output_path = output_dir + "/cenario_final.png";
    system(("mkdir -p " + output_dir).c_str()); 
    cenario.save(output_path);

    std::cout << "Cenario montado com sucesso e salvo como '" << output_path << "'!" << std::endl;

    return 0;
}