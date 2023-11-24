#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_RESOURCES 10000
#define MAX_PROCESS 10000
//ERROS A SEREM IMPLEMENTADOS:
//Incompatibility between customer.txt and command line
//Incompatibility between commands.txt and command line
int is_safe(int *available,int numProcesses, int need [MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES]);
int leituracommand(FILE *file,int *available,int resources, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[]);
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
    leituracommand(NULL, available, resources, max, allocation, need, numProcesses, resources, commands);

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
int leituracommand(FILE *file,int *available,int resources, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes, int num_resources, char commands[]){
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
    if (strcmp(command, "RQ") == 0 || strcmp(command, "RL") == 0) {
        int customer;
        fscanf(file, "%d", &customer);
        if (strcmp(command, "RQ") == 0) {
            // Adicione aqui a lógica para processar RQ
            for (int i = 0; i < num_resources; i++)
                fscanf(file, "%d", &req[i]);

            for (int i = 0; i < num_resources; i++) {
                if (req[i] > need[customer][i]) { // Change processIndex to customer
                    fprintf(result_file, "The customer %d request %d was denied because it exceeds its maximum need\n", customer, req[i]);
                }
            }for (int i = 0; i < resources; i++){
                if (req[i] > available[i]) {
                    fprintf(result_file, "The resources %d %d %d are not enough to customer %d request %d %d %d\n", available[0], available[1], available[2], customer, req[0], req[1], req[2]); // Change resources[i] to available[i]
                }
                if(is_safe(available, num_processes, need, allocation)==1){
                    fprintf(result_file,"The customer %d request %d %d %d was denied because result in an unsafe state",customer,req[0],req[1],req[2]);
                }
            }
            for (int i = 0; i < num_resources; i++){
                available[i]=available[i]-req[i];
                allocation[customer][i]=allocation[customer][i]+req[i];
                need[customer][i]-=req[i];
                printf("Allocate to customer %d the resources", customer);
                for (int i = 0; i < num_resources; i++) {
                printf(" %d", req[i]);
            }
            printf("\n");
            }
            }
         else if (strcmp(command, "RL") == 0) {
        if (customer < 0 || customer >= num_processes) {
            fprintf(result_file, "Incompatibility between customer.txt and command line");
        } else {
        // Verifique se a liberação excede a alocação atual
        for (int i = 0; i < num_resources; i++) {
            int released;
            fscanf(file, "%d", &released);

            if (released > allocation[customer][i]) {
                fprintf(result_file, "The customer %d released %d was denied because it exceeds its maximum allocation\n", customer, released);
            } else {
                // Libere os recursos
                allocation[customer][i] -= released;
                available[i] += released;
                printf("Release from customer %d the resources", customer);
                for (int i = 0; i < num_resources; i++) {
                    printf(" %d", released);
                }
                printf("\n");
            }
        }
    }
}
    } else if (strcmp(command, "*") == 0) {
        // Comando * encontrado, faça algo
        //AJEITAR ISSO
        //printf("Comando * encontrado\n");
        // Adicione aqui a lógica para processar *
        printf("MAXIMUM | ALLOCATION | NEED\n");
        for (int i = 0; i < num_processes; i++) {
            for (int j = 0; j < num_resources; j++) {
                printf("%d ", max[i][j]);
            }
            printf("| ");
            for (int j = 0; j < num_resources; j++) {
                printf("%d ", allocation[i][j]);
            }
            printf("| ");
            for (int j = 0; j < num_resources; j++) {
                printf("%d ", need[i][j]);
            }
            printf("\n");
        }

        printf("AVAILABLE ");
        for (int i = 0; i < num_resources; i++) {
            printf("%d ", available[i]);
        }
        printf("\n");
    } else {
        printf("Incompatibility between commands.txt and command line: \n");
    }
}
}



int is_safe(int available[], int numProcesses, int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESS];

    for (int i = 0; i < numProcesses; i++) {
        work[i] = available[i];
    }

    for (int i = 0; i < numProcesses; i++) {
        finish[i] = 0;
    }

    int numFinishedProcesses = 0;
    while (numFinishedProcesses < numProcesses) {
        int found = 0;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                int resourceIndex;
                for (resourceIndex = 0; resourceIndex < MAX_RESOURCES; resourceIndex++) {
                    if (need[i][resourceIndex] > work[resourceIndex]) {
                        break;
                    }
                }
                if (resourceIndex == MAX_RESOURCES) {
                    for (int j = 0; j < MAX_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                    numFinishedProcesses++;
                    found = 1;
                }
            }
        }
        if (!found) {
            // Não foi encontrado um processo para executar
            int unsafe=1;
            return unsafe; // Estado não seguro
        }
    }

    return 0; // Estado seguro
}
