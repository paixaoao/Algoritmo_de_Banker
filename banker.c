#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#define MAX_RESOURCES 100
#define MAX_PROCESS 100
//ERROS A SEREM IMPLEMENTADOS:
//Incompatibility between customer.txt and command line
//Incompatibility between commands.txt and command line
int is_safe(int *available,int numProcesses, int need [MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES]);
int leituracommand(FILE *customer,int *available,int resources, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes);
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("No resources.\n");
        return 1;
    }

    FILE *arq_customer = fopen("customer.txt", "r");
    if (arq_customer == NULL) {
        printf("Fail to read customer.txt");
        return 1;
    }

    int i, j;
    char linha[70];
    int numProcesses = 0;

    // Read the file to get the number of processes
    while (fgets(linha, sizeof(linha), arq_customer) != NULL) {
        numProcesses++;
    }

    // Reset file pointer to the beginning of the file
    fseek(arq_customer, 0, SEEK_SET);

    int resources = argc - 1;

    int available[resources];

    for (i = 0; i < resources; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    int allocation[MAX_PROCESS][MAX_RESOURCES];
    int max[MAX_PROCESS][MAX_RESOURCES];
    int need[MAX_PROCESS][MAX_RESOURCES];

    for (i = 0; i < numProcesses; ++i) {
        if (fgets(linha, sizeof(linha), arq_customer) == NULL) {
            fprintf(stderr, "Erro ao ler valores do arquivo\n");
            return 1;
        }

        char *token = strtok(linha, ",");
        for (j = 0; j < resources; ++j) {
            if (token != NULL) {
                if (atof(token) >= 0) {
                    max[i][j] = atoi(token);
                    need[i][j] = max[i][j];
                } else {
                    printf("Recurso Negativo.\n"); // TRATAMENTO
                    exit(1);
                }
            }
            token = strtok(NULL, ",");
        }
    }
        for(i=0;i<numProcesses;i++){
        // Initialize allocation and need arrays for each process
        for (j = 0; j < resources; j++) {
            allocation[i][j] = 0;
        }
    }
    /*for(i=0;i<numProcesses;i++){
        for(j=0;j<resources;j++){
            printf("NA MAIN O MAX ESTA\n");
            printf("%d\n",max[i][j]);
            printf("NA MAIN O allocation ESTA\n");
            printf("%d\n",allocation[i][j]);
            printf("NA MAIN O need ESTA\n");
            printf("%d\n",need[i][j]);
        }
    }
   /* A matriz de alocação é uma tabela que representa a alocação de recursos para cada processo em um sistema. Ele ajuda o Banker a tomar decisões sobre a segurança das alocações de recursos*/
    leituracommand(arq_customer, available, resources, max, allocation, need, numProcesses);

    fclose(arq_customer);
    return 0;
}
int leituracommand(FILE *customer, int *available, int resources, int max[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int num_processes) {
    FILE *file = fopen("commands.txt", "r");
    if (file == NULL) {
        printf("Fail to read commands.txt\n");
        return 1;
    }

    char command[3];
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
                for (int i = 0; i < resources; i++) {
                    fscanf(file, "%d", &req[i]);
                    if (req[i] > need[customer][i]) {
                        fprintf(result_file, "The customer %d request %d was denied because it exceeds its maximum need\n", customer, req[i]);
                    }
                }
                for (int i = 0; i < resources; i++) {
                    available[i] = available[i] - req[i];
                    allocation[customer][i] = allocation[customer][i] + req[i];
                    need[customer][i] -= req[i];
                }

                printf("Allocate to customer %d the resources", customer);
                for (int i = 0; i < resources; i++) {
                    printf(" %d", req[i]);
                }
                printf("\n");
            } else if (strcmp(command, "RL") == 0) {
                if (customer < 0 || customer >= num_processes) {
                    fprintf(result_file, "Incompatibility between customer.txt and command line");
                } else {
                    for (int i = 0; i < resources; i++) {
                        int released;
                        fscanf(file, "%d", &released);

                        if (released > allocation[customer][i]) {
                            fprintf(result_file, "The customer %d released %d was denied because it exceeds its maximum allocation\n", customer, released);
                        } else {
                            allocation[customer][i] -= released;
                            available[i] += released;

                            printf("Release from customer %d the resources", customer);
                            for (int i = 0; i < resources; i++) {
                                printf(" %d", released);
                            }
                            printf("\n");
                        }
                    }
                }
            }
        } else if (strcmp(command, "*") == 0) {
            printf("MAXIMUM | ALLOCATION | NEED\n");
            for (int i = 0; i < num_processes; i++) {
                for (int j = 0; j < resources; j++) {
                    printf("%d ", max[i][j]);
                }
                printf("| ");
                for (int j = 0; j < resources; j++) {
                    printf("%d ", allocation[i][j]);
                }
                printf("| ");
                for (int j = 0; j < resources; j++) {
                    printf("%d ", need[i][j]);
                }
                printf("\n");
            }

            printf("AVAILABLE ");
            for (int i = 0; i < resources; i++) {
                printf("%d ", available[i]);
            }
            printf("\n");
        } else {
            printf("Incompatibility between commands.txt and command line: \n");
        }
    }

    fclose(file);
    fclose(result_file);
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
            int unsafe=0;
            //printf("Estado não seguro encontrado.\n");
            return unsafe; // Estado não seguro
        }
    }

    return 1; // Estado seguro
}
