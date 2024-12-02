//Prot[otipos das funções
int ler_trecho(FILE *file_ptr, char **buffer);
int num_words(FILE *arquivo);
int frequencia_palavra_no_arquivo(char *palavra, int size_palavra, FILE *arquivo);
float TF(int t, int d);

// Armazena no buff a string armazenada e retorna o numero de caracteres na string armazenada no buff
int ler_trecho(FILE *file_ptr, char **buffer){
    int num_lidos,count=0;
    if(file_ptr == NULL){
        printf("\nArquivo não existente!!");
        return 0;
    }
    (*buffer) = (char*)malloc(sizeof(char)*500);
    num_lidos = fread(*buffer, sizeof(char), 500, file_ptr);

    while((*buffer)[num_lidos]!=' '){
        num_lidos--;
        count++;
    }
    (*buffer)[num_lidos] = '\0';
    fseek(file_ptr, -count ,SEEK_CUR);// Para a próxima leitura estar mais correta.

    return num_lidos;   
}

// Retorna o número de palavras em um arquivo
int num_words(FILE *arquivo){
    int flag = 0; 
    int count = 0;
    char caractere;
    rewind(arquivo);
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (isspace(caractere) || caractere == '.' || caractere == ':' || caractere == '!' || caractere == '?' || caractere == ',') {
            flag = 0;
        } else if (flag == 0) {
            flag = 1; 
            count++;
        }
    }
    return count;
}

int frequencia_palavra_no_arquivo(char *palavra, int size_palavra, FILE *arquivo){
    char *buffer;
    int freq=0, size_text;
    rewind(arquivo);
    while((size_text = ler_trecho(arquivo, &buffer))){
        freq += kmp(palavra,buffer,size_palavra,size_text);
    }
    return freq;
}

float TF(int t, int d){
    if(d == 0){
        return 0;
    }
    return (float) t/d;
}
