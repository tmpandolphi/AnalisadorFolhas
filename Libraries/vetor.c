#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vetor.h"
#include "filtros.h"
#include "imagem.h"


/*Cria vetor com tamanho de string pré definido - FUNCIONANDO.*/
char **criarVetor(int pos){
  int i;
  char **vetor = (char **) malloc(pos * sizeof(char *));

  for (i = 0; i < pos; ++i){
    vetor[i] = (char *)malloc(8 * sizeof(char *));
  }

  return(vetor);
}

/*Libera as posições do vetor uma por uma*/
void liberarVetor(char **vetor, int pos){
  int i;

  for(i=0;i<pos;i++){
    free(vetor[i]);
  }
}

/*Coloca uma linha do arquivo texto por posição do vetor - FUNCIONANDO.*/
void **preencheVetor(char *arquivo, char **vetor, int pos){
    FILE *f = NULL;
    char nomes[50];
    int i;

    f = fopen(arquivo,"r");

    if(f == NULL) {
      printf("Erro ao abrir o arquivo %s\n", arquivo);
      exit (0);
    } else {
          for(i=0;i<pos;i++) {
            fscanf(f,"%s\n", nomes);
            strcpy(vetor[i], nomes);
          }
            /*if(i<pos-1){
              vetor[i][strlen(vetor[i])-1]='\0';
            }*/
      }

    fclose(f);
}

/*Escreve o vetor - FUNCIONANDO.*/
void imprimeVetor(char **vetor, int pos){
  int i = 0;

  for(i=0;i<pos;i++){
    printf("%s\n", vetor[i]);
  }
}

/*Conta a quantidade de linhas do arquivo texto - FUNCIONANDO.*/
int contaLinhas (char *arquivo){
  FILE *f = NULL;
  char nomes[50];
  int cont = 0;

  f = fopen(arquivo,"r");

  if(f == NULL)  {
    printf("Erro ao abrir o arquivo %s\n", arquivo);
    exit (0);
  } else {
      while(fscanf(f,"%s\n", nomes) == 1) {
        cont++;
      }
    }

  fclose(f);
  return(cont);
}

/*Imprime arquivo texto que está aberto - FUNCIONANDO.*/
void imprimeArquivo (char *arquivo){
  FILE *f = NULL;
  char nomes[50];

  f = fopen(arquivo,"r");

  if(f == NULL) {
    printf("Erro ao abrir o arquivo %s\n", arquivo);
    exit (0);
  } else {
      while(fscanf(f,"%s\n", nomes) == 1) {
        printf("%s\n", nomes);
      }
    }
  fclose(f);
}
