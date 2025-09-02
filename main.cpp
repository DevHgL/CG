#include "cg2025.2/Image.h"
#include <algorithm>
#include <iostream>

void over(RGB& Bg, RGBA C) {
    vec4 vc = toVec(C);
    float alpha = vc[3];
    vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
    Bg = toColor(col);
}

int main() {
    // Definindo caminhos relativos para os arquivos
    const std::string background_path = "Images/png/BG.png";
    const std::string tile_path = "Images/graveyardtilesetnew/png/Tiles/Tile1.png";
    const std::string tree_path = "Images/graveyardtilesetnew/png/Objects/Tree.png";
    const std::string tomb_path = "Images/graveyardtilesetnew/png/Objects/TombStone1.png";
    const std::string skeleton_path = "Images/graveyardtilesetnew/png/Objects/Skeleton.png";

    // Caminhos para os personagens (assumindo que foram renomeados ou são válidos)
    const std::string boy_path = "Images/png/male/Walk1.png"; 
    const std::string girl_path = "Images/png/female/Idle1.png"; 
    const std::string red_hat_boy_path = "Images/png/male/Idle1.png";

    // 1. Carregar as imagens
    ImageRGB background{background_path};
    ImageRGBA tile{tile_path};
    ImageRGBA tree{tree_path};
    ImageRGBA tombstone{tomb_path};
    ImageRGBA skeleton{skeleton_path};
    ImageRGBA boy{boy_path}; 
    ImageRGBA girl{girl_path}; 
    ImageRGBA red_hat_boy{red_hat_boy_path};

    // Verificação de carregamento (essencial para depuração)
    if (background.width() == 0) { std::cerr << "Erro: Imagem de fundo não carregada: " << background_path << std::endl; return 1; }
    if (tile.width() == 0) { std::cerr << "Erro: Imagem do tile não carregada: " << tile_path << std::endl; return 1; }
    if (tree.width() == 0) { std::cerr << "Erro: Imagem da árvore não carregada: " << tree_path << std::endl; return 1; }
    if (tombstone.width() == 0) { std::cerr << "Erro: Imagem da lápide não carregada: " << tomb_path << std::endl; return 1; }
    if (skeleton.width() == 0) { std::cerr << "Erro: Imagem do esqueleto não carregada: " << skeleton_path << std::endl; return 1; }
    if (boy.width() == 0) { std::cerr << "Erro: Imagem do 'boy' não carregada: " << boy_path << std::endl; return 1; }
    if (girl.width() == 0) { std::cerr << "Erro: Imagem da 'girl' não carregada: " << girl_path << std::endl; return 1; }
    if (red_hat_boy.width() == 0) { std::cerr << "Erro: Imagem do 'red_hat_boy' não carregada: " << red_hat_boy_path << std::endl; return 1; }

    int background_width = background.width();
    int background_height = background.height();
    int tile_width = tile.width();
    int tile_height = tile.height();

    // 2. Criar o "chão" de tiles na PARTE INFERIOR do cenário
    int ground_level_y = background_height - tile_height;
    for (int x = 0; x < background_width; x += tile_width) {
        for (int j = 0; j < tile_height; ++j) {
            for (int i = 0; i < tile_width; ++i) {
                if (x + i < background_width && ground_level_y + j < background_height) {
                    over(background(x + i, ground_level_y + j), tile(i, j));
                }
            }
        }
    }

    // 3. Adicionar objetos decorativos (posicionados acima do chão)
    int tree_x = 50, tree_y = ground_level_y - tree.height() + 10;
    for(int j = 0; j < tree.height(); j++) {
        for(int i = 0; i < tree.width(); i++) {
            if (tree_x + i < background_width && tree_y + j < background_height) {
                over(background(tree_x + i, tree_y + j), tree(i, j));
            }
        }
    }
    
    int tomb_x = 250, tomb_y = ground_level_y - tombstone.height() + 5;
    for(int j = 0; j < tombstone.height(); j++) {
        for(int i = 0; i < tombstone.width(); i++) {
            if (tomb_x + i < background_width && tomb_y + j < background_height) {
                over(background(tomb_x + i, tomb_y + j), tombstone(i, j));
            }
        }
    }
    
    int skeleton_x = 400, skeleton_y = ground_level_y - skeleton.height();
    for(int j = 0; j < skeleton.height(); j++) {
        for(int i = 0; i < skeleton.width(); i++) {
            if (skeleton_x + i < background_width && skeleton_y + j < background_height) {
                over(background(skeleton_x + i, skeleton_y + j), skeleton(i, j));
            }
        }
    }
    
    // 4. Adicionar os personagens (posicionados acima do chão)
    int boy_x = 150, boy_y = ground_level_y - boy.height();
    for(int j = 0; j < boy.height(); j++) {
        for(int i = 0; i < boy.width(); i++) {
            if (boy_x + i < background_width && boy_y + j < background_height) {
                over(background(boy_x + i, boy_y + j), boy(i, j));
            }
        }
    }

    int girl_x = 300, girl_y = ground_level_y - girl.height();
    for(int j = 0; j < girl.height(); j++) {
        for(int i = 0; i < girl.width(); i++) {
            if (girl_x + i < background_width && girl_y + j < background_height) {
                over(background(girl_x + i, girl_y + j), girl(i, j));
            }
        }
    }

    int red_hat_boy_x = 600, red_hat_boy_y = ground_level_y - red_hat_boy.height();
    for(int j = 0; j < red_hat_boy.height(); j++) {
        for(int i = 0; i < red_hat_boy.width(); i++) {
            if (red_hat_boy_x + i < background_width && red_hat_boy_y + j < background_height) {
                over(background(red_hat_boy_x + i, red_hat_boy_y + j), red_hat_boy(i, j));
            }
        }
    }

    // 5. Salvar a imagem final
    background.save("Jogo.jpg");
    
    return 0;
}