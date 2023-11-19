#include <stdio.h>
#include <stdlib.h>
void alocacaorecurso(alocacao,maximo,necessidade,disponibilidade);
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);

int main(int argc, char *argv[])
{
     // Verifica se foram fornecidos valores suficientes
    if (argc < 2) {
        printf("No resourses.\n");
        return 1;
    }

    // Calcula a quantidade de valores fornecidos
    int resources = argc - 1;

    // Aloca espaço para o vetor de processos
    int *process = (int *)malloc(resources * sizeof(int));

    // Preenche o vetor de valores com os argumentos fornecidos
    for (int i = 0; i < resources; i++) {
        process[i] = atoi(argv[i + 1]);
    }

    // Cria um vetor para armazenar o número de valores
    int *Resources = (int *)malloc(sizeof(int));
    *Resources = resources;

    // Imprime a quantidade de valores
    printf("Quantidade de valores: %d\n", *Resources);

    // Imprime os valores armazenados no vetor
    printf("Valores: ");
    for (int i = 0; i < resources; i++) {
        printf("%d ", process[i]);
    }
    printf("\n");

    // Libera a memória alocada para os vetores
    free(process);
    free(Resources);

    // Declare os arrays com tamanhos apropriados
    /*int available[num_processos][num_recursos];
    int maximo[num_processos][num_recursos];
    int necessidade[num_processos][num_recursos];
    int disponibilidade[num_recursos];*/

    // Resto do seu código...
    return 0;
}