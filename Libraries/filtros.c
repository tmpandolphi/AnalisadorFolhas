#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetor.h"
#include "filtros.h"
#include "imagem.h"
#include <math.h>

static double Min(double a, double b) {
	return a <= b ? a : b;
}

static double Max(double a, double b) {
	return a >= b ? a : b;
}

RGB** grayscale(RGB **matriz, int altura, int largura) {
	int i, j, k;
	
	for (i=0 ; i < altura; i++) {
		for (j=0 ; j < largura; j++) {
			matriz[i][j].r = matriz[i][j].g = matriz[i][j].b = ((matriz[i][j].r + matriz[i][j].g + matriz[i][j].b)/3);
		}
	}
	
	return matriz;
}

RGB** bw(RGB **matriz, CABECALHO_IMAGEM cabecalho_i, int limiar) {
	int i, j, k;
	
	for (i=0 ; i<cabecalho_i.alturaImagem; i++) {
		for (j=0 ; j<cabecalho_i.larguraImagem ; j++) {
			matriz[i][j].r = matriz[i][j].g = matriz[i][j].b = (matriz[i][j].r > limiar?255:0);
		}
	}
	
	return matriz;
}

int* geraHistograma(RGB **matriz, int altura, int largura) {
	int i, j;
	static int histograma[256] = {0};
	
	for (i = 0; i < altura; i++) {
	    for (j = 0 ; j < largura; j++) {		
			histograma[matriz[i][j].r]++;
		}
	}
	
	return histograma;
}

void escreveHistograma(int *hist, int n) {
	int i, j;
	
	for (i = 0; i < n; i++) {
		printf("[%03d] ", i);
		for ( j = 0; j < hist[i] / 40; ++j) {
			printf("*");
		}
		printf("\n");
   }
}

int calculaLimiar(int *histograma, int indices) {
	int i, j, k;
	int limiar = NULL;
	double variacaoClasse = 0, novaVariacaoClasse = 0;
	
	double pesoB, mediaB, variancaB, niveisB;
	double pesoF, mediaF, variancaF, niveisF;
	
	for (i = 0; i < indices; i++) {
		pesoB = mediaB = variancaB = niveisB = 0;
		pesoF = mediaF = variancaF = niveisF = 0;
		
		for (j = 0; j < i; j++) {
			pesoB += histograma[j];
			mediaB += histograma[j] * j;
			niveisB += histograma[j];
		}
				
		pesoB /= indices;
		mediaB = niveisB > 0?mediaB / niveisB:0;
		
		for (j = 0; j < i; j++) {
			variancaB += ((j - mediaB) * (j - mediaB)) * histograma[j];
		}
		variancaB = niveisB > 0?variancaB / niveisB:0;
		
		// ----------------------------
		
		for (k = i; k < indices; k++) {
			pesoF += histograma[k];
			mediaF += histograma[k] * k;
			niveisF += histograma[k];
		}
		
		pesoF /= indices;
		mediaF = niveisF > 0?mediaF / niveisF:0;
		
		for (k = i; k < indices; k++) {
			variancaF += ((k - mediaF) * (k - mediaF)) * histograma[k];
		}
		
		variancaF = niveisF > 0?variancaF / niveisF:0;
		
		novaVariacaoClasse = pesoB * variancaB + pesoF * variancaF;
		
		if (i == 0 || novaVariacaoClasse < variacaoClasse) {
			limiar = i+1;
			variacaoClasse = novaVariacaoClasse;
		}
	}
	printf("limiar: %d\n", limiar);
	return limiar;
}

void ordena(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
          while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void mediana(RGB **matriz, int largura, int altura, int tamanho) {
	int i, j, k, l, m, n, limiteL, limiteA;
	
	int * valoresR;
	int * valoresG;
	int * valoresB;
	
	//int pos = ((tamanho * tamanho) / 2) + 1;
	
	/*RGB **copia = (RGB **)malloc(sizeof(RGB *)*altura);

	for (i=0;i<altura;i++){
		copia[i] = (RGB *)malloc(sizeof(RGB)*largura);
	}
	
	for (i=0 ; i<altura; i++) {
		for (j=0 ; j<largura ; j++) {
			copia[i][j] = matriz[i][j];
		}
	}*/
	//RGB **copia = matriz;
	RGB **copia = criarMatriz(altura, largura);
	copiaMatriz(matriz, copia, altura, largura);
	
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			k = i - (tamanho / 2) > 0?i - (tamanho / 2):0;
			l = j - (tamanho / 2) > 0?j - (tamanho / 2):0;

			limiteA = i + (tamanho / 2) < altura - 1?i + (tamanho / 2):altura - 1;
			limiteL = j + (tamanho / 2) < largura - 1?j + (tamanho / 2):largura - 1;

			valoresR = (int *)malloc(sizeof(int) * (limiteA + 1 - k) * (limiteL + 1 - l));
			valoresG = (int *)malloc(sizeof(int) * (limiteA + 1 - k) * (limiteL + 1 - l));
			valoresB = (int *)malloc(sizeof(int) * (limiteA + 1 - k) * (limiteL + 1 - l));

			//printf("k %d\n", k);
			//printf("limiteA %d\n", limiteA);

			//printf("l %d\n", l);
			//printf("limiteL %d\n", limiteL);

			//printf("qtde %d\n", (limiteA + 1 - k) * (limiteL + 1 - l));
	
			int count = 0;
							
			for (m = k ; m <= limiteA; m++) {
				for (n = l ; n <= limiteL; n++) {
					valoresR[count] = copia[m][n].r;
					valoresG[count] = copia[m][n].g;
					valoresB[count] = copia[m][n].b;
					count++;
				}
			}
				
			ordena(valoresR, count);
			ordena(valoresG, count);
			ordena(valoresB, count);
				
			if (count % 2 == 0) {
				matriz[i][j].r = (valoresR[count/2] + valoresR[count/2+1]) / 2;
				matriz[i][j].g = (valoresG[count/2] + valoresG[count/2+1]) / 2;
				matriz[i][j].b = (valoresB[count/2] + valoresB[count/2+1]) / 2;
			}
			else {
				matriz[i][j].r = valoresR[count/2];
				matriz[i][j].g = valoresG[count/2];
				matriz[i][j].b = valoresB[count/2];
			}
					
			free(valoresR);
			free(valoresG);
			free(valoresB);
		}
	}
}

void fundoBranco(RGB **matriz, RGB **bw, int largura, int altura) {
	int i, j;
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (bw[i][j].r == 255 && bw[i][j].g == 255 && bw[i][j].b == 255) {
				matriz[i][j].r = matriz[i][j].g = matriz[i][j].b = 255;
			}
		}
	}
}

void limpaHSV(HSV **matriz, int altura) {
	int i;
	for (i = 0; i < altura; i++) {
		free(matriz[i]);
	}
	
	free(matriz);
}

HSV **rgb2hsv(RGB **matriz, int altura, int largura) {
	int i, j;
	double delta, min;
	double r, g, b;
	double h = 0, s, v;
	
	HSV **conversao = (HSV **)malloc(sizeof(HSV *)*altura);

	for (i=0; i < altura ; i++){
		conversao[i] = (HSV *)malloc(sizeof(HSV)*largura);
	}
	
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			r = matriz[i][j].r / 255.0;
			g = matriz[i][j].g / 255.0;
			b = matriz[i][j].b / 255.0;
			
			
			min = Min(Min(r, g), b);
			v = Max(Max(r, g), b);
			delta = v - min;
		
			if (v == 0.0)
				s = 0;
			else
				s = delta / v;
		
			if (s == 0)
				h = 0.0;
		
			else {
				if (r == v)
					h = (g - b) / delta;
				else if (g == v)
					h = 2 + (b - r) / delta;
				else if (b == v)
					h = 4 + (r - g) / delta;
		
				h *= 60;
		
				if (h < 0.0) {
					h = h + 360;
				}
			}
						
			conversao[i][j].h = h;
			conversao[i][j].s = s;
			conversao[i][j].v = v;
		}
	}
	return conversao;
}

RGB **hsv2rgb(HSV **matriz, int altura, int largura) {
	int i, j;
	
	double r = 0, g = 0, b = 0;
	
	RGB **conversao = (RGB **)malloc(sizeof(RGB *)*altura);

	for (i=0; i < altura ; i++){
		conversao[i] = (RGB *)malloc(sizeof(RGB)*largura);
	}
	
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].s == 0) {
				r = matriz[i][j].v;
				g = matriz[i][j].v;
				b = matriz[i][j].v;
			}
			else
			{
				int k;
				double f, p, q, t;
		
				if (matriz[i][j].h == 360)
					matriz[i][j].h = 0;
				else
					matriz[i][j].h = matriz[i][j].h / 60;
		
				k = (long) matriz[i][j].h;
				f = matriz[i][j].h - k;
		
				p = matriz[i][j].v * (1.0 - matriz[i][j].s);
				q = matriz[i][j].v * (1.0 - (matriz[i][j].s * f));
				t = matriz[i][j].v * (1.0 - (matriz[i][j].s * (1.0 - f)));
		
				switch (k)
				{
				case 0:
					r = matriz[i][j].v;
					g = t;
					b = p;
					break;
		
				case 1:
					r = q;
					g = matriz[i][j].v;
					b = p;
					break;
		
				case 2:
					r = p;
					g = matriz[i][j].v;
					b = t;
					break;
		
				case 3:
					r = p;
					g = q;
					b = matriz[i][j].v;
					break;
		
				case 4:
					r = t;
					g = p;
					b = matriz[i][j].v;
					break;
		
				default:
					r = matriz[i][j].v;
					g = p;
					b = q;
					break;
				}
			}
			conversao[i][j].r = (int) ceil(r * 255) % 256;
			conversao[i][j].g = (int) ceil(g * 255) % 256;
			conversao[i][j].b = (int) ceil(b * 255) % 256;
		}
	}
	return conversao;
}

void removeCores(HSV **matriz, int altura, int largura, int inicio, int fim) {
	int i, j;
	
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].h >= inicio && matriz[i][j].h <= fim) {
				matriz[i][j].h = 0;
				matriz[i][j].s = 1;
				matriz[i][j].v = 1;
			}
		}
	}
}

double calculaPorcentagem(RGB **matriz, int altura, int largura) {
	int i, j;
	int folha = 0;
	int mancha = 0;
	
	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].r != 255 || matriz[i][j].g != 255 || matriz[i][j].b != 255) {
				if (matriz[i][j].r != 255 || matriz[i][j].g != 0 && matriz[i][j].b != 0) {
					mancha++;
				}
				
				folha++;
			}
		}
	}
	
	printf("%d\n", folha);
	printf("%d\n", mancha);
	
	return (double) mancha / (double) folha * 100;
}
