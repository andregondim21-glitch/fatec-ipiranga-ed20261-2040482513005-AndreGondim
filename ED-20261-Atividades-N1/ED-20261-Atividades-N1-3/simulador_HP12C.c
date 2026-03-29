/*--------------------------------------------------*/
/* FATEC-Ipiranga                                   */ 
/* ADS - Estrutura de Dados                         */
/* Id da Atividade: N1-3                            */
/* Objetivo: Estrutura de Dados de Pilha            */
/*                                                  */
/* Autor: Prof. Verissimo                           */
/* Data: 24/03/2026                                 */
/*--------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Define o numero maximo de elementos na pilha
#define MAX 4
//Estruturando a struct da pilha
typedef struct {
    double stack[MAX];
    int top;
} HP12C;
//Função para inicializar a pilha
void init(HP12C *hp) {
    hp->top = -1;
}
//Função para mostrar o conteúdo da pilha
void mostrar(HP12C *hp) {
    printf("T: %.2f | Z: %.2f | Y: %.2f | X: %.2f\n",
           hp->stack[3], hp->stack[2], hp->stack[1], hp->stack[0]);
}
//Função para verificar se a entrada é um número
int ehNumero(char *parte) {
    if (*parte == '-' && *(parte + 1) != '\0') parte++;

    while (*parte) {
        if (!isdigit(*parte) && *parte != '.')
            return 0;
        parte++;
    }
    return 1;
}
//Função para verificar se a pilha está cheia
int isFull(HP12C *hp) {
    return hp->top == MAX - 1;
}
//Função para verificar se a pilha está vazia
int isEmpty(HP12C *hp) {
    return hp->top == -1;
}
//Função para empilhar um valor na pilha
void push(HP12C *hp, double value) {
    if (isFull(hp)) {
        printf("Pilha cheia! Não é possível empilhar.\n");
        return;
    }
    hp->stack[++hp->top] = value;
}
//Função para desempilhar um valor da pilha
double pop(HP12C *hp) {
    if (isEmpty(hp)) {
        printf("Pilha vazia! Não é possível desempilhar.\n");
        return 0.0; // Retorna um valor padrão
    }
    return hp->stack[hp->top--];
}
//Função para realizar operações matemáticas
void operar(HP12C *hp, char op) {
    double x = pop(hp);
    double y = pop(hp);
    double resultado;

    switch(op) {
        case '+': resultado = y + x; break;
        case '-': resultado = y - x; break;
        case '*': resultado = y * x; break;
        case '/':
            if (x == 0) {
                printf("Erro: divisão por zero!\n");
                return;
            }
            resultado = y / x;
            break;
        default:
            printf("Operador inválido!\n");
            return;
    }

    printf("Operação: %.2f %c %.2f = %.2f\n", y, op, x, resultado);

    // Resultado volta para a pilha
    push(hp, resultado);
}

int main() {
    HP12C hp;
    init(&hp);
    int i=0;

    char entrada[200];
    //Pede ao usuario para digitar a conta em RPN
    printf("Digite a expressao em RPN:\n");
    fgets(entrada, sizeof(entrada), stdin);

    //Remove ENTER
    entrada[strcspn(entrada, "\n")] = '\0';

    //Divide em partes
    char *parte = strtok(entrada, " ");

    //Processa cada parte da entrada
    while (parte != NULL) {
        if(i == 4) {
            printf("Pilha cheia! Operação não pode ser realizada.\n");
            parte = NULL;        
        }
        else{
            printf("\nEntrada: %s\n", parte);

            // Número
            if (ehNumero(parte)) {
                double valor = atof(parte);
                push(&hp, valor);
                i++;
            }
            // Operador
            else if (strlen(parte) == 1) {
                operar(&hp, parte[0]);
                i = 0;
            }
            else {
                printf("Entrada inválida: %s\n", parte);
                return 1;
            }

            mostrar(&hp);

            parte = strtok(NULL, " ");
            
            printf("\nResultado final: %.2f\n", hp.stack[0]);
        }
    }
    return 0;
}