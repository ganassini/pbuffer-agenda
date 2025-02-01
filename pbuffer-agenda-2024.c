#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
    #define CLEAR "clear"
#endif
#ifdef _WIN32
    #define CLEAR "cls"
#endif

#define P_USEROPTION        ((char*)pBuffer)
#define P_MESSAGE           ((char*)(P_USEROPTION + sizeof(int)))
#define P_TMP_NODE_NAME     ((char*)(P_MESSAGE + 50 * sizeof(char)))
#define P_TMP_NODE_AGE      ((char*)(P_TMP_NODE_NAME + 50 * sizeof(char)))
#define P_TMP_NODE_EMAIL    ((char*)(P_TMP_NODE_AGE + 10 * sizeof(char)))
#define P_HEAD              ((void**)(P_TMP_NODE_EMAIL + 60 * sizeof(char)))
#define GREEN               "\x1b[32m"
#define RED                 "\x1b[31m"
#define RESET               "\x1b[0m"

void displayMenu(void* pBuffer);
void invalidOption(void* pBuffer);
void* createNode(void* pBuffer);
void getUserInput(void* pBuffer);
void insertNode(void* pBuffer);
void printList(void* pBuffer);
void removeNode(void* pBuffer);
void searchPerson(void* pBuffer);

int main(void) {
    void* pBuffer = malloc(256 * sizeof(char));
    
    if ( pBuffer == NULL ) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    *(void**)P_HEAD = NULL;
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
    if ( *P_MESSAGE == '\0' || strlen(P_MESSAGE) == 0 ) {
            printf(">> ");
        } else {
            printf("%s\n\n", P_MESSAGE);
            printf(">> ");
        }
    scanf(" %c", P_USEROPTION);
    getchar();
    switch ( *P_USEROPTION ) {
        case '1':
            getUserInput(pBuffer);
            insertNode(pBuffer);
            displayMenu(pBuffer);
            break;
        case '2':   
            removeNode(pBuffer);
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
    strcpy(P_MESSAGE, RED "Opção inválida" RESET);
}

void* createNode(void* pBuffer) {
    void* node = malloc(3 * sizeof(char*) + 2 * sizeof(void*));

    if ( node == NULL ) {
        return NULL;
    }

    char* name = (char*)P_TMP_NODE_NAME;
    char* age = (char*)P_TMP_NODE_AGE;
    char* email = (char*)P_TMP_NODE_EMAIL;

    *((char**)(node)) = (char*)malloc(strlen(name) + 1);
    if ( *((char**)(node)) == NULL ) {
        free(node);
        return NULL;
    }
    strcpy(*((char**)(node)), name);

    *((char**)(node + sizeof(char*))) = (char*)malloc(strlen(age) + 1);
    if ( *((char**)(node + sizeof(char*))) == NULL ) {
        free(*((char**)(node)));
        free(node);
        return NULL;
    }
    strcpy(*((char**)(node + sizeof(char*))), age);

    *((char**)(node + 2 * sizeof(char*))) = (char*)malloc(strlen(email) + 1);
    if ( *((char**)(node + 2 * sizeof(char*))) == NULL ) {
        free(*((char**)(node)));
        free(*((char**)(node + sizeof(char*))));
        free(node);
        return NULL;
    }
    strcpy(*((char**)(node + 2 * sizeof(char*))), email);

    *((void**)(node + 3 * sizeof(char*))) = NULL;
    *((void**)(node + 3 * sizeof(char*) + sizeof(void*))) = NULL;

    return node;
}

void insertNode(void* pBuffer) {
    void** head = (void**)P_HEAD;
    void* node = createNode(pBuffer);

    if ( node == NULL ) {
        return;
    }
    if ( *head == NULL || strcmp(*((char**) *head), *((char**) node)) > 0 ) {
        *((void**)(node + 3 * sizeof(char*))) = *head;
        if ( *head != NULL ) {
            *((void**)(*head + 3 * sizeof(char*) + sizeof(void*))) = node;
        }
        *head = node;
    } else {
        void* current = *head;
        while (*((void**)(current + 3 * sizeof(char*))) != NULL &&
               strcmp(*((char**)(*((void**)(current + 3 * sizeof(char*))))), *((char**) node)) <= 0) {
            current = *((void**)(current + 3 * sizeof(char*)));
        }
        *((void**)(node + 3 * sizeof(char*))) = *((void**)(current + 3 * sizeof(char*))); 
        if (*((void**)(current + 3 * sizeof(char*))) != NULL) {
            *((void**)(*((void**)(current + 3 * sizeof(char*))) + 3 * sizeof(char*) + sizeof(void*))) = node; 
        }
        *((void**)(current + 3 * sizeof(char*))) = node; 
        *((void**)(node + 3 * sizeof(char*) + sizeof(void*))) = current; 
    }
	strcpy(P_MESSAGE, GREEN"Contato adicionado com sucesso"RESET);
}

void getUserInput(void* pBuffer) {
    printf("\nNome: ");
    scanf(" %[^\n]s", (char*)P_TMP_NODE_NAME);
    getchar();
    printf("Idade: ");
    scanf(" %[^\n]s", (char*)P_TMP_NODE_AGE);
    getchar();
    printf("Email: ");
    scanf(" %[^\n]s", (char*)P_TMP_NODE_EMAIL);
    getchar(); 
}

void printList(void* pBuffer) {
    void** head = (void**)P_HEAD;
    void* current = *head;
    
    if (current == NULL) {
        strcpy(P_MESSAGE, RED"A lista está vazia"RESET);
        return;
    }
    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    while (current != NULL) {
        printf("Name:  %s\nAge:   %s\nEmail: %s\n", 
               *((char**)current),
               *((char**)(current + sizeof(char*))),
               *((char**)(current + 2 * sizeof(char*))));
		printf("-----------------------------------------------\n");
        current = *((void**)(current + 3 * sizeof(char*)));
    }
    printf("\nAperte ENTER para sair ");
    getchar();
}

void searchPerson(void* pBuffer) {
    void** head = (void**)P_HEAD;
    void* current = *head;
    char* pattern = (char*)P_TMP_NODE_NAME;
    int* found = (int*)P_USEROPTION;
    char* message = (char*)P_MESSAGE;
	
	if (*head == NULL) {
        strcpy(message, RED"A lista está vazia"RESET);
        return;
    }
    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", pattern);
    getchar();
	*found = 0;
    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");
    while (current != NULL) {
        if (strncmp(*((char**)current), pattern, strlen(pattern)) == 0) {
            printf("Nome:  %s\nIdade: %s\nEmail: %s\n",
                   *((char**)current),
                   *((char**)(current + sizeof(char*))),
                   *((char**)(current + 2 * sizeof(char*))));
            printf("-----------------------------------------------\n");
            *found = 1;
        }
        current = *((void**)(current + 3 * sizeof(char*)));
    }
    if (!*found) {
        strcpy(message, RED "Nenhum contato encontrado" RESET);
    } else {
        strcpy(message, "");
        printf("\nAperte ENTER para sair ");
        getchar();
    }
}

void removeNode(void* pBuffer) {
    void** head = (void**)P_HEAD;
    char* nameToRemove = (char*)P_TMP_NODE_NAME;
    char* message = (char*)P_MESSAGE;
    void* current = *head;
    void* nodeToRemove = NULL;

	if (*head == NULL) {
        strcpy(message, RED"A lista está vazia"RESET);
        return;
    }
    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", nameToRemove);
    getchar();
    if (strcmp(*((char**)(*head)), nameToRemove) == 0) {
        nodeToRemove = *head;
        *head = *((void**)(*head + 3 * sizeof(char*)));
        if (*head != NULL) {
            *((void**)(*head + 3 * sizeof(char*) + sizeof(void*))) = NULL;
        }
        free(*((char**)nodeToRemove));
        free(*((char**)(nodeToRemove + sizeof(char*))));
        free(*((char**)(nodeToRemove + 2 * sizeof(char*))));
        free(nodeToRemove);
        strcpy(message, GREEN "Contato removido com sucesso" RESET);
        return;
    }
    while (current != NULL && strcmp(*((char**)current), nameToRemove) != 0) {
        current = *((void**)(current + 3 * sizeof(char*)));
    }
    if (current == NULL) {
        strcpy(message, RED "Contato não encontrado" RESET);
        return;
    }
    nodeToRemove = current;
    if (*((void**)(nodeToRemove + 3 * sizeof(char*) + sizeof(void*))) != NULL) {
        *((void**)(*((void**)(nodeToRemove + 3 * sizeof(char*) + sizeof(void*))) + 3 * sizeof(char*))) = *((void**)(nodeToRemove + 3 * sizeof(char*)));
    }
    if ( *((void**)(nodeToRemove + 3 * sizeof(char*))) != NULL ) {
        *((void**)(*((void**)(nodeToRemove + 3 * sizeof(char*))) + 3 * sizeof(char*) + sizeof(void*))) = *((void**)(nodeToRemove + 3 * sizeof(char*) + sizeof(void*)));
    } else {
        *head = *((void**)(nodeToRemove + 3 * sizeof(char*)));
    }
    free(*((char**)nodeToRemove));
    free(*((char**)(nodeToRemove + sizeof(char*))));
    free(*((char**)(nodeToRemove + 2 * sizeof(char*))));
    free(nodeToRemove);
    strcpy(message, GREEN "Contato removido com sucesso" RESET);
}
