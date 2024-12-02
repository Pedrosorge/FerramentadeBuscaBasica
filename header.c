// Bibliotecas externas
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
// Variáveis constantes
#define DIR_NAME "./textos"
#define DIR_VOCABULARIO "./Datas/vocabulary.txt"
#define BUFF_PATH_SIZE 270 // Tamanho do buffer que armazenará o caminho até o arquivo com o texto
#define QUERY_MAX 100 // Tamanho máximo da query de busca
#define RECOMENDATION_MAX_DIGITS 4 // Máximo de digitos que o número de recomendações requisitadas pode ter
#define MAX_SIZE_WORD_VOCABULARY 30 // Tamanho máximo que uma palavra do vocabulário pode ter
// importações de arquivos criados
#include "./Lists/vocabulary_list.c"
#include "./Lists/string_list.c"
#include "./Lists/float_list.c"
#include "./KMP/kmp_main.c"
#include "./File_Treatment/file_main.c"
#include "./Search/search_main.c"
