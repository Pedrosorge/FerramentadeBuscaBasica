typedef struct Voc_Node{
    char *palavra;
    int word_size;
    float *vetor_TF_IDF;   
    float IDF;
    int vetor_size;
    struct Voc_Node* next;
}Voc_Node;

typedef struct {
    Voc_Node *head;
    Voc_Node *tail;
    int size;
}Voc_List;

//Protótipos das funções
void Initialize_Voc_List(Voc_List *lista);
void InsertEnd_Voc_List(Voc_List *lista, char *palavra, int TF_IDF_size);
void Print_Voc_List(Voc_List lista);
void Free_Voc_List(Voc_List *lista);

// Inicializa uma lista 
void Initialize_Voc_List(Voc_List *lista){
    lista->head = NULL;
    lista->tail = NULL;
    lista->size =0;
}

// Insere um novo nó ao final da lista
void InsertEnd_Voc_List(Voc_List *lista, char *palavra, int TF_IDF_size){
    Voc_Node *aux = (Voc_Node*)malloc(sizeof(Voc_Node));
    int count;
    //Inicializando o node
    aux->next = NULL;
    aux->IDF =0;
    aux->palavra = (char *)malloc(sizeof(char)*30);
    aux->vetor_TF_IDF = (float *)malloc(TF_IDF_size*sizeof(float));    
    if(aux->palavra==NULL || aux->vetor_TF_IDF == NULL){
        printf("Ocorreu um erro na alocação de memória");
        return;
    } 
    aux->vetor_size = TF_IDF_size;
    for(count=0;palavra[count]!='\0' && count < 30;count++){
        aux->palavra[count] = palavra[count];
    }
    aux->palavra[count] = '\0';
    aux->word_size = ++count;
    
    if(lista->tail==NULL){
        lista->head = aux;
        lista->tail = aux;
        lista->size++;
        return;
    }
    aux->next = lista->tail->next;
    lista->tail->next = aux;
    lista->tail = aux;
    lista->size++;
}

// Printa a lista
void Print_Voc_List(Voc_List lista){
    Voc_Node *aux = lista.head;
    while(aux!=NULL){
        printf("\n %s", aux->palavra);
        printf(" -> ");
        for(int i=0;i<aux->vetor_size;i++){
            printf(" %f ", aux->vetor_TF_IDF[i]);
        }
        aux=aux->next;
    }
    printf("\nNúmero de itens : %d", lista.size);
}

// Libera a memória alocada pela lista
void Free_Voc_List(Voc_List *lista){
    Voc_Node *aux= lista->head;
    Voc_Node *temp;
    while(aux!=NULL){        
        temp = aux;
        free(temp->vetor_TF_IDF);
        free(temp->palavra);
        aux = aux->next;
        free(temp);       
    }
    Initialize_Voc_List(lista);
    
}

