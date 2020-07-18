#ifndef _FILTROS_H_
#define _FILTROS_H_

#include "imagem.h"
//#pragma pack(1)

struct hsv {
	double v;
	double s;
	double h;
};
typedef struct hsv HSV;

static double Min(double a, double b);
static double Max(double a, double b);

void limpaHSV(HSV **matriz, int altura);
RGB** grayscale(RGB **matriz, int altura, int largura);
RGB** bw(RGB **matriz, CABECALHO_IMAGEM cabecalho_i, int limiar);
int* geraHistograma(RGB **matriz, int altura, int largura);
void escreveHistograma(int *hist, int n);
int calculaLimiar(int *histograma, int indices);
void ordena(int arr[], int n);
void mediana(RGB **matriz, int largura, int altura, int tamanho);
void fundoBranco(RGB **matriz, RGB **bw, int largura, int altura);
HSV **rgb2hsv(RGB **matriz, int altura, int largura);
RGB **hsv2rgb(HSV **matriz, int altura, int largura);
void removeCores(HSV **matriz, int altura, int largura, int inicio, int fim);
double calculaPorcentagem(RGB **matriz, int altura, int largura);


#endif
