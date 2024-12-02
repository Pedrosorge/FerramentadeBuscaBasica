PROJETO DO RIBAS

Esse projeto visa fazer uma ferramenta de busca que utiliza o método TF-IDF para encontrar a melhor recomendação para dada pesquisa.

A organização do projeto se dá da seguinte forma:

->Datas: Diretório com arquivos '.txt' e '.dat' estão armazenados funcionando como um "banco de dados";
    - input.txt : arquivo com alguns exemplos de entrada que podem ser testados;
    - matrix.dat : arquivo em que é guardada a matriz TF-IDF para que não tenha que ser calculada toda vez que deseja testar o código;
    - vocabulary.txt : arquivo no qual as palavras-chave que podemm ser usadas na busca estão armazenadas.

-> File_Treatment: Diretório com arquivos '.c' que estão destinas à elaboração de funções que estão relaconadas à leitura e escrita em arquivos de texto ou arquivos binários;
    - file_main.c : arquivo com as principais funções de leitura e escrita;
    - file_subfunctions.c : arquivo com funções secundárias que não possuem tanta importância para as funções em sí, sendo mais usado com o intuito de organizar o código.

-> KMP : Diretório com arquivo '.c' que está relacionado ao algoritmo de casamento de padrões KMP
    - kmp_main.c : arquivo com a função kmp e outras funções essenciais para seu funcionamento.

-> Lists : Diretório com arquivos '.c' que estão destinados ao funcionamento de todas as listas utilizadas nos demais arquivos
    - float_list.c : arquivo com funções de uma lista que tem como elemento principal um número real;
    - string_list.c : arquivo com funções de uma lista cujo principal elemento é uma string (cadeia de caracteres);
    - vocabulary_list.c : arquivo com funções relacionadas à manipulação de uma lista de informações essenciais à formação da matriz TF-IDF.

-> Search : Diretório com arquivo '.c' com funções relacionadas à ferramenta de busca;
    - search_main.c : arquivo com funções de calculos relacionados à criação e manipulação do vetor de busca.

-> textos : Diretório com arquivos '.txt' contendo os arquivos com as informações que serão buscadas pelo usuário.

-> .. : Diretório Principal: Conté, arquivos '.c', o README.txt e o arquivo executável do programa.
    - index.c : arquivo com a principal lógica de funcionamento do priojéto, sendo ele o arquivo que comandará as regras de usa do programa;
    - header.c : arquivo contendo as bibliotecas utilizadas em todo o programa;
    - README.txt : arquivo contendo informações sobre o programa;
    - ... .exe : objeto executável contendo o real programa utilizável.

Cada arquivo '.c' contém em seu início primeiramente um struct utilizado majoritariamente no arquivo (caso tenha), os protótipos das funções definidas no arqivo e as definições das funções.


INSTRUÇÕES PARA USO DO PROGRAMA:

- Ao verificar que existem arquivos no ditetório "./textos" e verificar a existencia de palavras no arquivo de caminho "./Datas/vocabulary.txt" compile o programa.

    #Compilando o programa#           

    Windows : "gcc -o teste index.c"

    Linux : "gcc -o teste index.c -lm"

    #Rodando o programa#

    Windows/Linux : "./teste"

Ao executar o programa uma interface será projetada no seu terminal. Essa interface apresentará 5 operações diferentes:

    [0] Finalizar - Finaliza o programa liberando todas as memória que foram alocadas ao decorrer dele.
    [1] Mapear Dados - Cria a matriz tf e a armazena no arquivo "./Datas/matrix.dat"
    [2] Carregar Dados - Carrega os dados da matriz armazenada no arquivo "./Datas/matrix.dat"
    [3] Buscar Artigo - Permite que o usuário execute uma busca utilizando palavras-chave que estão no arquivo "./Datas/vocabulary.txt", retornando os artigos mais relacionados com as palavras buscadas
    [4] Ver Dados (NÃO RECOMENDADOS QUANDO UTILIZADO GRANDE QUANTIDADE DE DADOS) - Permite que o usuário visualize os dados salvos na matriz do arquivo "./Datas/vocabulary.dat"

# IMPORTANTE #

Caso seja a sua primeira vez executando esse programa, recomendo que utilize os seguintes passos de operação, respectivamente:

    [1] -> [2]

Dessa forma, será certo que os dados estarão corretos.
Além disso, é importante destacar que toda vez que desejar realizar uma busca ou visualizar os dados é essencial carregá-los antes.

Aproveite o programa!!