#include "Image.h"
#include <algorithm>
#include <iostream>

// Função que sobrepõe uma cor RGBA sobre uma cor RGB
void over(RGB& Bg, RGBA C) {
    vec4 vc = toVec(C);
    float alpha = vc[3];
    vec3 col = lerp(alpha, toVec(Bg), toVec3(vc));
    Bg = toColor(col);
}

// Função para sobrepor com fator de escala
void over_scaled(ImageRGB& background, const ImageRGBA& foreground, int x_pos, int y_pos, float scale) {
    for (int j = 0; j < foreground.height(); ++j) {
        for (int i = 0; i < foreground.width(); ++i) {
            int dest_x = x_pos + static_cast<int>(i * scale);
            int dest_y = y_pos + static_cast<int>(j * scale);

            if (dest_x >= 0 && dest_x < background.width() && dest_y >= 0 && dest_y < background.height()) {
                over(background(dest_x, dest_y), foreground(i, j));
            }
        }
    }
}

int main() {
    // Definindo caminhos relativos para os arquivos.
    const std::string background_path = "Images/png/BG.png";
    const std::string tile_path = "Images/graveyardtilesetnew/png/Tiles/Tile1.png";
    const std::string tree_path = "Images/graveyardtilesetnew/png/Objects/Tree.png";
    const std::string tomb_path = "Images/graveyardtilesetnew/png/Objects/TombStone1.png";
    const std::string skeleton_path = "Images/graveyardtilesetnew/png/Objects/Skeleton.png";

    const std::string boy_path = "Images/png/male/Walk1.png";
    const std::string girl_path = "Images/png/female/Idle1.png";
    const std::string red_hat_boy_path = "Images/png/male/Idle1.png";

    ImageRGB background{background_path};
    if (background.width() == 0) { std::cerr << "Erro: Imagem de fundo não carregada: " << background_path << std::endl; return 1; }
    ImageRGBA tile{tile_path};
    if (tile.width() == 0) { std::cerr << "Erro: Imagem do tile não carregada: " << tile_path << std::endl; return 1; }
    ImageRGBA tree{tree_path};
    if (tree.width() == 0) { std::cerr << "Erro: Imagem da árvore não carregada: " << tree_path << std::endl; return 1; }
    ImageRGBA tombstone{tomb_path};
    if (tombstone.width() == 0) { std::cerr << "Erro: Imagem da lápide não carregada: " << tomb_path << std::endl; return 1; }
    ImageRGBA skeleton{skeleton_path};
    if (skeleton.width() == 0) { std::cerr << "Erro: Imagem do esqueleto não carregada: " << skeleton_path << std::endl; return 1; }
    ImageRGBA boy{boy_path};
    if (boy.width() == 0) { std::cerr << "Erro: Imagem do 'boy' não carregada: " << boy_path << std::endl; return 1; }
    ImageRGBA girl{girl_path};
    if (girl.width() == 0) { std::cerr << "Erro: Imagem da 'girl' não carregada: " << girl_path << std::endl; return 1; }
    ImageRGBA red_hat_boy{red_hat_boy_path};
    if (red_hat_boy.width() == 0) { std::cerr << "Erro: Imagem do 'red_hat_boy' não carregada: " << red_hat_boy_path << std::endl; return 1; }

    int background_width = background.width();
    int background_height = background.height();
    float scale = 0.5f;
    int tile_width_scaled = static_cast<int>(tile.width() * scale);
    int tile_height_scaled = static_cast<int>(tile.height() * scale);

    // 2. Criar o "chão" de tiles na PARTE INFERIOR do cenário com o novo tamanho
    int ground_level_y = background_height - tile_height_scaled;
    for (int x = 0; x < background_width; x += tile_width_scaled) {
        over_scaled(background, tile, x, ground_level_y, scale);
    }
    
    // 3. Adicionar objetos decorativos (posicionados acima do chão)
    // Posições X foram ajustadas para um espaçamento melhor
    int tree_x = 50, tree_y = ground_level_y - static_cast<int>(tree.height() * scale);
    over_scaled(background, tree, tree_x, tree_y, scale);
    
    int tomb_x = 250, tomb_y = ground_level_y - static_cast<int>(tombstone.height() * scale);
    over_scaled(background, tombstone, tomb_x, tomb_y, scale);
    
    int skeleton_x = 400, skeleton_y = ground_level_y - static_cast<int>(skeleton.height() * scale);
    over_scaled(background, skeleton, skeleton_x, skeleton_y, scale);
    
    // 4. Adicionar os personagens (posicionados acima do chão)
    int boy_x = 150, boy_y = ground_level_y - static_cast<int>(boy.height() * scale);
    over_scaled(background, boy, boy_x, boy_y, scale);

    int girl_x = 300, girl_y = ground_level_y - static_cast<int>(girl.height() * scale);
    over_scaled(background, girl, girl_x, girl_y, scale);

    int red_hat_boy_x = 600, red_hat_boy_y = ground_level_y - static_cast<int>(red_hat_boy.height() * scale);
    over_scaled(background, red_hat_boy, red_hat_boy_x, red_hat_boy_y, scale);

    // 5. Salvar a imagem final
    background.save("jogo.png");
    
    return 0;
}