#include <stdio.h>
#include <stdlib.h>
#define MAX_RESOURCES 20
#define MAX_PROCESS 20
int leituracustomer(FILE *file);
int leituracommand(FILE *file,FILE *commands_txt,int available, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[])

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No resourses.\n");
        return 1;
    }

    int resources = argc - 1;
    int i,j;
    int available[MAX_RESOURCES];
    char commands[1024];
    for (int i = 0; i < resources; i++) {
        available[i] = atoi(argv[i + 1]);
    }
    int allocation[MAX_PROCESS][MAX_RESOURCES];
    for(i=0;i<MAX_PROCESS;i++){
        for(j=0;j<resources;j++)
        scanf("%d",&allocation[i][j]);
    }/* A matriz de alocação é uma tabela que rerpesenta a alocação de recursos para cada processo em um sistema. Ele ajuda o Banker a tomar decisões sobre a segurança das alocações de recursos*/
    //incerta se ta correto essa forma de implementação. CHECAR O CLASS DE ERICO E A IMPLEMENTAÇÃO COMO ESTÁ NO SILBERCHATZ
    int numProcesses;
    FILE *file;
    leituracustomer(file);
    int max[MAX_PROCESS][MAX_RESOURCES];
    for (i = 0; i < numProcesses; i++) {
        for (j = 0; j < resources; j++) {
            fscanf(file,"%d,", &max[i][j]);
        }
    }
     for(i=0;i<available;i++){
        for(j=0;j<resources;j++)
        scanf("%d",&max[i][j]);
     }


    return 0;
}
int leituracustomer(FILE *file) {
    file = fopen("customer.txt", "r");
    if (file == NULL) {
        printf("FAILURE ON READING FILE");
        return 1;
    }

    char linha[70];
    int numProcesses = 0;

    while (fgets(linha, sizeof(linha), file) != NULL) {
        printf("%s", linha);
        numProcesses++;
    }

    fclose(file);
    return numProcesses;
}
int leituracommand(FILE *file,FILE *commands_txt,int available, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[]){
    file= fopen("commands.txt","r"); 
    if (file == NULL) {
        printf("FAILURE ON READING FILE");
        return 1;
    }
    char command[3]; // Para armazenar RQ, RL ou *
    int processIndex, req1, req2, req3;

    while (fscanf(file, "%s", command) == 1) {
        if (strcmp(command,     "RQ") == 0) {
            // Comando RQ encontrado, faça algo
            int customer;
            fscanf(file, "%d",customer);
            // Adicione aqui a lógica para processar RQ
        } else if (strcmp(command, "RL") == 0) {
            // Comando RL encontrado, faça algo
            fscanf(file, "%d %d %d %d", &processIndex, &req1, &req2, &req3);
            printf("Comando RL encontrado: Processo %d, Recursos %d %d %d\n", processIndex, req1, req2, req3);
            // Adicione aqui a lógica para processar RL
        } else if (strcmp(command, "*") == 0) {
            // Comando * encontrado, faça algo
            printf("Comando * encontrado\n");
            // Adicione aqui a lógica para processar *
        } else {
            printf("Comando desconhecido: %s\n", command);
        }
    }
}