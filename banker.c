#include <stdio.h>
#include <stdlib.h>
int leituracustomer(FILE *file);
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No resourses.\n");
        return 1;
    }

    int resources = argc - 1;
    int i,j;
    int *instancia = (int *)malloc(resources * sizeof(int));

    for (int i = 0; i < resources; i++) {
        instancia[i] = atoi(argv[i + 1]);
    }

    int *Resources = (int *)malloc(sizeof(int));
    *Resources = resources;
    int linha=20;
    int coluna=20;
    int **alocacao=(int **)malloc(resources*sizeof(int));
    for(i=0;i<linha;i++)
        alocacao[i]=(int *)malloc(instancia[i]*sizeof(int));
    if (alocacao==NULL){
        printf("Erro na alocacao de memoria");
        return 1;
    }
    /*PRECISO FAZER O REALLOC DEPOIS*/
    for(i=0;i<instancia;i++){
        for(j=0;j<Resources;j++)
        scanf("%d",&alocacao[i][j]);
    }/* A matriz de alocação é uma tabela que rerpesenta a alocação de recursos para cada processo em um sistema. Ele ajuda o Banker a tomar decisões sobre a segurança das alocações de recursos*/
    //incerta se ta correto essa forma de implementação. CHECAR O CLASS DE ERICO E A IMPLEMENTAÇÃO COMO ESTÁ NO SILBERCHATZ
    int **max=(int **)malloc(resources*sizeof(int));
    for(i=0;i<linha;i++)
        max[i]=(int *)malloc(instancia[i]*sizeof(int));
    if (max==NULL){
        printf("Erro no recebimento do MAX de recursos");
        return 1;
    }
     for(i=0;i<instancia;i++){
        for(j=0;j<Resources;j++)
        scanf("%d",&max[i][j]);
     }
    FILE *file;
    leituracustomer(file);

   
    free(instancia);
    free(Resources);
    return 0;
}
int leituracustomer(FILE *file){
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
}