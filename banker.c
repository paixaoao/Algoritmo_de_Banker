#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include <unistd.h>
#define MAX_RESOURCES 100
#define MAX_PROCESS 100
//ERROS A SEREM IMPLEMENTADOS:
//Incompatibility between customer.txt and command line
//Incompatibility between commands.txt and command line
//int is_safe_state(int available[MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int numCustomers, int numResources);
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
        exit(1);
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

   /* A matriz de alocação é uma tabela que representa a alocação de recursos para cada processo em um sistema. Ele ajuda o Banker a tomar decisões sobre a segurança das alocações de recursos*/
    leituracommand(arq_customer, available, resources, max, allocation, need, numProcesses);

    fclose(arq_customer);
    return 0;
}
int leituracommand(FILE *customer, int *available, int resources, int max[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int num_processes) {
    FILE *file = fopen("commands.txt", "r");
    if (file == NULL) {
        printf("Fail to read commands.txt\n");
        exit(1);
    }

    char command[3];
    int processIndex, req[resources];
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
            if (customer >= num_processes) {
                fprintf(result_file, "Invalid process ID %d\n", customer);
                continue;
            }
            if (strcmp(command, "RQ") == 0) {
                bool validRequest = true;
                for (int i = 0; i < resources; i++) {
                    fscanf(file, "%d", &req[i]);
                    if (req[i] > need[customer][i]) {
                        fprintf(result_file, "The customer %d request", customer);
                            for (int i = 0; i < resources; i++) {
                                fprintf(result_file, " %d", req[i]);
                            }           
                             fprintf(result_file, " was denied because exceeds its maximum need\n");
                            validRequest = false;
                    break;
                    }                    
                    if (req[i] > available[i]) {
                         fprintf(result_file, "The resources");
                        for (int j = 0; j < resources; j++)
                        fprintf(result_file, " %d", available[j]);
    
                        fprintf(result_file, " are not enough to customer %d request", customer);
    
                        for (int j = 0; j < resources; j++)
                        fprintf(result_file, " %d", req[j]);

                        fprintf(result_file, "\n");
                        validRequest = false;
                        break;
                }
                    /*if (validRequest && is_safe_state(available, need, allocation, customer, resources) == 3) {
                    fprintf(result_file, "The customer %d request", customer);
                    for (int i = 0; i < resources; i++) {
                    fprintf(result_file, " %d", req[i]);
                    }
                    fprintf(result_file, " was denied because result in an unsafe state\n");
                    continue;
                }*/
                }


                if (validRequest) {
                    for (int i = 0; i < resources; i++) {
                        available[i] -= req[i];
                        allocation[customer][i] += req[i];
                        need[customer][i] -= req[i];
                    }

                    fprintf(result_file,"Allocate to customer %d the resources", customer);
                    for (int i = 0; i < resources; i++) {
                        fprintf(result_file," %d", req[i]);
                    }
                    fprintf(result_file,"\n");
                }           
            } else if (strcmp(command, "RL") == 0) {
                if (customer < 0 || customer >= num_processes) {
                    fprintf(result_file, "Incompatibility between customer.txt and command line");
                    exit(1);
                } else {
                    for (int i = 0; i < resources; i++) {
                        int released;
                        if (fscanf(file, "%d", &released) != 1) {
                        fprintf(result_file, "Error: Failed to read release value\n");
                        exit(1);
                         }
                         if (released < 0) {
                            fprintf(result_file, "Error: Negative release value (%d) for customer %d\n", released, customer);
                            exit(1);
                        }
                        if (released > allocation[customer][i]) {
                        fprintf(result_file, "The customer %d released", customer);
                        for (int j = 0; j < resources; j++) {
                        fprintf(result_file, " %d", released);
                        }
                        fprintf(result_file, " was denied because exceed its maximum allocation\n");
                        continue;
                    }
                        else {
                            allocation[customer][i] -= released;
                            available[i] += released;

                            fprintf(result_file,"Release from customer %d the resources", customer);
                            for (int i = 0; i < resources; i++) {
                                fprintf(result_file," %d", released);
                            }
                            fprintf(result_file,"\n");
                        }
                    }
                }
            }
        }else if (strcmp(command, "*") == 0) {
    int max_width = 0;  // Largura máxima para cada coluna

    // Calcular a largura máxima para cada coluna em max
    for (int j = 0; j < resources; j++) {
        for (int i = 0; i < num_processes; i++) {
            int width = snprintf(NULL, 0, "%d", max[i][j]);
            if (width > max_width) {
                max_width = width;
            }
        }
    }

    // Imprimir as tabelas formatadas
    fprintf(result_file, "MAXIMUM | ALLOCATION | NEED\n");
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < resources; j++) {
            fprintf(result_file, "%*d ", max_width, max[i][j]);
        }
        fprintf(result_file, "| ");
        for (int j = 0; j < resources; j++) {
            fprintf(result_file, "%*d ", max_width, allocation[i][j]);
        }
        fprintf(result_file, "| ");
        for (int j = 0; j < resources; j++) {
            fprintf(result_file, "%*d ", max_width, need[i][j]);
        }
        fprintf(result_file, "\n");
    }

    // Imprimir a tabela AVAILABLE
    fprintf(result_file, "AVAILABLE ");
    for (int i = 0; i < resources; i++) {
        fprintf(result_file, "%*d ", max_width, available[i]);
    }
    fprintf(result_file, "\n");
} else {
    printf("Incompatibility between commands.txt and command line: \n");
    //exit(1);
}
    }
    fclose(file);
    fclose(result_file);
}




/*int is_safe_state(int available[MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int numCustomers, int numResources) {
    bool canFinish[MAX_PROCESS];

    for (int i = 0; i < numCustomers; ++i) {
        canFinish[i] = true;
    }

    for (int i = 0; i < numCustomers; ++i) {
        if (canFinish[i]) {
            for (int j = 0; j < numResources; ++j) {
                if (need[i][j] > available[j]) {
                    canFinish[i] = false;
                    return 3;
                }
            }

            if (canFinish[i]) {
                for (int j = 0; j < numResources; ++j) {
                    available[j] += allocation[i][j];
                }
            }
        }
    }

    bool safeState = true;
    for (int i = 0; i < numCustomers; ++i) {
        if (!canFinish[i]) {
            safeState = false;
            break;
        }
    }

    return safeState;
}*/
