#ifndef _IMAGEM_H_
#define _IMAGEM_H_

#pragma pack(1)

/*-------------------------*/
struct cabecalho_arquivo{
	unsigned short tipo;
	unsigned int tamanhoArquivo;
	unsigned short reservado1;
	unsigned short reservado2;
	unsigned int offset;
};
typedef struct cabecalho_arquivo CABECALHO_ARQUIVO;

/*--------------------------------*/
struct cabecalho_imagem {
	unsigned int tamanhoCabecalho;
	unsigned int larguraImagem;
	unsigned int alturaImagem;
	unsigned short planos;
	unsigned short nbits;
	unsigned int compressao;
	unsigned int tamanhoImagem;
	unsigned int resolucaoX;
	unsigned int resolucaoY;
	//sao usadas B G R
	//estrutura que consiga representar 8bits 8bits 8bits, tipo byte = char
	unsigned int coresUsadas;
	unsigned int coresImp;
};
typedef struct cabecalho_imagem CABECALHO_IMAGEM;

/*-----------------------------------*/
struct rgb {
	//para ser tons de cinza todos o rgb tem que ser iguais
	//para trasnformar em cinza fazer media do rgb e substituir os 3 pela media
	unsigned char b;
	unsigned char g;
	unsigned char r;
};
typedef struct rgb RGB;

void limpaMatriz(RGB **matriz, int altura);
void copiaMatriz(RGB **matriz, RGB **copia, int altura, int largura);
RGB **criarMatriz (int altura, int largura);
void geraImagem(RGB **matriz, CABECALHO_ARQUIVO cabecalho_a, CABECALHO_IMAGEM cabecalho_i, char *nomeArquivo, char tipoImagem[]);
void abreImagens(char **vetor, int pos);

#endif
