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
    // Atualize estes caminhos para os arquivos descompactados
    const std::string boy_path = "Images/png/Walk (15).png"; 
    const std::string girl_path = "Images/png/female/Idle (6).png"; 
    const std::string red_hat_boy_path = "Images/png/Idle (15).png"; 

    // 1. Carregar as imagens
    ImageRGB img{background_path};
    ImageRGBA boy{boy_path};
    ImageRGBA cute_girl{girl_path};
    ImageRGBA red_hat_boy{red_hat_boy_path}; 

    // Verifique se as imagens foram carregadas corretamente
    if (img.width() == 0 || boy.width() == 0 || cute_girl.width() == 0 || red_hat_boy.width() == 0) {
        printf("Erro ao carregar uma ou mais imagens. Verifique os caminhos.\n");
        return 1;
    }
    
    // 2. Definir as posições dos personagens.
    int boy_x = 280, boy_y = 108;
    int girl_x = 100, girl_y = 150;
    int red_hat_x = 450, red_hat_y = 120;
    
    // 3. Sobrepor os personagens na imagem de fundo.
    
    // Sobrepor o primeiro personagem (boy)
    for(int j = 0; j < boy.height(); j++) {
        for(int i = 0; i < boy.width(); i++) {
            if (boy_x + i < img.width() && boy_y + j < img.height()) {
                over(img(boy_x + i, boy_y + j), boy(i, j));
            }
        }
    }
    
    // Sobrepor o segundo personagem (cute_girl)
    for(int j = 0; j < cute_girl.height(); j++) {
        for(int i = 0; i < cute_girl.width(); i++) {
            if (girl_x + i < img.width() && girl_y + j < img.height()) {
                over(img(girl_x + i, girl_y + j), cute_girl(i, j));
            }
        }
    }

    // Sobrepor o terceiro personagem (red_hat_boy)
    for(int j = 0; j < red_hat_boy.height(); j++) {
        for(int i = 0; i < red_hat_boy.width(); i++) {
            if (red_hat_x + i < img.width() && red_hat_y + j < img.height()) {
                over(img(red_hat_x + i, red_hat_y + j), red_hat_boy(i, j));
            }
        }
    }

    // 4. Salvar a imagem final
    img.save("output.jpg");
    
    return 0;
}