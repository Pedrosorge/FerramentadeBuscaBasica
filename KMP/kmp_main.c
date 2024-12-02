//Protótipos das funções
int kmp(char *pattern, char *text, int size_pattern, int size_text);
int * preprocess(char *pattern, int size);
int verify_miss_of_letter(char *pattern, char *text, int size_pattern, int index_pattern);

//Realiza o casamento de padrões procurando um padão de caracteres em um dado texto
int kmp(char *pattern, char *text, int size_pattern, int size_text){

    int count=0; 
    // Cria o lps
    int *lps = preprocess(pattern, size_pattern);
    int i=0, j=0, error_pos=-1;

    while(i<size_text){
        if(tolower(text[i])==tolower(pattern[j])){
            i++;
            j++;
        }
        else{
            //Testes de tolerância de erro
            if(error_pos==-1){
                if((i+1<size_text && verify_miss_of_letter(pattern, &text[i+1], size_pattern, j))
                || (i+1 < size_text && j+1 < size_pattern && verify_miss_of_letter(pattern, &text[i+1], size_pattern, j+1))
                || (j+1 < size_pattern && verify_miss_of_letter(pattern, &text[i], size_pattern, j+1))){
                    error_pos=i;
                    count++;
                    i++;
                }
            }
            //Fim dos testes
            if(j!=0){
                j=lps[j-1];
            }
            else{
                i++;
            }
        }
        //Contabiliza a contagem do kmp
        if((j==size_pattern || j == size_pattern-1) && error_pos==-1){
                count++;
                error_pos = j;
                j=lps[j-1];
        }
        //Considera a posição do ultimo erro considerado fazendo com que nenhuma outra palavra semelhante seja encontrada em uma distância do tamanho do padrão(palavra que deseja procurar) -1
        if(error_pos!=-1 && i-error_pos > size_pattern-1){
            error_pos  =-1;
        }
        // printf("\ni: %d  j: %d err_pos: %d", i, j, error_pos); //DEBUG
   }

    return count;//Retorna o número de vezes que o padrão aparece no texto 
}

// Pré processa as palavras que serão procuradas no texto para que o kmp funcione. Cria o lps(longest prefix sulfix).
int * preprocess(char *pattern, int size){

    int *lps = (int *)calloc(size,sizeof(int));
    int i =1;
    int len =0;
    lps[0] = 0;
    while(i<size){
        if(pattern[i]==pattern[len]){
            len++;
            lps[i]=len;
            i++;
        }
        else{
            if(len>0){
                len=lps[len-1];
            }
            else{
                lps[i]=0;
                i++;
            }
        }
    }

    return lps;// Retorna o ponteiro de um vetor com o lps de cada posição do padrão que deseja procurar
}

// Verificação com tolerância da falta de uma letra no meio da palavra
int verify_miss_of_letter(char *pattern, char *text, int size_pattern, int index_pattern){
    int i=0;
    while(index_pattern<size_pattern && text[i]!='\0'){
        if(pattern[index_pattern]!=text[i]){
            return 0;
        }
        index_pattern++;
        i++;
    }
    return 1;

}

