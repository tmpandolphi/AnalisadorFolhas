#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagem.h"
#include "vetor.h"
#include "filtros.h"

//escalas de cinza - FUNCIONANDO
int pretoBranco(RGB pixel){
  int media;

  media = (pixel.r + pixel.g + pixel.b) / 3;

  return media;
}

//copiar a matriz
void copiaMatriz(RGB **matriz, RGB **copia, int altura, int largura) {
	int i, j;
	//RGB **copia = (RGB **)malloc(sizeof(RGB *)*altura);

	//for (i=0;i<altura;i++){
	//	copia[i] = (RGB *)malloc(sizeof(RGB)*largura);
	//}
	
	for (i=0 ; i<altura; i++) {
		for (j=0 ; j<largura ; j++) {
			copia[i][j].r = matriz[i][j].r;
			copia[i][j].g = matriz[i][j].g;
			copia[i][j].b = matriz[i][j].b;
		}
	}
}

//limpa matriz
void limpaMatriz(RGB **matriz, int altura) {
	int i;
	for (i = 0; i < altura; i++) {
		free(matriz[i]);
	}
	
	free(matriz);
}

//Criar matriz - FUNCIONANDO
RGB **criarMatriz (int altura, int largura){
	int i;

	RGB **matriz = (RGB **)malloc(sizeof(RGB *)*altura);

	for (i=0;i<altura;i++){
		matriz[i] = (RGB *)malloc(sizeof(RGB)*largura);
	}

	return matriz;
}

//preencher matriz - FUNCIONANDO
void preencheMatriz(RGB **matriz, int offset, int altura, int largura, FILE *f){
	int i, j, k;
	unsigned char byte;
	
	fseek(f, offset, SEEK_SET);

	for (i=0 ; i<altura; i++) {
		for (j=0 ; j<largura ; j++) {
			fread(&matriz[i][j], sizeof(RGB), 1, f);
		}
		int alinhamento = (largura*3)%4;
		if(alinhamento != 0){
			for(k=0;k<4-alinhamento;k++){
				fread(&byte, sizeof(byte), 1, f);
			}
		}
	}
}

//gera as imagens
void geraImagem(RGB **matriz, CABECALHO_ARQUIVO cabecalho_a, CABECALHO_IMAGEM cabecalho_i, char *nomeArquivo, char *tipo) {
	FILE  *f = NULL;
	int i, j, k;
	unsigned char byte;
	
	strcat(tipo, nomeArquivo);
	
	f = fopen(tipo, "wb");

	fwrite(&cabecalho_a, sizeof(CABECALHO_ARQUIVO), 1, f);
	fwrite(&cabecalho_i, sizeof(CABECALHO_IMAGEM), 1, f);
	
	fseek(f, cabecalho_a.offset, SEEK_SET);
	
	for (i=0 ; i<cabecalho_i.alturaImagem; i++) {
		for (j=0 ; j<cabecalho_i.larguraImagem ; j++) {
			fwrite(&matriz[i][j], sizeof(RGB), 1, f);
		}
		int alinhamento = (cabecalho_i.larguraImagem*3)%4;
		if(alinhamento != 0){
			for(k=0;k<4-alinhamento;k++){
				fwrite(&byte, sizeof(byte), 1, f);
			}
		}
	}
	
	fclose(f);
}

//abre as imagens
void abreImagens(char **vetor, int pos){
	int k, i, j;
	char nomeArquivo[100];
	char str[10];
	CABECALHO_ARQUIVO cabecalho_arquivo;
  	CABECALHO_IMAGEM cabecalho_imagem;
	
	FILE *f = NULL;
		
	for(k=0;k<pos;k++){
		
		strcpy(nomeArquivo, vetor[k]);
	
		f = fopen(nomeArquivo, "rb"); //usar tipo de operação mais b para abrir binario

		if (f == NULL){
			printf("Erro ao abrir arquivo %s\n", nomeArquivo );
			exit(0);
		}
		
		fread(&cabecalho_arquivo, sizeof(CABECALHO_ARQUIVO), 1, f);
  		fread(&cabecalho_imagem, sizeof(CABECALHO_IMAGEM), 1, f);


  		//criar a matriz
  		RGB **matriz = criarMatriz(cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);
  		RGB **matriz2 = criarMatriz(cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);
		
  		//preencheMatriz
  		preencheMatriz(matriz, cabecalho_arquivo.offset, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem, f);

		copiaMatriz(matriz, matriz2, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);
		  		
		mediana(matriz, cabecalho_imagem.larguraImagem, cabecalho_imagem.alturaImagem, 5);
		
		strcpy(str, "Mediana_");
		geraImagem(matriz, cabecalho_arquivo, cabecalho_imagem, nomeArquivo, str);

		grayscale(matriz, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);	
						  	
		strcpy(str, "Gray_");	
		geraImagem(matriz, cabecalho_arquivo, cabecalho_imagem, nomeArquivo, str);
		
		int * histograma = (int *) geraHistograma(matriz, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);
		//escreveHistograma(histograma, 256);
		int limiar = calculaLimiar(histograma, 256);
		
		bw(matriz, cabecalho_imagem, limiar);
		  
		strcpy(str, "BW_");		
		geraImagem(matriz, cabecalho_arquivo, cabecalho_imagem, nomeArquivo, str);
		  		
		fundoBranco(matriz2, matriz, cabecalho_imagem.larguraImagem, cabecalho_imagem.alturaImagem);
		  		
		strcpy(str, "WBG_");	
		geraImagem(matriz, cabecalho_arquivo, cabecalho_imagem, nomeArquivo, str);
		  		
		HSV **matriz3 = rgb2hsv(matriz2, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);
		removeCores(matriz3, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem, 38, 360);
		RGB **matriz4 = hsv2rgb(matriz3, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem);

		printf("%lf\n", calculaPorcentagem(matriz4, cabecalho_imagem.alturaImagem, cabecalho_imagem.larguraImagem));
		
		strcpy(str, "HSV_");
		geraImagem(matriz4, cabecalho_arquivo, cabecalho_imagem, nomeArquivo, str);
		
		printf("Imagem %s processada.\n", nomeArquivo);
	
		limpaMatriz(matriz, cabecalho_imagem.alturaImagem);
		limpaMatriz(matriz2, cabecalho_imagem.alturaImagem);
		limpaHSV(matriz3, cabecalho_imagem.alturaImagem);
		limpaMatriz(matriz4, cabecalho_imagem.alturaImagem);
		
  		fclose(f);
	}	
}

