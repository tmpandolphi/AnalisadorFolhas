#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetor.h"
#include "imagem.h"
#include "filtros.h"

int main(){

  char *arquivoTexto = "imagens.txt";
  int numLinhas = contaLinhas(arquivoTexto);
  char **vetor = criarVetor(numLinhas);

  preencheVetor(arquivoTexto, vetor, numLinhas);
  imprimeVetor(vetor, numLinhas);

  abreImagens(vetor, numLinhas);
  liberarVetor(vetor, numLinhas);
  
  system("pause");
}
