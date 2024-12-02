typedef struct Node_String{
    char *word;
    struct Node_String *next;
}Node_String;

typedef struct{
    Node_String *head;
    Node_String *tail;
    int size;
}Lista_String;

//Protótipos das funções
void Initialize_String_List(Lista_String *lista);
void InsertEnd_String_List(Lista_String *lista, char *palavra);
void Print_String_List(Lista_String lista);
void Free_String_List(Lista_String *lista);

// Inicializa a lista
void Initialize_String_List(Lista_String *lista){
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
}

//Insere um elemento no final da lista
void InsertEnd_String_List(Lista_String *lista, char *palavra){
    Node_String *aux = (Node_String*)malloc(sizeof(Node_String));
    char * w_aux = (char *)malloc(sizeof(char)*BUFF_PATH_SIZE);
    strcpy(w_aux,palavra);
    aux->word = w_aux;
    aux->next = NULL;
    if(lista->tail == NULL){
        lista->head = aux;
        lista->tail = aux;
        lista->size++;
        return;
    }
    lista->tail->next = aux;
    lista->tail = aux;
    lista->size++;
}

// Imprime os elementos da lista no terminal
void Print_String_List(Lista_String lista){
    Node_String *aux = lista.head;
    printf("\n");
    while(aux!=NULL){
        printf("%s", aux->word);
        if(aux->next!=NULL){
            printf(" -> ");
        }
        aux = aux->next;
    }
    printf("\nA lista tem %d elementos", lista.size);
}

// Libera a memória alocada pela lista
void Free_String_List(Lista_String *lista){
    Node_String *aux = lista->head, *temp;
    while(aux!=NULL){
        temp = aux;
        free(aux->word);
        aux=aux->next;
        free(temp);
    }
    Initialize_String_List(lista);
}