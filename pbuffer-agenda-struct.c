#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define 	MEMORY_SIZE 		1024

#define 	P_USEROPTION 		(pBuffer)
#define 	P_MESSAGE 			(P_USEROPTION     + sizeof(int))
#define 	P_TMP_NODE_NAME 	(P_MESSAGE        + 50 * sizeof(char))
#define 	P_TMP_NODE_AGE 		(P_TMP_NODE_NAME  + 50 * sizeof(char))
#define 	P_TMP_NODE_EMAIL 	(P_TMP_NODE_AGE   + 10 * sizeof(char))
#define 	P_HEAD 				(P_TMP_NODE_EMAIL + 60 * sizeof(char))

#define 	GREEN 				"\x1b[32m"
#define 	RED 				"\x1b[31m"
#define 	RESET 				"\x1b[0m"

typedef struct Node_t {
    char* name;
    char* age;
    char* email;
    struct Node_t* prev;
    struct Node_t* next;
} Node_t;

Node_t* createNode(const char* name, const char* age, const char* email);
void displayMenu();
void getUserInput(void* pBuffer);
void insertInOrder(void* pBuffer);
void printList(void* pBuffer);
void searchPerson(void* pBuffer);
void removeNode(void* pBuffer);

int main() {
    void* pBuffer = malloc(MEMORY_SIZE);
    int* userOption = (int*)P_USEROPTION;
    char* message = (char*)P_MESSAGE;
    Node_t** head = (Node_t**)P_HEAD;
    
    *head = NULL;

    if ( pBuffer == NULL ) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *userOption = 1;

    while ( *userOption != 5 ) {
        system("clear");
        displayMenu();

        if ( *message == '\0' || strlen(message) == 0 ) {
            printf(">> ");
        } else {
            printf("%s\n\n", message);
            printf(">> ");
        }

        scanf("%d", userOption);

        switch ( *userOption ) {
            case 1:
                getUserInput(pBuffer);
                insertInOrder(pBuffer);
                strcpy(message, GREEN "Contato adicionado com sucesso" RESET);
                break;
            case 2:
                if ( *head == NULL ) {
                    strcpy(message, RED "A lista está vazia" RESET);
                } else {
                    removeNode(pBuffer);
                }
                break;
            case 3:
                if ( *head == NULL ) {
                    strcpy(message, RED "A lista está vazia" RESET);
                } else {
                    searchPerson(pBuffer);
                }
                break;
            case 4:
                if ( *head == NULL ) {
                    strcpy(message, RED "A lista está vazia" RESET);
                } else {
                    strcpy(message, "");
                    printList(pBuffer);
                }
                break;
            case 5:
                break;
            default:
                strcpy(message, RED "Opção inválida" RESET);
                break;
        }
    }

    free(pBuffer);
    
	return 0;
}

void displayMenu() {
    printf("==================================\n"
           "||            AGENDA            ||\n"
           "==================================\n"
           "|| 1. Adicionar pessoa          ||\n"
           "|| 2. Remover pessoa            ||\n"
           "|| 3. Buscar pessoa             ||\n"
           "|| 4. Listar todos              ||\n"
           "|| 5. Sair                      ||\n"
           "==================================\n"
           "\n");
}

Node_t* createNode(const char* name, const char* age, const char* email) {
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));

    if ( node == NULL ) {
        return NULL;
    }

    node->name = (char*)malloc(strlen(name) + 1);
    if ( node->name == NULL ) {
        free(node);
        return NULL;
    }
    strcpy(node->name, name);

    node->age = (char*)malloc(strlen(age) + 1);
    if ( node->age == NULL ) {
        free(node->name);
        free(node);
        return NULL;
    }
    strcpy(node->age, age);

    node->email = (char*)malloc(strlen(email) + 1);
    if ( node->email == NULL ) {
        free(node->age);
        free(node->name);
        free(node);
        return NULL;
    }
    strcpy(node->email, email);

    node->next = NULL;
    node->prev = NULL;
    return node;
}

void insertInOrder(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    char* name = (char*)P_TMP_NODE_NAME;
    char* age = (char*)P_TMP_NODE_AGE;
    char* email = (char*)P_TMP_NODE_EMAIL;
    Node_t* node = createNode(name, age, email);

    if ( node == NULL ) {
        return;
    }

    if ( *head == NULL || strcmp((*head)->name, node->name) > 0 ) {
        node->next = *head;
        if ( *head != NULL ) {
            (*head)->prev = node;
        }
        *head = node;
    } else {
        Node_t* current = *head;
        while ( current->next != NULL && strcmp(current->next->name, node->name) <= 0 ) {
            current = current->next;
        }
        node->next = current->next;
        if ( current->next != NULL ) {
            current->next->prev = node;
        }
        current->next = node;
        node->prev = current;
    }
}

void printList(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    Node_t* current = *head;

    system("clear");

    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    while ( current != NULL ) {
        printf("Name:  %s\nAge:   %s\nEmail: %s\n", current->name, current->age, current->email);
        printf("-----------------------------------------------\n");
        current = current->next;
    }

    printf("\nAperte ENTER para sair ");
    getchar();
    getchar();
	getchar();
}

void getUserInput(void* pBuffer) {
    char* name = (char*)P_TMP_NODE_NAME;
    char* age = (char*)P_TMP_NODE_AGE;
    char* email = (char*)P_TMP_NODE_EMAIL;

    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", name);

    printf("Digite a idade: ");
    scanf(" %[^\n]s", age);

    printf("Digite o email: ");
    scanf(" %[^\n]s", email);
}

void searchPerson(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    Node_t* current = *head;
    char* pattern = (char*)P_TMP_NODE_NAME;
    int* found = (int*)P_USEROPTION;
    char* message = (char*)P_MESSAGE;

    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", pattern);

    *found = 0;

    system("clear");

    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    while ( current != NULL ) {
        if ( strncmp(current->name, pattern, strlen(pattern)) == 0 ) {
            printf("Nome:  %s\nIdade: %s\nEmail: %s\n", current->name, current->age, current->email);
            printf("-----------------------------------------------\n");
            *found = 1;
        }
        current = current->next;
    }

    if ( !*found ) {
        strcpy(message, RED "Nenhum contato encontrado" RESET);
    } else {
        strcpy(message, "");
        printf("\nAperte ENTER para sair ");
        getchar();
        getchar();
    }
}

void removeNode(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    char* nameToRemove = (char*)P_TMP_NODE_NAME;
    char* message = (char*)P_MESSAGE;
    Node_t* current = *head;
    Node_t* nodeToRemove = NULL;

    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", nameToRemove);

    if ( strcmp((*head)->name, nameToRemove) == 0 ) {
        nodeToRemove = *head;
        *head = (*head)->next;
        if ( *head != NULL ) {
            (*head)->prev = NULL;
        }
        free(nodeToRemove->name);
        free(nodeToRemove->age);
        free(nodeToRemove->email);
        free(nodeToRemove);
        strcpy(message, GREEN "Contato removido com sucesso" RESET);
        return;
    }

    while ( current != NULL && strcmp(current->name, nameToRemove) != 0 ) {
        current = current->next;
    }

    if ( current == NULL ) {
        strcpy(message, RED "Contato não encontrado" RESET);
        return;
    }

    nodeToRemove = current;
    current->prev->next = current->next;
    if ( current->next != NULL ) {
        current->next->prev = current->prev;
    }
    free(nodeToRemove->name);
    free(nodeToRemove->age);
    free(nodeToRemove->email);
    free(nodeToRemove);
    strcpy(message, GREEN "Contato removido com sucesso" RESET);
}

