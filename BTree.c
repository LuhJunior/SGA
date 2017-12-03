#include <stdlib.h>
#include <stdio.h>
#include "Btree.h"


BTree criarNo(){
    BTree aux = (BTree)malloc(sizeof(NoArv));
    int i=0;

    for (i=0; i<MAXFILHOS; i++){
        aux->filhos[i]=NULL;
    }

    aux->preenchidos=0;

    return aux;
}

void carregaBTree(char nome[], BTree *raiz, SEGMENT area){
    FILE *fp;
    indice buffer;
    fp = fopen(nome, "r+b");
    if (fp==NULL){
        printf("Erro ao abrir o BD");
        exit(1);
    }
    fseek(fp, area.BASE, SEEK_SET);
    while(ftell(fp)<=area.LIMIT+area.BASE){
        fread(&buffer, sizeof(indice), 1, fp);
        Insere(*raiz, buffer);
    }
    fclose(fp);
}

int buscaBin(indice vetor[], int preenchidos, int chave){
    int inicio;
    int fim;
    int metade;

    inicio = 0;
    fim = preenchidos-1;

    while (inicio<=fim){
        metade = (inicio+fim)/2;

        if (vetor[metade].n==chave){
            return metade;
        }else if(vetor[metade].n>chave){
            fim = metade-1;
        }
        else{
            inicio = metade+1;
        }
    }
    return inicio;
}

/*
int insereChave(BTree no, indice chave, BTree filhodir){
    int posIdeal, fim;

    posIdeal = buscaBin(no->chave, no->preenchidos, chave);
    fim = no->preenchidos-1;

    if(no->preenchidos<CHEIA){
        while(fim>posIdeal){
            no->chave[fim+1]=no->chave[fim];
            fim--;
        }
        no->chave[posIdeal]=chave;
        no->preenchidos++;
        no->filhos[pos+1] = filhodir;
        return 1;
    }
    printf("No cheio");
    return 0;
}*/


void  insereChave(BTree raiz, indice info, BTree filhodir)
{
  int k, pos;

  //busca para obter a posi��o ideal para inserir a nova chave
  pos = buscaBin(raiz->chave, raiz->preenchidos, info.n);
  k = raiz->preenchidos;

  //realiza o remanejamento para manter as chaves ordenadas
  while (k > pos && info.n < raiz->chave[k-1].n)
  {
    raiz->chave[k].n = raiz->chave[k-1].n;
    raiz->filhos[k+1] = raiz->filhos[k];
    k--;
  }
  //insere a chave na posi��o ideal
  raiz->chave[pos].n = info.n;
  raiz->filhos[pos+1] = filhodir;
  raiz->preenchidos++;
}


BTree Insercao(BTree raiz, indice chave, bool *flag, indice *retorno)
{
  indice info_mediano; //auxiliar para armazenar a chave que ir� subir para o pai
  int i, pos;
      BTree temp, filho_dir; //ponteiro para o filho � direita da chave

  if (raiz == NULL)
   {
     //O n� anterior � o ideal para inserir a nova chave (chegou em um n� folha)
     *flag = true;
     *retorno = chave;
     return(NULL);
   }
  else {
         pos = buscaBin(raiz->chave, raiz->preenchidos, chave.n);
         if (raiz->preenchidos > pos && raiz->chave[pos].n == chave.n)
           {
             printf("Chave j� contida na �rvore");
             *flag = false;
           }
         else {
                               //desce na �rvore at� encontrar o n� folha para inserir a chave.
               filho_dir = Insercao(raiz->filhos[pos],chave,flag,retorno);
               if (*flag) //Se true deve inserir a info_retorno no n�.
                 {
                     if (raiz->preenchidos < CHEIA) //Tem espa�o na p�gina
                     {
                       insereChave(raiz, *retorno, filho_dir);
                       *flag = false;
                     }
                   else {
                          temp = criarNo();
                         info_mediano.n = raiz->chave[MIN_OCUP].n;

                         //insere metade do n� raiz no temp (efetua subdivis�o)
                         temp->filhos[0] = raiz->filhos[MIN_OCUP+1];
                         for (i = MIN_OCUP + 1; i < CHEIA; i++)
                           insereChave(temp, raiz->chave[i], filho_dir);

                         //atualiza n� raiz.
                         for (i = MIN_OCUP; i<CHEIA; i++)
                         {
                           raiz->chave[i].n = 0;
                           raiz->filhos[i+1] = NULL;
                         }
                         raiz->preenchidos = MIN_OCUP;

                         //Verifica em qual n� ser� inserida a nova chave
                         if (pos <= MIN_OCUP)
                              insereChave(raiz, *retorno, filho_dir);
                         else insereChave(temp, *retorno, filho_dir);

                         //retorna o mediano para inser�-lo no n� pai e o temp como filho direito do mediano.
                         *retorno = info_mediano;
                         return(temp);
                       }
                 }
              }
     }
}

BTree Insere(BTree no, indice v)
{
  bool flag;
  int  i;
  indice retorno;
  BTree filho_dir, buffer;

  filho_dir = Insercao(no, v,&flag,&retorno);
  if (flag)
   { //Aumetar� a altura da �rvore
     buffer = criarNo();
     buffer->preenchidos = 1;
     buffer->chave[0].n = retorno.n;
     buffer->filhos[0] = no;
     buffer->filhos[1] = filho_dir;
     for (i = 2; i <= CHEIA; i++)
       buffer->filhos[i] = NULL;
     return(buffer);
   }
  else return(no);
}

void em_ordem(BTree raiz)
{
  int i;

  if (raiz != NULL)
   {
     for (i = 0; i < raiz->preenchidos; i++)
      {
        em_ordem(raiz->filhos[i]);
        printf("\n%d", raiz->chave[i].n);
      }
     em_ordem(raiz->filhos[i]);
      }
}
