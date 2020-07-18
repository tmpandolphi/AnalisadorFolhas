#ifndef _VETOR_H_
#define _VETOR_H_

char **criarVetor(int pos);
void liberarVetor(char **vetor, int pos);
void **preencheVetor(char *arquivo, char **vetor, int pos);
void imprimeVetor(char **vetor, int pos);
int contaLinhas (char *arquivo);
void imprimeArquivo (char *arquivo);

#endif
