#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
    #define CLEAR "clear"
#endif
#ifdef _WIN32
    #define CLEAR "cls"
#endif

// static pointers
#define USER_OPTION        ((char*)(pBuffer))
#define FOUND              ((int*)(pBuffer + sizeof(char*)))
#define MESSAGE            ((char*)(pBuffer + sizeof(char*) + sizeof(int)))
#define TMP_NODE_NAME      ((char*)(pBuffer + sizeof(char*) + sizeof(int) + 50))
#define TMP_NODE_AGE       ((char*)(pBuffer + sizeof(char*) + sizeof(int) + 50 + 50))
#define TMP_NODE_EMAIL     ((char*)(pBuffer + sizeof(char*) + sizeof(int) + 50 + 50 + 10))
// head pointers
#define HEAD               ((void**)(pBuffer + sizeof(char*) + sizeof(int) + 50 + 50 + 10 + 60))
#define P_HEAD             ((void**)HEAD)
#define P_HEAD_NAME        (*(char**)(*P_HEAD))
#define P_HEAD_AGE         (*((char**)(*P_HEAD + sizeof(char*))))
#define P_HEAD_EMAIL       (*((char**)(*P_HEAD + 2 * sizeof(char*))))
#define P_HEAD_PREV        (*((void**)(*P_HEAD + 3 * sizeof(char*))))
#define P_HEAD_NEXT        (*((void**)(*P_HEAD + 3 * sizeof(char*) + sizeof(void*))))
// create
#define NODE_TO_CREATE_NAME        ((char*)(node))
#define NODE_TO_CREATE_AGE         ((char*)(node + sizeof(char*)))
#define NODE_TO_CREATE_EMAIL       ((char*)(node + 2 * sizeof(char*)))
#define NODE_TO_CREATE_PREV        ((void**)(node + 3 * sizeof(char*)))
#define NODE_TO_CREATE_NEXT        ((void**)(node + 3 * sizeof(char*) + sizeof(void*)))
#define NODE_TO_CREATE_NEXT_PREV   ((void**)(NODE_TO_CREATE_NEXT + sizeof(void*)))
// remove
#define NODE_TO_REMOVE_NAME        ((char*)(NODE_TO_REMOVE))
#define NODE_TO_REMOVE_AGE         ((char*)(NODE_TO_REMOVE + sizeof(char*)))
#define NODE_TO_REMOVE_EMAIL       ((char*)(NODE_TO_REMOVE + 2 * sizeof(char*)))
#define NODE_TO_REMOVE_PREV        ((void**)(NODE_TO_REMOVE + 3 * sizeof(char*)))
#define NODE_TO_REMOVE_NEXT        ((void**)(NODE_TO_REMOVE + 3 * sizeof(char*) + sizeof(void*)))
#define NODE_TO_REMOVE_NEXT_NEXT   ((void**)(NODE_TO_REMOVE_NEXT + sizeof(void*)))
#define NODE_TO_REMOVE_PREV_NEXT   ((void**)(NODE_TO_REMOVE_PREV + sizeof(void*) + 3 * sizeof(char*)))
#define P_HEAD_NEXT_PREV   (*((void**)(P_HEAD_NEXT)))
//colors
#define GREEN              "\x1b[32m"
#define RED                "\x1b[31m"
#define RESET              "\x1b[0m"

void displayMenu(void* pBuffer);
void invalidOption(void* pBuffer);
void* createNode(void* pBuffer);
void getUserInput(void* pBuffer);
void insertInOrder(void* pBuffer);
void printList(void* pBuffer);
void removePerson(void* pBuffer);
void searchPerson(void* pBuffer);

int main(void) {
    void* pBuffer = malloc(256 * sizeof(char));
    
    if (pBuffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    displayMenu(pBuffer);
    free(pBuffer);
    
    return 0;
}

void displayMenu(void* pBuffer) {
    system(CLEAR);
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
    if (*MESSAGE == '\0' || strlen(MESSAGE) == 0) {
        printf(">> ");
    } else {
        printf("%s\n\n", MESSAGE);
        printf(">> ");
    }
    scanf(" %c", USER_OPTION);
    getchar();
    switch (*USER_OPTION) {
        case '1':
            getUserInput(pBuffer);
            insertInOrder(pBuffer);
            displayMenu(pBuffer);
            break;
        case '2':   
            removePerson(pBuffer);
            displayMenu(pBuffer);
            break;
        case '3':   
            searchPerson(pBuffer);
            displayMenu(pBuffer);
            break;
        case '4':   
            printList(pBuffer);
            displayMenu(pBuffer);
            break;
        case '5': 
            break;
        default:
            invalidOption(pBuffer);
            displayMenu(pBuffer);
            break;
    }
}

void invalidOption(void* pBuffer) {
    strcpy(MESSAGE, RED "Opção inválida" RESET);
}

void* createNode(void* pBuffer) {
    void* node = malloc(3 * sizeof(char*) + 2 * sizeof(void*));

    if (node == NULL) {
        return NULL;
    }

    NODE_TO_CREATE_NAME = (char*)malloc(strlen(TMP_NODE_NAME) + 1);
    if (NODE_TO_CREATE_NAME == NULL) {
        free(node);
        return NULL;
    }
    strcpy(NODE_TO_CREATE_NAME, TMP_NODE_NAME);

    NODE_TO_CREATE_AGE = (char*)malloc(strlen(TMP_NODE_AGE) + 1);
    if (NODE_TO_CREATE_AGE == NULL) {
        free(NODE_TO_CREATE_NAME);
        free(node);
        return NULL;
    }
    strcpy(NODE_TO_CREATE_AGE, TMP_NODE_AGE);

    NODE_TO_CREATE_EMAIL = (char*)malloc(strlen(TMP_NODE_EMAIL) + 1);
    if (NODE_TO_CREATE_EMAIL == NULL) {
        free(NODE_TO_CREATE_NAME);
        free(NODE_TO_CREATE_AGE);
        free(node);
        return NULL;
    }
    strcpy(NODE_TO_CREATE_EMAIL, TMP_NODE_EMAIL);

    NODE_TO_CREATE_PREV = NULL;
    NODE_TO_CREATE_NEXT = NULL;

    return node;
}

void insertInOrder(void* pBuffer) {
    void* node = createNode(pBuffer);

    if (node == NULL) {
        return;
    }                       
    
    if (*P_HEAD == NULL || strcmp(NODE_TO_CREATE_NAME, *P_HEAD_NAME) < 0) {
        NODE_TO_CREATE_PREV = NULL;
        NODE_TO_CREATE_NEXT = *P_HEAD;
        if (*P_HEAD != NULL) {
            *P_HEAD_NEXT_PREV = node;
        }
        *P_HEAD = node;
    } else {
        void** current = *P_HEAD;
        while (*current != NULL && strcmp(NODE_TO_CREATE_NAME, *(char**)(*current)) > 0) {
            current = (void**)(*current + sizeof(void*));
        }
        NODE_TO_CREATE_PREV = *(void**)(*current);
        NODE_TO_CREATE_NEXT = *current;
        if (*current != NULL) {
            *(void**)(*current + sizeof(void*)) = node;
        }
        *current = node;
    }
    strcpy(MESSAGE, GREEN "Contato adicionado com sucesso" RESET);
}

void getUserInput(void* pBuffer) {
    printf("\nNome: ");
    scanf(" %[^\n]s", TMP_NODE_NAME);
    getchar();
    printf("Idade: ");
    scanf(" %[^\n]s", TMP_NODE_AGE);
    getchar();
    printf("Email: ");
    scanf(" %[^\n]s", TMP_NODE_EMAIL);
    getchar(); 
}

void printList(void* pBuffer) {
    if (*P_HEAD == NULL) {
        strcpy(MESSAGE, RED "A lista está vazia" RESET);
        return;
    }
    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    void* current = *P_HEAD;
    while (current != NULL) {
        printf("Nome:  %s\nIdade: %s\nEmail: %s\n", 
               NODE_TO_CREATE_NAME,
               NODE_TO_CREATE_AGE,
               NODE_TO_CREATE_EMAIL);
        printf("-----------------------------------------------\n");
        current = *NODE_TO_CREATE_NEXT;
    }
    printf("\nAperte ENTER para sair ");
    getchar();
}

void searchPerson(void* pBuffer) {
    if (*P_HEAD == NULL) {
        strcpy(MESSAGE, RED "A lista está vazia" RESET);
        return;
    }
    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", TMP_NODE_NAME);
    getchar();
    *FOUND = 0;
    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    void* current = *P_HEAD;
    while (current != NULL) {
        if (strncmp(NODE_TO_CREATE_NAME, TMP_NODE_NAME, strlen(TMP_NODE_NAME)) == 0) {
            printf("Nome:  %s\nIdade: %s\nEmail: %s\n",
                   NODE_TO_CREATE_NAME,
                   NODE_TO_CREATE_AGE,
                   NODE_TO_CREATE_EMAIL);
            printf("-----------------------------------------------\n");
            *FOUND = 1;
        }
        current = *NODE_TO_CREATE_NEXT;
    }
    if (!*FOUND) {
        strcpy(MESSAGE, RED "Nenhum contato encontrado" RESET);
    } else {
        strcpy(MESSAGE, "");
        printf("\nAperte ENTER para sair ");
        getchar();
    }
}

void removePerson(void* pBuffer) {
    if (*P_HEAD == NULL) {
        strcpy(MESSAGE, RED "A lista está vazia" RESET);
        return;
    }
    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", TMP_NODE_NAME);
    getchar();
    if (strcmp(NODE_TO_REMOVE_NAME, TMP_NODE_NAME) == 0) {
        *P_HEAD = *NODE_TO_REMOVE_NEXT;
        if (*P_HEAD != NULL) {
            *P_HEAD_PREV = NULL;
        }
        free(NODE_TO_REMOVE_NAME);
        free(NODE_TO_REMOVE_AGE);
        free(NODE_TO_REMOVE_EMAIL);
        free(NODE_TO_REMOVE);
        strcpy(MESSAGE, GREEN "Contato removido com sucesso" RESET);
        return;
    }
    void* current = *P_HEAD;
    while (current != NULL && strcmp(NODE_TO_REMOVE_NAME, *(char**)(current)) != 0) {
        current = *NODE_TO_REMOVE_NEXT;
    }
    if (current == NULL) {
        strcpy(MESSAGE, RED "Contato não encontrado" RESET);
        return;
    }
    if (*NODE_TO_REMOVE_NEXT != NULL) {
        *(void**)(*NODE_TO_REMOVE_NEXT + sizeof(void*)) = *NODE_TO_REMOVE_PREV;
    }
    if (*NODE_TO_REMOVE_PREV != NULL) {
        *(void**)(*NODE_TO_REMOVE_PREV + sizeof(void*)) = *NODE_TO_REMOVE_NEXT;
    } else {
        *P_HEAD = *NODE_TO_REMOVE_NEXT;
    }
    free(NODE_TO_REMOVE_NAME);
    free(NODE_TO_REMOVE_AGE);
    free(NODE_TO_REMOVE_EMAIL);
    free(NODE_TO_REMOVE);
    strcpy(MESSAGE, GREEN "Contato removido com sucesso" RESET);
}
