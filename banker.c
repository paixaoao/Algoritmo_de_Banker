#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#define MAX_RESOURCES 100
#define MAX_PROCESS 100
void print_simulation(int num_processes, int resources, int max[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int available[]);
int safety(int *available,int numProcesses, int need [MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES]);
int leituracommand(FILE *customer,int *available,int resources, int max[MAX_PROCESS][MAX_RESOURCES],int allocation [MAX_PROCESS][MAX_RESOURCES],int need [MAX_PROCESS][MAX_RESOURCES], int num_processes);
void request_resources(FILE *customer,int process, int request[], int available[], int num_processes, int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES]);
void release_resources(int customer, int release[], int available[], int num_processes, int allocation[MAX_PROCESS][MAX_RESOURCES],FILE *result_file,int resources);
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
    while (fgets(linha, sizeof(linha), arq_customer) != NULL) {
        numProcesses++;
    }
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
    FILE *arq_command = fopen("commands.txt", "r");
    if (arq_command == NULL) {
        printf("Fail to read commands.txt\n");
        return 1;
    }
    int i, j;
    char command[3];
    int processIndex, req[MAX_RESOURCES];
    FILE *result_file = fopen("result.txt", "w");
    if (result_file == NULL) {
        printf("Fail to create result.txt\n");
        fclose(arq_command);
        return 1;
    }

    while (fscanf(arq_command, "%s", command) == 1) {
            
            if (strcmp(command, "RQ") == 0) {
            int customer;
            fscanf(arq_command, "%d", &customer);
            request_resources(arq_command, customer, req, available, num_processes, need, allocation);
            } else if (strcmp(command, "RL") == 0) {
            int customer;
            fscanf(arq_command, "%d", &customer);
            int released[MAX_RESOURCES];
            for (int i = 0; i < resources; i++) {
                fscanf(arq_command, "%d", &released[i]);
            }
            release_resources(customer, released, available, num_processes, allocation,result_file,resources);
            }
         else if (strcmp(command, "*") == 0) {
                print_simulation(num_processes, resources, max, allocation, need, available);
        } else {
            printf("Incompatibility between commands.txt and command line: \n");
        }
    }

    fclose(arq_command);
    fclose(result_file);
}



int safety(int available[], int numProcesses, int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES]) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESS];
    int i, j;
    for (i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    for (i = 0; i < numProcesses; i++) {
        finish[i] = 0;
    }

    int numFinishedProcesses = 0;
    while (numFinishedProcesses < numProcesses) {
        int found = 0;
        for (i = 0; i < numProcesses; i++) {
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
void request_resources(FILE *customer,int process, int request[], int available[], int num_processes, int need[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES]) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > need[process][i]) {
            printf("The customer %d request %d was denied because it exceeds its maximum need.\n", process, i);
            return;
        }
    }
    if (safety(available, num_processes, need, allocation) == 0) {
    printf("The customer %d request was denied because it results in an unsafe state.\n", process);
    return;
    }

    for (int i = 0; i < MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    printf("The customer %d request was successful.\n", process);
}

// Função para liberar recursos
void release_resources(int customer, int release[], int available[], int num_processes, int allocation[MAX_PROCESS][MAX_RESOURCES],FILE *result_file,int resources) {
    int i;

    if (customer < 0 || customer >= num_processes) {
        printf("Incompatibility between customer.txt and command line");
    } else {
       for (i = 0; i < MAX_RESOURCES; i++) {
    int released;
    released = release[i];

    if (released > allocation[customer][i]) {
        printf("The customer %d released %d was denied because it exceeds its maximum allocation \n", customer, released);
    } else {
        allocation[customer][i] -= released;
        available[i] += released;
        printf("Release from customer %d the resources: %d\n", customer, released);
            }
        }
    }
}

void print_simulation(int num_processes, int resources, int max[MAX_PROCESS][MAX_RESOURCES], int allocation[MAX_PROCESS][MAX_RESOURCES], int need[MAX_PROCESS][MAX_RESOURCES], int available[]) {
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
    for (int i = 0; i < MAX_RESOURCES; i++) {
    printf("%d ", available[i]);
    }
    printf("\n");
}
