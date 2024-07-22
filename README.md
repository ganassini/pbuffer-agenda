# pBuffer-agenda

### DOCUMENTAÇÃO

#### Memória total alocada para o pBuffer
```c
#define 	MEMORY_SIZE 		1024
```

#### Ponteiros para "variáveis" do programa utilizando aritmética de ponteiros

```c
#define P_USEROPTION        (pBuffer)
#define P_MESSAGE           (P_USEROPTION + sizeof(int))
#define P_TMP_NODE_NAME     (P_MESSAGE + 50 * sizeof(char))
#define P_TMP_NODE_AGE      (P_TMP_NODE_NAME + 50 * sizeof(char))
#define P_TMP_NODE_EMAIL    (P_TMP_NODE_AGE + 10 * sizeof(char))
#define P_HEAD 	            (P_TMP_NODE_EMAIL + 60 * sizeof(char))
```

#### struct Node

```c
typedef struct Node {
    char* name;  
    char* age;  
    char* email;  
    struct Node* next;  
    struct Node* prev;  
} Node;
```
#### Node* createNode

```c
Node* createNode(const char* name, const char* age, const char* email) {
```
A função consiste em alocar dinamicamente espaço para o nodo e para cada ponteiro do nodo (nome, idade e email)
e copiar o conteudo do ponteiro passado no argumento para o espaço alocado e em caso de erro de alocação, 
libera todos os espaços de memória e retorna um ponteiro para o vazio.

### void insertInOrder

```c
void insertInOrder(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    char* name = (char*)P_TMP_NODE_NAME;
    char* age = (char*)P_TMP_NODE_AGE;
    char* email = (char*)P_TMP_NODE_EMAIL;
    Node_t* node = createNode(name, age, email);
    Node_t* current = *head;
```
A função recebe, por referência, a cabeça da lista e o input do usuário, cria um nodo temporário com input do usuário, define um ponteiro 
de iteração que aponta para a cabeça da lista e, se a lista estiver vazia ou o nome do contato digitado for lexicograficamente maiordo que o 
nome do primeiro contato da lista (insere substrings em ordem de tamanho), o contato digitado se torna a cabeça, caso a lista nao esteja vazia
e o nome digitado for lexicograficamente igual ou menor a zero, percorre a lista até o final, e insere o novo nodo, ou até encontrar o nodo N 
cujo proximo nodo é um nome lexicograficamente maior do que o nome digitado pelo usuário, ao encontrar, insere o nodo entre o nodo N e o nodo 
N + 1.

### void printList

```c
void printList (void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    Node_t* current = *head;
```
A função recebe, por referência, a cabeça da lista e define um ponteiro de iteração que aponta para a cabeça da lista. Enquanto o ponteiro não
apontar para o vazio, exibe os conteudos da struct Node e aponta para o próximo nodo.

### void getUserInput

```c
void getUserInput(void* pBuffer) {
    char* name = (char*)P_TMP_NODE_NAME;
    char* age = (char*)P_TMP_NODE_AGE;
    char* email = (char*)P_TMP_NODE_EMAIL;
```
A função recebe os ponteiros para o local no pBuffer reservado para o input do usuário, e copia as strings digitadas para os endereços de 
memória no pBuffer.

### void searchPerson

```c
void searchPerson(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    Node_t* current = *head;
    char* pattern = (char*)P_TMP_NODE_NAME;
    int* found = (int*)P_USEROPTION;
    char* message = (char*)P_MESSAGE;
```

A função recebe, por referência, a cabeça da lista, um ponteiro de iteração que aponta para a cabeça da lista, define um ponteiro para o 
local no pBuffer reservado para o input do usuário, um ponteiro para o local reservado para a escolha da opção no menu para servir de flag
na hora de mudar o conteudo da mensagem, e o ponteiro para a mensagem. O usuário digita o nome da ser procurado, a flag found recebe o valor
0 (nao foi achado) e começa a iterar pela lista, se o nome do contato inicia pela substring digitada pelo usuário, exibe os dados do contato
e a flag found se torna 1. Caso a flag found termine o loop em 0, exibe uma mensagem de erro no menu e caso termine em 1, exibe uma mensagem
de sucesso.

### void removeNode

```c
void removeNode(void* pBuffer) {
    Node_t** head = (Node_t**)P_HEAD;
    char* nameToRemove = (char*)P_TMP_NODE_NAME;
    char* message = (char*)P_MESSAGE;
    Node_t* current = *head;
    Node_t* nodeToRemove = NULL;
```
A função recebe, por referência, a cabeça da lista, um ponteiro de iteração, um ponteiro para indicar qual nodo deve ser excluido, o ponteiro 
da mensagem e o ponteiro do nome a ser removidoe e é solicitado ao usuário digitar o nome do contato a ser removido. Se o o contato a ser
removido for o primeiro da lista, a nova cabeça se torna o segundo da lista e o espaço na memória do nodo removido é desalocado. Caso o contato
a ser removido não for o primeiro da lista, é feita a iteração até o fim da lista, se não achar nenhuma string igual retorna uma mensagem de 
erro e se achar remove o nodo alterando os ponteiros do nodo anterior e do nodo seguinte.

### int main

```c 
int main() {
    void* pBuffer = malloc(MEMORY_SIZE);
    int* userOption = (int*)P_USEROPTION;
    char* message = (char*)P_MESSAGE;
    Node_t** head = (Node_t**)P_HEAD;
```
A função main do programa aloca memória para o pBuffer e utiliza os ponteiros para exibir a mensagem e armazenar a opção do usuário.
