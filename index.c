#include "header.c"

//Protótipos de funções
void Construir_Banco_de_Dados();
void Busca(Processed_Matrix Matriz, char **nome_arquivos);

// Código principal
int main(){

    Processed_Matrix Matriz;
    char **nome_arquivos = NULL;
    int flag=0;
    int operacao;
    Matriz.Linha = NULL;

    do{
        //Validação de entrada da operação
        do{
            printf("\nDigite o número da operação que deseja realizar:\n\t[0] Finalizar\n\t[1] Mapear Dados\n\t[2] Carregar Dados\n\t[3] Buscar Artigo\n\t[4] Ver Dados(Não recomendado quando utilizado grande quantidade de dados)\n>>>");
            scanf(" %d", &operacao);
        }while(operacao<0||operacao>4);
        
        switch(operacao){
            //Caso o queira encerrar o programa
            case 0: printf("\nEncerrando Programa..."); break;
            //Caso queira mapear mudanças feitas durante a execução do programa
            case 1: Construir_Banco_de_Dados(); break;
            //Caso queira importar a matriz
            case 2: 
                if(!flag){
                    Load_Matrix(&Matriz, &nome_arquivos,"./Datas/matrix.dat"); 
                    flag = 1;
                }
                else{
                    printf("\nLimpando dados...");
                    Free_Loaded_Data(&Matriz, &nome_arquivos);
                    printf("\nImportando novos dados");
                    Load_Matrix(&Matriz, &nome_arquivos,"./Datas/matrix.dat"); 
                }
                break; 
            //Realizar busca de artigos
            case 3: 
                if(!flag){
                    printf("\nCarregue a matriz primeiro!!");
                    break;
                }
                Busca(Matriz,nome_arquivos);
                break;
            case 4: 
                if(!flag){
                    printf("\nCarregue a matriz primeiro!!");
                    break;
                }
                Print_Loaded_File_Names(nome_arquivos, Matriz); 
                Print_Loaded_Matrix(Matriz); 
                break;
            default: break;
        }
    }while(operacao);
    
    Free_Loaded_Data(&Matriz, &nome_arquivos);
    printf("\nPrograma Encerrado!!");

    return 0;
}

//Realiza as operações para atualizar ou criar o arquivo de referência com a matriz TF-IDF, o nome dos arquivos e as palavras do vocabulário
void Construir_Banco_de_Dados(){
    DIR *dir = opendir(DIR_NAME);
    Lista_String arquivos;
    Voc_List vocabulario; // VOU TIRAR

    //Pega os nomes dos arquivos do diretório textos e armazena-os em uma lista
    printf("\nInicializando...");
    Initialize_String_List(&arquivos);
    listar_arquivos(dir, &arquivos);
    closedir(dir);

    //Criando a matriz TF-IDF (vocabulário X documento)
    printf("\nCriando Matriz...");
    Initialize_Voc_List(&vocabulario);
    Create_Vocabulary_Matrix(&vocabulario,"./Datas/vocabulary.txt", arquivos.size);
    
    //Calculando matriz TF-IDF
    printf("\nCalculando Matriz...");
    Calculate_Matriz_TF_IDF(&vocabulario, &arquivos);
    Save_Matrix(vocabulario, arquivos,"./Datas/matrix.dat");

    //Liberando memória usada
    Free_String_List(&arquivos);
    Free_Voc_List(&vocabulario);

    printf("\nMemórias desalocadas com sucesso!!");

} 

// Algoritmo para realizar a busca de textos em arquivos
void Busca(Processed_Matrix Matriz, char **nome_arquivos){
    char *query = (char*)malloc(sizeof(char)*QUERY_MAX);
    char buff[RECOMENDATION_MAX_DIGITS];
    int num_rec=-1,i;
    float *vetor_busca, *vetor_similaridade;
    Float_List fila;
    Initialize_Float_List(&fila);

    //Verifica a alocação de memória
    if(query ==NULL){
        printf("não foi possível alocar memória!!");
        return;
    }

    printf("\nDigite as palavras chaves da busca:\n");
    scanf(" %[^\n]s", query);
    strcat(query," ");

    // Coleta o número de recomendações caso haja
    for(i=0; i<4;i++){
        if(query[i]==' '){
            buff[i]='\0';
            break;
        }
        else{
            if(query[i]>='0' && query[i]<='9'){
                buff[i] = query[i];
            }
            else{
                buff[i] = '\0';
                break;
            }
        }
    }
    if(i!=0){num_rec = atoi(buff);}

    //Realiza os calculos e busca usando a query digitada
    vetor_busca = Calculate_Search_Query(&query[i],Matriz);
    vetor_similaridade = Calculate_Similarity_Vector(Matriz,vetor_busca);
    for(int i =0;i<Matriz.num_colunas;i++){
        Insert(&fila,vetor_similaridade[i],i);
    }
    Sort_Float_List(&fila);
    Print_Titles(fila,nome_arquivos,num_rec);
    
    //Desalocamento de memórias locais
    free(vetor_busca);
    free(vetor_similaridade);
    Free_Float_List(&fila);
    free(query);
}


