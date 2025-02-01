#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
  #define CLEAR "clear"
#endif
#ifdef _WIN32
  #define CLEAR "cls"
#endif

#define RESET   					"\x1b[0m"
#define RED     					"\x1b[31m"
#define GREEN   					"\x1b[32m"

#define SIZE_PBUFFER                4096
#define SIZE_USER_OPTION       		1
#define SIZE_MESSAGE          		50
#define SIZE_NODE_NAME        		50
#define SIZE_NODE_AGE       		10
#define SIZE_NODE_EMAIL       		60
#define SIZE_NODE        			(SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL + (2 * sizeof(void*)))
#define SIZE_TMP_CURRENT            (sizeof(void*))
#define SIZE_TMP_FOUND              (sizeof(int))
#define OFFSET_USEROPTION           0
#define OFFSET_MESSAGE              (OFFSET_USEROPTION + SIZE_USER_OPTION)
#define OFFSET_TMP_NODE_NAME        (OFFSET_MESSAGE + SIZE_MESSAGE)
#define OFFSET_TMP_NODE_AGE         (OFFSET_TMP_NODE_NAME + SIZE_NODE_NAME)
#define OFFSET_TMP_NODE_EMAIL       (OFFSET_TMP_NODE_AGE + SIZE_NODE_AGE)
#define OFFSET_TMP_CURRENT          (OFFSET_TMP_NODE_EMAIL + SIZE_NODE_EMAIL)
#define OFFSET_TMP_FOUND            (OFFSET_TMP_CURRENT + SIZE_TMP_CURRENT)
#define SIZE_TMP_AREA               (OFFSET_TMP_FOUND + SIZE_TMP_FOUND)
#define P_USEROPTION                ((char*)pBuffer + OFFSET_USEROPTION)
#define P_MESSAGE                   ((char*)pBuffer + OFFSET_MESSAGE)
#define P_TMP_NODE_NAME      		((char*)pBuffer + OFFSET_TMP_NODE_NAME)       
#define P_TMP_NODE_AGE       		((char*)pBuffer + OFFSET_TMP_NODE_AGE)      
#define P_TMP_NODE_EMAIL     		((char*)pBuffer + OFFSET_TMP_NODE_EMAIL)
#define P_TMP_CURRENT        		((void**)(pBuffer + OFFSET_TMP_CURRENT))
#define P_TMP_FOUND          		((int*)(pBuffer + OFFSET_TMP_FOUND))
#define OFFSET_P_HEAD      			(SIZE_TMP_AREA)
#define OFFSET_P_NODE_FREE 			(OFFSET_P_HEAD + sizeof(void*))
#define P_HEAD             			((void**)(pBuffer + OFFSET_P_HEAD))
#define P_NODE_FREE        			((void**)(pBuffer + OFFSET_P_NODE_FREE))
#define P_NODE_AREA     			((char*)pBuffer + OFFSET_P_NODE_FREE + sizeof(void*))
#define NODE_NAME(p)   				((char*)(p))
#define NODE_AGE(p)    				((char*)(p + SIZE_NODE_NAME))
#define NODE_EMAIL(p)  				((char*)(p + SIZE_NODE_NAME + SIZE_NODE_AGE))
#define NODE_NEXT(p)   				(*((void**)(p + SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL)))
#define NODE_PREV(p)   				(*((void**)(p + SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL + sizeof(void*))))

void displayMenu(void* pBuffer);
void invalidOption(void* pBuffer);
void getUserInput(void* pBuffer);
void insertNode(void* pBuffer);
void printList(void* pBuffer);
void removeNode(void* pBuffer);
void searchPerson(void* pBuffer);
void print(void* pBuffer);

int main(void) 
{
    void* pBuffer = malloc(SIZE_PBUFFER);
    
    if (pBuffer == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *P_HEAD = NULL;
    *P_NODE_FREE = P_NODE_AREA;

    displayMenu(pBuffer);

    free(pBuffer);
    return 0;
}

void displayMenu(void* pBuffer) 
{
    system(CLEAR);
    printf("==================================\n"
           "||            AGENDA            ||\n"
           "==================================\n"
           "|| 1. Adicionar pessoa          ||\n"
           "|| 2. Remover pessoa            ||\n"
           "|| 3. Buscar pessoa             ||\n"
           "|| 4. Listar todos              ||\n"
           "|| 5. Sair                      ||\n"
           "==================================\n\n");

    if (*P_MESSAGE == '\0' || strlen(P_MESSAGE) == 0)
        printf(">> ");
    else
        printf("%s\n\n>> ", P_MESSAGE);

    scanf(" %c", P_USEROPTION);
    getchar();

    switch (*P_USEROPTION) {
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
            system(CLEAR);
            break;
        default:
            invalidOption(pBuffer);
            displayMenu(pBuffer);
            break;
    }
}

void invalidOption(void* pBuffer) 
{
    strcpy(P_MESSAGE, RED "Opção inválida." RESET);
}

void getUserInput(void* pBuffer) 
{
    printf("\nNome: ");
    scanf(" %[^\n]s", P_TMP_NODE_NAME);
    getchar();
    printf("Idade: ");
    scanf(" %[^\n]s", P_TMP_NODE_AGE);
    getchar();
    printf("Email: ");
    scanf(" %[^\n]s", P_TMP_NODE_EMAIL);
    getchar();
}

void insertNode(void* pBuffer) 
{
    strcpy(NODE_NAME(*P_NODE_FREE), P_TMP_NODE_NAME);
    NODE_NAME(*P_NODE_FREE)[SIZE_NODE_NAME - 1] = '\0';

    strcpy(NODE_AGE(*P_NODE_FREE), P_TMP_NODE_AGE);
    NODE_AGE(*P_NODE_FREE)[SIZE_NODE_AGE - 1] = '\0';

    strcpy(NODE_EMAIL(*P_NODE_FREE), P_TMP_NODE_EMAIL);
    NODE_EMAIL(*P_NODE_FREE)[SIZE_NODE_EMAIL - 1] = '\0';

    NODE_NEXT(*P_NODE_FREE) = NULL;
    NODE_PREV(*P_NODE_FREE) = NULL;

    if (*P_HEAD == NULL || strcmp(NODE_NAME(*P_HEAD), NODE_NAME(*P_NODE_FREE)) > 0) {
        NODE_NEXT(*P_NODE_FREE) = *P_HEAD;

        if (*P_HEAD != NULL)
            NODE_PREV(*P_HEAD) = *P_NODE_FREE;

        *P_HEAD = *P_NODE_FREE;
        *P_NODE_FREE += SIZE_NODE;
    } else {
        *P_TMP_CURRENT = *P_HEAD;

        while (NODE_NEXT(*P_TMP_CURRENT) != NULL &&
               strcmp(NODE_NAME(NODE_NEXT(*P_TMP_CURRENT)), P_TMP_NODE_NAME) <= 0)
        {
            *P_TMP_CURRENT = NODE_NEXT(*P_TMP_CURRENT);
        }

        NODE_NEXT(*P_NODE_FREE) = NODE_NEXT(*P_TMP_CURRENT);

        if (NODE_NEXT(*P_TMP_CURRENT) != NULL)
            NODE_PREV(NODE_NEXT(*P_TMP_CURRENT)) = *P_NODE_FREE;

        NODE_NEXT(*P_TMP_CURRENT) = *P_NODE_FREE;
        NODE_PREV(*P_NODE_FREE) = *P_TMP_CURRENT;

        *P_NODE_FREE += SIZE_NODE;
    }
    strcpy(P_MESSAGE, GREEN "Contato adicionado com sucesso." RESET);
}

void printList(void* pBuffer) 
{
    if (*P_HEAD == NULL) {
        strcpy(P_MESSAGE, RED "A lista está vazia." RESET);
        return;
    }

    *P_TMP_CURRENT = *P_HEAD;
    *P_MESSAGE = '\0';

    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");

    while (*P_TMP_CURRENT != NULL) {
        printf("Nome:  %s\nIdade: %s\nEmail: %s\n",
               NODE_NAME(*P_TMP_CURRENT),
               NODE_AGE(*P_TMP_CURRENT),
               NODE_EMAIL(*P_TMP_CURRENT));
        printf("-----------------------------------------------\n");

        *P_TMP_CURRENT = NODE_NEXT(*P_TMP_CURRENT);
    }

    printf("\nAperte ENTER para sair ");
    getchar();
}

void searchPerson(void* pBuffer) 
{
    if (*P_HEAD == NULL) {
        strcpy(P_MESSAGE, RED "A lista está vazia." RESET);
        return;
    }
    
    *P_TMP_FOUND = 0;

    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", P_TMP_NODE_NAME);
    getchar();

    system(CLEAR);
    printf("===============================================\n"
           "||                CONTATOS                   ||\n"
           "===============================================\n");

    *P_TMP_CURRENT = *P_HEAD;

    while (*P_TMP_CURRENT != NULL) {
        if (strncmp(NODE_NAME(*P_TMP_CURRENT), P_TMP_NODE_NAME, strlen(P_TMP_NODE_NAME)) == 0) {
            printf("Nome:  %s\nIdade: %s\nEmail: %s\n",
                   NODE_NAME(*P_TMP_CURRENT),
                   NODE_AGE(*P_TMP_CURRENT),
                   NODE_EMAIL(*P_TMP_CURRENT));
            printf("-----------------------------------------------\n");
            *P_TMP_FOUND = 1;
        }
        *P_TMP_CURRENT = NODE_NEXT(*P_TMP_CURRENT);
    }

    if (!(*P_TMP_FOUND))
        strcpy(P_MESSAGE, RED "Nenhum contato encontrado." RESET);
    else {
        *P_MESSAGE = '\0';
        printf("\nAperte ENTER para sair ");
        getchar();
    }
}

void removeNode(void* pBuffer) 
{
    if (*P_HEAD == NULL) {
        strcpy(P_MESSAGE, RED "A lista está vazia." RESET);
        return;
    }

    printf("\nDigite o nome: ");
    scanf(" %[^\n]s", P_TMP_NODE_NAME);
    getchar();

    *P_TMP_CURRENT = *P_HEAD;

    if (!strcmp(NODE_NAME(*P_HEAD), P_TMP_NODE_NAME)) {
        *P_HEAD = NODE_NEXT(*P_HEAD);

        if (*P_HEAD != NULL)
            NODE_PREV(*P_HEAD) = NULL;

        strcpy(P_MESSAGE, GREEN "Contato removido com sucesso." RESET);
        return;
    }

    while (*P_TMP_CURRENT != NULL && strcmp(NODE_NAME(*P_TMP_CURRENT), P_TMP_NODE_NAME))
        *P_TMP_CURRENT = NODE_NEXT(*P_TMP_CURRENT);
    
    if (*P_TMP_CURRENT == NULL) {
        strcpy(P_MESSAGE, RED "Contato não encontrado." RESET);
        return;
    }

    if (NODE_PREV(*P_TMP_CURRENT) != NULL)
        NODE_NEXT(NODE_PREV(*P_TMP_CURRENT)) = NODE_NEXT(*P_TMP_CURRENT);

    if (NODE_NEXT(*P_TMP_CURRENT) != NULL)
        NODE_PREV(NODE_NEXT(*P_TMP_CURRENT)) = NODE_PREV(*P_TMP_CURRENT);

    strcpy(P_MESSAGE, GREEN "Contato removido com sucesso." RESET);
}