//Struct
typedef struct Node_Float{
    float value;
    int posicao_arquivo;
    struct Node_Float *next;
}Node_Float;

typedef struct{
    Node_Float *head;
    Node_Float *tail;
}Float_List;

//Protótipos das Funções
void Initialize_Float_List(Float_List *lista);
void Insert(Float_List *lista, float num, int pos);
Node_Float *Remove(Float_List *lista);
void Free_Float_List(Float_List *lista);
void Sort_Float_List(Float_List *lista);

//Inicializa a lista
void Initialize_Float_List(Float_List *lista){
    lista->head = NULL;
    lista->tail = NULL;
}

//Insere um valor no final da lista
void Insert(Float_List *lista, float num, int pos){
    Node_Float *aux = (Node_Float*)malloc(sizeof(Node_Float));
    aux->value = num;
    aux->next = NULL;
    aux->posicao_arquivo = pos;
    if(lista->head == NULL){
        lista->head = aux;
        lista->tail = aux;
        return;
    }
    lista->tail->next = aux;
    lista->tail = aux;
}

//Retira o valor do inicio da lista e o retorna
Node_Float * Remove(Float_List *lista){
    Node_Float *valor = lista->head;
    Node_Float *aux;
    if(lista->head == NULL){
        return valor;
    }
    aux=lista->head;
    lista->head = lista->head->next;
    if(lista->head == NULL){
        lista->tail = NULL;
    }
    return valor;
}

//Desaloca toda a lista da memória
void Free_Float_List(Float_List *lista){
    Node_Float *aux, *temp;
    aux=lista->head;
    while(aux!=NULL){
        temp = aux;
        aux=aux->next;
        free(temp);
    }
}

//Retorna o valor do primeiro elemento da lista
float Front_Float_List(Float_List lista){
    if(lista.head==NULL){
        return -1;
    }
    return lista.head->value;
}

//Ordena lista usando apenas a estrutura do tipo fila
void Sort_Float_List(Float_List *lista){    
    if(lista->head == NULL){
        return;
    }
    Float_List *Sorted = malloc(sizeof(Float_List)); 
    Float_List Auxiliar;
    Node_Float *temp, *buff;
    Initialize_Float_List(Sorted);
    Initialize_Float_List(&Auxiliar);
    
    //Faz uma primeira inserção para ter uma base de onde começar a lógica
    temp = Remove(lista);
    Insert(Sorted, temp->value, temp->posicao_arquivo);

    while(lista->head!=NULL){
        temp = Remove(lista);
        //Se for menor que o úimo valor insere no final da fila
        if(temp->value < Sorted->tail->value){
            Insert(Sorted, temp->value, temp->posicao_arquivo);
        }
        else{
            //Desenfilero até encontrar o primeiro valor que é menor ou igual ao valor que será adicionado
            while(Front_Float_List(*Sorted) > temp->value){
                buff = Remove(Sorted);
                Insert(&Auxiliar, buff->value, buff->posicao_arquivo);
            }
            //Insiro o valor no final da fila Auxiliar
            Insert(&Auxiliar, temp->value, temp->posicao_arquivo);
            //Descarrego os elementos da fila Sorted na fila Auxiliar
            while(Sorted->head!=NULL){
                buff = Remove(Sorted);
                Insert(&Auxiliar, buff->value, buff->posicao_arquivo);
            }
            // Recoloco os valores na fila sorted
            while(Auxiliar.head!=NULL){
                buff = Remove(&Auxiliar);
                Insert(Sorted, buff->value, buff->posicao_arquivo);
            }
        }   
    }
    Free_Float_List(lista);
    lista->head = Sorted->head;
    lista->tail = Sorted->tail;

}
