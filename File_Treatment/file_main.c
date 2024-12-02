#include "./file_subfunctions.c"

//Struct
typedef struct{
    float idf;
    char *palavra;
    float *tf_idf;
}Processed_Line;

typedef struct{
    Processed_Line *Linha;
    int num_linhas;
    int num_colunas;
}Processed_Matrix;

//Protótipos das funções
void listar_arquivos(DIR *diretorio, Lista_String *buff);
void Create_Vocabulary_Matrix(Voc_List *vocabulario, char * path_vocabulary, int num_documentos);
void Calculate_Matriz_TF_IDF(Voc_List *matriz, Lista_String *arquivos);
void Save_Matrix(Voc_List matriz, Lista_String nome_arquivos, char *caminho_arquivo);
void Load_Matrix(Processed_Matrix *matriz, char ***nome_arquivos, char * caminho);
void Print_Loaded_File_Names(char **matriz, Processed_Matrix m);
void Print_Loaded_Matrix(Processed_Matrix m);
void Free_Loaded_Data(Processed_Matrix *matriz, char ***arquivos);

// Retorna um nome do arquivo do diretório
void listar_arquivos(DIR *diretorio, Lista_String *buff){
    struct dirent *entrada;
    if (diretorio == NULL) {
        perror("Não foi possível abrir o diretório");
        return;
    }
    rewinddir(diretorio);
    while((entrada = readdir(diretorio)) != NULL){
        if(entrada->d_name[0] == '.'){
            continue;
        }
        // Construir o caminho completo até o arquivo
        char *path=malloc(sizeof(char)*BUFF_PATH_SIZE);
        snprintf(path, BUFF_PATH_SIZE,"%s/%s", DIR_NAME ,entrada->d_name);// Mudar tamanho de 50 para 270 ou mais OBS, mudar também no insert End_string
        InsertEnd_String_List(buff, path);
        free(path);
    }
}

// Cria matriz vocabulário
void Create_Vocabulary_Matrix(Voc_List *vocabulario, char * path_vocabulary, int num_documentos){
    FILE *arquivo  = fopen(path_vocabulary,"r");
    if(arquivo == NULL){
        printf("\nArquivo inexistente !!");
        return;
    }
    char *buff = (char *)malloc(sizeof(char)*MAX_SIZE_WORD_VOCABULARY);
    int count = 0;
    char aux;
    while((aux = fgetc(arquivo))!=EOF){
        if(aux == '\n'){
            buff[count] = '\0';
            InsertEnd_Voc_List(vocabulario, buff, num_documentos);
            count=0;
        }else{
            buff[count++] = aux;
        }
    }
    buff[count] = '\0';
    InsertEnd_Voc_List(vocabulario, buff, num_documentos);
    fclose(arquivo);
    free(buff);
}

// Calcular matriz TF-IDF
void Calculate_Matriz_TF_IDF(Voc_List *matriz, Lista_String *arquivos){
    int palavras_total=0;
    int palavras_local;
    int coluna=0;
    FILE *arq;
    Node_String *aux = arquivos->head;
    Voc_Node *temp;
    while(aux!=NULL){// Percorre todos os arquivos
        arq = fopen(aux->word,"r");
        if(arq == NULL){
            printf("Arquivo inexistente!!"); 
            return;
        }
        palavras_local = num_words(arq);
        palavras_total += palavras_local;
        temp = matriz->head;
        while(temp!=NULL){// Percorre todas as palavras do vocabulário 
            temp->vetor_TF_IDF[coluna] = TF(frequencia_palavra_no_arquivo(temp->palavra, temp->word_size, arq), palavras_local);
            if(temp->vetor_TF_IDF[coluna] >0){
                temp->IDF++;// Usarei o IDF nesse momento para ver em quantos arquivos essa plavra aparece
            }
            temp = temp->next;
        }
        fclose(arq);
        coluna++;
        aux = aux->next;
    }
    // Agora temos uma matriz apenas com o TF, temos que calcular a TF-IDF
    temp = matriz->head;
    while(temp!=NULL){
        temp->IDF = log10(arquivos->size/(temp->IDF+1));//Calculo o IDF Real
        if(temp->IDF< 0){
            temp->IDF = -temp->IDF;
        }
        for(int i=0;i<temp->vetor_size;i++){
            temp->vetor_TF_IDF[i] = temp->vetor_TF_IDF[i]* temp->IDF;
        }
        temp = temp->next;
    }
}

//Salva a matriz TD-IDF em um arquivo binário
void Save_Matrix(Voc_List matriz, Lista_String nome_arquivos, char *caminho_arquivo){
    FILE *arq = fopen(caminho_arquivo,"wb");
    Voc_Node  *aux = matriz.head;
    Node_String *temp = nome_arquivos.head;

    //Salva os nomes dos arquivos
    fwrite(&(nome_arquivos.size),sizeof(int),1,arq);
    while(temp!=NULL){
        fwrite(temp->word,sizeof(char),BUFF_PATH_SIZE,arq);
        temp=temp->next;
    }

    //Salva matriz
    fwrite(&(matriz.size),sizeof(int),1,arq);
    while(aux!=NULL){
        fwrite(aux->palavra, sizeof(char),MAX_SIZE_WORD_VOCABULARY,arq);
        fwrite(aux->vetor_TF_IDF, sizeof(float),aux->vetor_size,arq);
        fwrite(&(aux->IDF), sizeof(float),1,arq);
        aux=aux->next;
    }
    fclose(arq);
    printf("\nMatriz salva com sucesso!!");
}

// Carregar matriz salva no arquivo 
void Load_Matrix(Processed_Matrix *matriz, char ***nome_arquivos, char * caminho){
    FILE *arq = fopen(caminho,"rb");
    if(arq == NULL){
        printf("\nNão foi possível abrir o arquivo!!");
        return;
    }
    fread(&(matriz->num_colunas),sizeof(int),1,arq);

    //Alocando matriz com os nomes dos arquivos;
    *nome_arquivos = (char**)malloc(sizeof(char*)*matriz->num_colunas);
    for(int i=0;i<matriz->num_colunas;i++){
        (*nome_arquivos)[i]= (char*)malloc(sizeof(char)*BUFF_PATH_SIZE);
        if((*nome_arquivos)[i]==NULL){
            printf("Falha na alocação de memória!!");
            return;
        }
        fread((*nome_arquivos)[i],sizeof(char), BUFF_PATH_SIZE,arq);//Carreaga nome do arquivo 
    }

    fread(&(matriz->num_linhas),sizeof(int),1,arq);

    //Alocando matriz TF-IDF
    matriz->Linha = (Processed_Line*)malloc(sizeof(Processed_Line)*matriz->num_linhas);
    if(matriz->Linha ==NULL){
        printf("\nErro na alocação de memória!!Não foi possível carregar os dados!!");
        return;
    }
    for(int i=0;i<matriz->num_linhas;i++){
        matriz->Linha[i].palavra  = (char*)malloc(sizeof(char)*MAX_SIZE_WORD_VOCABULARY);
        matriz->Linha[i].tf_idf = (float*)malloc(sizeof(float)*matriz->num_colunas);
        fread(matriz->Linha[i].palavra,sizeof(char),MAX_SIZE_WORD_VOCABULARY,arq);
        fread(matriz->Linha[i].tf_idf,sizeof(float),matriz->num_colunas,arq);
        fread(&(matriz->Linha[i].idf),sizeof(float),1,arq);
    }
    printf("\nMatriz carregada com sucesso!!");
    fclose(arq);
}

//Printa a matriz com o nome dos arquivos que foram salvos na criação da base de dados
void Print_Loaded_File_Names(char **matriz, Processed_Matrix m){
    for(int i=0;i<m.num_colunas;i++){
        printf("\t%s",matriz[i]);
    }
}       

//Printa a matriz processada no terminal
void Print_Loaded_Matrix(Processed_Matrix m){
    printf("\n%d X %d ", m.num_linhas, m.num_colunas);
    for(int i=0; i<m.num_linhas;i++){
        printf("\n%s [%f] ->", m.Linha[i].palavra, m.Linha[i].idf);
        for(int j=0;j<m.num_colunas; j++){
            printf(" %f ",m.Linha[i].tf_idf[j]);
        } 
    }
}

//Liberar memória alocada pelos carregamentos
void Free_Loaded_Data(Processed_Matrix *matriz, char ***arquivos) {
    if (arquivos != NULL) {
        for (int i = 0; i < matriz->num_colunas; i++) {
            free((*arquivos)[i]); 
        }
        free(*arquivos);
        *arquivos = NULL;
        printf("\nNome dos arquivos desalocados com sucesso!!!");
    }

    if (matriz != NULL && matriz->Linha != NULL) {
        for (int i = 0; i < matriz->num_linhas; i++) {
            free(matriz->Linha[i].palavra);
            free(matriz->Linha[i].tf_idf);
        }
        free(matriz->Linha);  // Libera a estrutura Linha
        matriz->Linha = NULL; // Opcional: evita acesso a ponteiro liberado
        printf("\nMemórias desalocadas com sucesso!!!");
    }
}