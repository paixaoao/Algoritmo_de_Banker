#include <stdio.h>
#include <stdlib.h>
void alocacaorecurso(alocacao,maximo,necessidade,disponibilidade);
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No resourses.\n");
        return 1;
    }

    int resources = argc - 1;

    int *instancia = (int *)malloc(resources * sizeof(int));

    for (int i = 0; i < resources; i++) {
        instancia[i] = atoi(argv[i + 1]);
    }

    int *Resources = (int *)malloc(sizeof(int));
    *Resources = resources;

    /*printf("Quantidade de valores: %d\n", *Resources);

    printf("Valores: ");
    for (int i = 0; i < resources; i++) {
        printf("%d ", instancia[i]);
    }
    printf("\n");*/
    FILE *file;
    file=fopen("customer.txt","r");
    if (file==NULL){
        printf("FAILURE ON READING FILE");
        return 1;
    }
    char linha[70];
     while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
    }
        fclose(file);


    // Declare os arrays com tamanhos apropriados
    //int available[instancia][Resources];
    /*int maximo[num_processos][num_recursos];
    int necessidade[num_processos][num_recursos];
    int disponibilidade[num_recursos];*/

    // Resto do seu código...
    free(instancia);
    free(Resources);
    return 0;
}