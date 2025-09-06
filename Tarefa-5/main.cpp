#include "Image.h"

float G(float x, float y, float cx, float cy, float s){
	float dx = x - cx;
	float dy = y - cy;
	return 100*exp(-(dx*dx + dy*dy)/(s*s));
}

float F(float x, float y){
	return 50   
		- G(x, y, 300, 220, 95) //corpo
        - G(x, y, 300, 340, 60)//corpo
        - G(x, y, 300, 450, 40)//cabeça
        - G(x, y, 355, 120, 30)//perna direita
        - G(x, y, 355, 70, 25)// pé direito
        - G(x, y, 233, 130, 30)//perna esquerda
        - G(x, y, 180, 140, 25)//pé esquerdo
        - G(x, y, 220, 330, 20)// ombro esquerdo
        - G(x, y, 180, 350, 20)//cotovelo esquerda
        - G(x, y, 140, 360, 18)//mão esquerda
        - G(x, y, 380, 330, 20)//ombro direito
        - G(x, y, 420, 350, 20)//cotovovelo direito
        - G(x, y, 460, 360, 18);//mão direita
}

int main(){
	ImageRGB img(600, 600);
	img.fill(white);
	for(int y = 0; y < img.height(); y++)
		for(int x = 0; x < img.width(); x++)
			if(F(x,y) <= 0)
				img(x,y) = red;
		

	img.save("Boneco.png");
}