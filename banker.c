#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_RESOURCES 20
#define MAX_PROCESS 20
//ERROS A SEREM IMPLEMENTADOS:
//Incompatibility between commands.txt and command line
//Incompatibility between commands.txt and command line
int leituracommand(FILE *file,FILE *commands_txt,int available, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[]);
int impressaodados(int available, int max, int need, int allocation);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No resourses.\n");
        return 1;
    }
    FILE *arq_customer = fopen("customer.txt", "r");
    if (arq_customer == NULL) {
        printf("Fail to read customer.txt");
        return 1;
    }

    char linha[70];
    int numProcesses = 0;

    while (fgets(linha, sizeof(linha), arq_customer) != NULL) {
        //printf("%s", linha);
        numProcesses++;
    }

    int resources = argc - 1;
    int i,j;
    int available[resources];
    char commands[1024];
    for (i = 0; i < resources; i++) {
        available[i] = atoi(argv[i + 1]);
    }
    int allocation[MAX_PROCESS][MAX_RESOURCES];
    int max[MAX_PROCESS][MAX_RESOURCES];
    int need[MAX_PROCESS][MAX_RESOURCES];
    for (i = 0; i < numProcesses; i++) {
        for (j = 0; j < resources; j++) {
            fscanf(arq_customer,"%d,", &max[i][j]);
            allocation[i][j]=0;
            need[i][j]=max[i][j]-allocation[i][j]; 
        }
    }
   /* A matriz de alocação é uma tabela que rerpesenta a alocação de recursos para cada processo em um sistema. Ele ajuda o Banker a tomar decisões sobre a segurança das alocações de recursos*/
    //incerta se ta correto essa forma de implementação. CHECAR O CLASS DE ERICO E A IMPLEMENTAÇÃO COMO ESTÁ NO SILBERCHATZ

    fclose(arq_customer);
    return 0;
}
/*int impressaodados(int available, int max, int need, int allocation){
    for (int i = 0; i < 5; i++) {
        printf("%d %d %d | %d %d %d | %d %d %d\n",
               maximum[i][0], maximum[i][1], maximum[i][2],
               allocation[i][0], allocation[i][1], allocation[i][2],
               need[i][0], need[i][1], need[i][2]);
    }
}*/
int leituracommand(FILE *file,FILE *commands_txt,int available, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[]){
    file= fopen("commands.txt","r"); 
    if (file == NULL) {
        printf("Fail to read commands.txt\n");
        return 1;
    }
    char command[3]; // Para armazenar RQ, RL ou *
    int processIndex, req[MAX_RESOURCES];
     FILE *result_file = fopen("result.txt", "w");
    if (result_file == NULL) {
        printf("Fail to create result.txt\n");
        fclose(file);
        return 1;
    }
    while (fscanf(file, "%s", command) == 1) {
        if (strcmp(command,"RQ") == 0) {
            // Comando RQ encontrado, faça algo
            int customer;
            fscanf(file, "%d",customer);
            // Adicione aqui a lógica para processar RQ
            for (int i = 0; i < num_resources; i++) {
            fscanf(file, "%d", &req[i]);    
        }
        int exceedsMaxNeed;
            for (int i = 0; i < num_resources; i++) {
                if (req[i] > need[processIndex][i]) {
                    exceedsMaxNeed = 1;
                    fprintf(result_file, "The customer %d request %d was denied because it exceeds its maximum need\n", customer, req[i]);
                }
            }
            }           
         else if (strcmp(command, "RL") == 0) {
            // Comando RL encontrado, faça algo
            fscanf(file, "%d", &processIndex, &req);
            //AJEITAR ISSO
            printf("Comando RL encontrado: Processo %d, Recursos %d\n", processIndex, req);
            // Adicione aqui a lógica para processar RL
        } else if (strcmp(command, "*") == 0) {
            // Comando * encontrado, faça algo
            //AJEITAR ISSO
            printf("Comando * encontrado\n");
            // Adicione aqui a lógica para processar *
            //impressaodados();
        } else {
            printf("Comando Incompatibility between commands.txt and command line: \n");
        }
    }
}