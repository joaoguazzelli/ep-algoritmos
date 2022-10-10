#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char TipoDaFila;
    int fila;
    int ini;
    int fim;
}bloco;

typedef struct{
    bloco *v;
    int topo;
    int max;
}pilha;

void criaPilha(pilha*, int);
int pilhaVazia(pilha*);
void empilha(bloco, pilha*);
bloco desempilha(pilha*);

int** geraTabuleiro(int, int);
char** alocaPalavras(int, int);

char** encaixaPalavras(char**, int, int**, int, int);
int cabe(char*, char**, bloco);
void encaixa(char*, char**, int**, bloco);
void desfazEncaixe(int**, char**, bloco);

void imprimeTabela(char**, int, int);
int max(int, int);

int main(){
    int i, linha, coluna, p, tam, **tabela, contador;
    char **palavras, **solucao;

    printf("EP2 - Inventando palavras cruzadas:\n");
    printf("\nInsira os dados:\n\n");

    if(scanf("%d %d", &linha, &coluna)){};
    solucao = 0;
    contador = 1;

    while((linha != 0) && (coluna != 0)){
        tabela = geraTabuleiro(linha, coluna);
        if(scanf("%d", &p)){};
        tam = max(linha, coluna);
        palavras = alocaPalavras(p, tam);

        printf("\nInstancia %d\n", contador);

        if(palavras)
            solucao = encaixaPalavras(palavras, p, tabela, linha, coluna);

        if(solucao)
            imprimeTabela(solucao, linha, coluna);
        else
            printf("Nao ha solucao!\n");

        printf("\n\nInsira os dados:\n\n");
        if(scanf("%d %d", &linha, &coluna)){};

        contador++;

        for(i = 0; i < linha; i++){
            free(tabela[i]);
            free(solucao[i]);
        }
        free(tabela);
        free(solucao);
        for(i = 0; i < p; i++)
            free(palavras[i]);
        free(palavras);
    }

    return 0;
}


void criaPilha(pilha *P, int tam){
    int i;
    P->v = (bloco*)malloc(tam*sizeof(bloco));
    for(i = 0; i < tam; i++){
        P->v[i].fila = 0;
        P->v[i].ini = 0;
        P->v[i].fim = 0;
        P->v[i].TipoDaFila = 0;
    }
    P->topo = 0;
    P->max = tam;
}


int pilhaVazia(pilha *P){
    if(P->topo == 0)
        return 1;
    return 0;
}


void empilha(bloco x, pilha *P){
    if(P->topo == P->max){
        printf("Pilha cheia");
        return;
    }
    P->v[P->topo] = x;
    P->topo++;
}


bloco desempilha(pilha *P){
    if(pilhaVazia(P)){
        printf("Pilha vazia");
    }
    else{
        P->topo--;
        return(P->v[P->topo]);
    }
}


int** geraTabuleiro(int linha, int coluna){
    int **tabela, i, j;

    tabela = (int**)malloc(linha * sizeof(int*));
    for(i = 0; i < linha; i++){
        tabela[i] = (int*)malloc(coluna * sizeof(int));
    }
    for(i = 0; i < linha; i++){
        for(j = 0; j < coluna; j++){
            if(scanf("%d", &tabela[i][j])){};
        }
    }
    return tabela;
}


char** alocaPalavras(int pal, int letra){
    int i, j, ok;
    char **Palavras;
    char *palavra;
    ok = 1;

    palavra = (char*)malloc((letra + 2) * sizeof(char));
    for(i = 0; i < (letra + 2); i++) palavra[i] = 0;

    Palavras = (char**)malloc(pal*sizeof(char*));
    for(i = 0; i < pal; i++){
        Palavras[i] = (char*)malloc((letra + 1) * sizeof(char));
        for(j = 0; j < (letra + 1); j++)
            Palavras[i][j] = 0;
    }
    for(i = 0; i < pal; i++){
        if(scanf("%s", palavra)){};
        for(j = 0; palavra[j] != 0; j++){
            if(j > (letra - 1)){
                free(palavra);
                ok = 0;
            }
            Palavras[i][j] = palavra[j];
        }
    }
    if(!ok) return 0;

    free(palavra);
    return Palavras;
}


char** encaixaPalavras(char** palavras, int numPal, int** tabela, int linha, int coluna){
    char **solucao;
    int p, i, j, ok;
    bloco atual;
    pilha blocos;
    criaPilha(&blocos, numPal);
    solucao = (char**)malloc(linha * sizeof(char*));
    for(i = 0; i < linha; i++){
        solucao[i] = (char*)malloc(coluna * sizeof(char));
        for(j = 0; j < coluna; j++)
            solucao[i][j] = 0;
    }
    atual.fim = 0;
    atual.fila = 0;
    atual.TipoDaFila = 'l';
    p = 0;
    while(p < numPal){
        ok = 0;
        if(atual.TipoDaFila == 'l'){
            for(i = atual.fila; i < linha; i++){
                atual.fila = i;
                j = atual.fim;
                while(j < coluna){
                    while((j < coluna) && (tabela[i][j] == (-1)))
                        j++;
                    atual.ini = j;
                    while((j < coluna) && (tabela[i][j] != (-1)))
                        j++;
                    atual.fim = j;
                    if(cabe(palavras[p], solucao, atual)){
                        encaixa(palavras[p], solucao, tabela, atual);
                        ok = 1;
                        break;
                    }
                    else
                        j++;
                }
                if(ok) break;
                else atual.fim = 0;
            }
            if(!ok){
                atual.TipoDaFila = 'c';
                atual.fila = 0;
            }
        }
        if(atual.TipoDaFila == 'c'){
            for(j = atual.fila; j < coluna; j++){
                atual.fila = j;
                i = atual.fim;
                while(i < linha){
                    while((i < linha) && (tabela[i][j] == (-1)))
                        i++;
                    atual.ini = i;
                    while((i < linha) && (tabela[i][j] != (-1)))
                        i++;
                    atual.fim = i;
                    if(cabe(palavras[p], solucao, atual)){
                        encaixa(palavras[p], solucao, tabela, atual);
                        ok = 1;
                        break;
                    }
                    else
                        i++;
                }
            if(ok) break;
            else atual.fim = 0;
            }
        }
        if(ok){
            empilha(atual, &blocos);
            p++;
            atual.TipoDaFila = 'l';
            atual.fila = 0;
            atual.fim = 0;
        }
        else{
            if(pilhaVazia(&blocos))
                return 0;
            atual = desempilha(&blocos);
            desfazEncaixe(tabela, solucao, atual);
            p--;
        }
    }
    for(i = 0; i < linha; i++)
        for(j = 0; j < coluna; j++)
            if(solucao[i][j] == 0)
                solucao[i][j] = '*';
    return solucao;
}


int cabe(char *palavra, char **solucao, bloco b){
    int k, tam_palavra, tam_bloco, ok;
    ok = 0;
    tam_bloco = b.fim - b.ini;
    for(tam_palavra = 0; palavra[tam_palavra] != 0; tam_palavra++);
    if(tam_bloco == tam_palavra)
        ok = 1;
    if(b.TipoDaFila == 'l'){
        if(ok){
            for(k = 0; k < tam_bloco; k++){
                if(solucao[b.fila][k + b.ini] != 0){
                    if((solucao[b.fila][k + b.ini]) != (palavra[k])){
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    else if(b.TipoDaFila == 'c'){
        if(ok){
            for(k = 0; k < tam_bloco; k++){
                if(solucao[k + b.ini][b.fila] != 0){
                    if((solucao[k + b.ini][b.fila]) != (palavra[k])){
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}


void encaixa(char *palavra, char **solucao, int **tab, bloco b){
    int k, tam_bloco;
    tam_bloco = b.fim - b.ini;
    if(b.TipoDaFila == 'l'){
        for(k = 0; k < tam_bloco; k++){
            solucao[b.fila][b.ini + k] = palavra[k];
        }
        for(k = 0; k < tam_bloco; k++){
            tab[b.fila][b.ini + k] += 1;
        }
    }
    if(b.TipoDaFila == 'c'){
        for(k = 0; k < tam_bloco; k++){
        solucao[k + b.ini][b.fila] = palavra[k];
        }
        for(k = 0; k < tam_bloco; k++){
            tab[k + b.ini][b.fila] += 1;
        }
    }
}


void desfazEncaixe(int **tab, char **solucao, bloco b){
    int k;
    if(b.TipoDaFila == 'l'){
        for(k = b.ini; k < b.fim; k++)
            if(tab[b.fila][k] == 1)
                solucao[b.fila][k] = 0;
        for(k = b.ini; k < b.fim; k++)
            tab[b.fila][k] -= 1;
    }
    else if(b.TipoDaFila == 'c'){
        for(k = b.ini; k < b.fim; k++)
            if(tab[k][b.fila] == 1)
                solucao[k][b.fila] = 0;
        for(k = b.ini; k < b.fim; k++)
            tab[k][b.fila] -= 1;
    }
}


void imprimeTabela(char **M, int lin, int col){
    int i, j;

    for(i = 0; i < lin; i++){
        for(j = 0; j < col; j++){
            printf(" %c ", M[i][j]);
        }
        printf("\n");
    }
}


int max(int num1, int num2){
    if(num2 > num1)
        return num2;
    else
        return num1;
}