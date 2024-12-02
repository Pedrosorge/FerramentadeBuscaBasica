float * Calculate_Search_Query(char *query, Processed_Matrix matriz);
void Calculate_Word_TF_IDF(char *palavra,int size_palavra,float **vetor_query, Processed_Matrix matriz, char *query, int size_query);
float * Calculate_Similarity_Vector(Processed_Matrix matriz, float *query);
void Print_Titles(Float_List melhores_artigos,char **nome_arquivos, int quantidade_impressoes);
int strcmp_sem_acentuacao(char * palavra_1, char *palavra_2);
char tira_acento(char letra);

//Calcula o vetor TF-IDF da query de busca
float * Calculate_Search_Query(char *query, Processed_Matrix matriz){
    float *vetor_query = (float *)calloc(matriz.num_linhas,sizeof(float));
    char *buff = (char *)malloc(sizeof(char)*100);
    int flag = 1, count =0, quant=5, words=0;
    int size_query=strlen(query);

    //Verificando alocações de memória
    if (vetor_query == NULL) {
        perror("Failed to allocate memory for vetor_query");
        return NULL;
    }
    if (buff == NULL) {
        perror("Failed to allocate memory for vetor_query");
        return NULL;
    }

    for(int i=0;query[i]!='\0';i++){
        if(query[i] != ' '){
            buff[count++] = tolower(query[i]);
        }
        else{
            if(count){
                words++;
                buff[count] = '\0';
                Calculate_Word_TF_IDF(buff,count, &vetor_query, matriz, query, size_query);
                count = 0;
            }
        }
    }
    free(buff);
    return vetor_query;
}

//Calcula o TF de cada palavra da query de busca
void Calculate_Word_TF_IDF(char *palavra,int size_palavra,float **vetor_query, Processed_Matrix matriz, char *query, int size_query){
    int  size_padrao, size_texto, i;
    for(i=0;i<matriz.num_linhas;i++){
        if(strcmp_sem_acentuacao(palavra, matriz.Linha[i].palavra)==1){
           (*vetor_query)[i] = kmp(palavra,query,size_palavra,size_query)*matriz.Linha[i].idf;
            return;
        }
    }
}

//Calcula o vetor com a similaridade do vetor de busca em comparação com cada arquivo
float * Calculate_Similarity_Vector(Processed_Matrix matriz, float *query){
    float *similarity_vec = (float*)calloc(sizeof(float),matriz.num_colunas);
    float modulo_vetor_A=0, modulo_vetor_B=0, produto_interno=0;

    if(similarity_vec == NULL){
        printf("Não foi possível alocar memória!!");
        return NULL;
    }
    for(int i=0;i<matriz.num_colunas;i++){
        for(int j=0;j<matriz.num_linhas;j++){
            produto_interno += matriz.Linha[j].tf_idf[i] * query[j];
            modulo_vetor_A += pow(matriz.Linha[j].tf_idf[i],2);
            modulo_vetor_B += pow(query[j],2);
        }
        if(sqrt(modulo_vetor_A) * sqrt(modulo_vetor_B) == 0 || modulo_vetor_A <0 || modulo_vetor_B <0){
            similarity_vec[i] = 0;
        }
        else{
            similarity_vec[i] = produto_interno/(sqrt(modulo_vetor_A)*sqrt(modulo_vetor_B));
        }
        modulo_vetor_A=0;
        modulo_vetor_B=0;
        produto_interno=0;
    }
    return similarity_vec;
}

//Imprime o nome dos artigos mais recomendados 
void Print_Titles(Float_List melhores_artigos,char **nome_arquivos, int quantidade_impressoes){
    FILE * arq;
    char buff[200];
    int buff_i, num=1;
    Node_Float *aux = melhores_artigos.head;
    if(quantidade_impressoes == -1){ quantidade_impressoes = 5;}//Por default mostrarei os 5 melhores artigos para sua busca  
    while(aux!=NULL && quantidade_impressoes>0){
        arq = fopen(nome_arquivos[aux->posicao_arquivo],"r");
        buff_i=0;
        while((buff[buff_i++] = fgetc(arq))!='\n');
        buff[buff_i] = '\0';
        printf("\nCaminho: %s [%f]", nome_arquivos[aux->posicao_arquivo], aux->value);
        printf("\n%d - %s", num, buff);
        aux = aux->next;
        num++;
        quantidade_impressoes--;
        fclose(arq);
    }
}

// Compara duas string sem considerar a acentuação delas
int strcmp_sem_acentuacao(char * palavra_1, char *palavra_2){

    int size_palavra_1 = strlen(palavra_1); 
    int size_palavra_2 = strlen(palavra_2);
    if(size_palavra_1 != size_palavra_2){
        return 0;
    }
    for(int  i =0; palavra_1[i]!='\0';i++){
        if(tira_acento(palavra_1[i]) != tira_acento(palavra_2[i])){
            return 0;
        }
    }
    return 1;
}

// Retira a cacentuação de uma letra
char tira_acento(char letra){
    char acento[] = "áéíóãõç";
    char sem_acento[] = "aeioaoc";

    for(int i=0;acento[i]!='\0';i++){
        if(acento[i] == tolower(letra)){
            return sem_acento[i];
        }
    }
    return letra;
}