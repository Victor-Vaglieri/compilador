
// Pedro Nomura Picchioni - 10401616
// Victor Vaglieri de Oliveira - 10400787

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int linha = 1; //contador da linha
char *buffer = NULL; // string para o futuro arquivo
int linhas_de_comentario = 0; // variavel para compensar comentarios de varias linhas


typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    COMENTARIO,
    AND,
    BEGIN, 
    BOOLE, // mudanca de BOOL para BOOLE para não confundir com a palavra reservada "bool"
    ELIF, 
    END, 
    FALSEe, // mudanca de FALSE para FALSEe para não confundir com a palavra reservada "false"
    FOR, 
    IF, 
    INTEGER, 
    NOT, 
    OF, 
    OR, 
    PROGRAM, 
    READ, 
    SET, 
    TO, 
    TRUEe, 
    WRITE,
    MAIS,
    MENOS,
    VEZES,
    DIVIDE,
    MAIOR_Q,
    MENOR_Q,
    MAIOR_IGUAL_Q,
    MENOR_IGUAL_Q,
    IGUAL,
    DIFERENTE,
    PONTO,
    VIRGULA,
    PONTO_VIRGULA,
    D_PONTOS,
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    EOS
}TAtomo;

char *msgAtomo[] = {
    "erro",
    "identificador",
    "numero",
    "comentario",
    "and",
    "begin", 
    "boolean", 
    "elif", 
    "end", 
    "false", 
    "for", 
    "if", 
    "integer", 
    "not", 
    "of", 
    "or", 
    "program", 
    "read", 
    "set", 
    "to", 
    "true", 
    "write",
    "soma",
    "subtrai",
    "multiplica",
    "divide",
    "maior que",
    "menor que",
    "maior ou igual que",
    "menor ou igual que",
    "igual",
    "diferente",
    "ponto",
    "virgula",
    "ponto e virgula",
    "dois pontos",
    "abre parenteses",
    "fecha parenteses",
    "EOS"
};
typedef struct{
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[16];
}TInfoAtomo;

TInfoAtomo lookahead;

// -------------------------------------------------------------------------------------------------------
// função que converte string contendo um binario em numero inteiro

int bin_to_int(char *bin_str) {
    bin_str += 2;
    int result = 0;
    while (*bin_str) {
        result = result * 2 + (*bin_str - '0'); // Converte o caractere binário para inteiro
        bin_str++;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------------
// função que recebe um arquivo, le o arquivo inteiro e retorna uma string

char *ler_arquivo(FILE *arquivo) {
    if (arquivo == NULL) {
        printf("O ponteiro para o arquivo é NULL.\n");
        return NULL;
    }

    char *buffer = NULL;
    size_t tamanho = 0;
    char temp_buffer[1024];

    while (fgets(temp_buffer, sizeof(temp_buffer), arquivo) != NULL) {
        size_t temp_len = strlen(temp_buffer);
        char *temp = realloc(buffer, tamanho + temp_len + 1);
        if (temp == NULL) {
            printf("Erro ao alocar memória.\n");
            free(buffer); 
            return NULL;
        }
        buffer = temp;
        strcpy(buffer + tamanho, temp_buffer);
        tamanho += temp_len;
    }

    if (buffer != NULL) {
        buffer[tamanho] = '\0'; 
    }

    return buffer;
}


// -------------------------------------------------------------------------------------------------------
// função que retorna a estrutura TInfoAtomo ou para ao achar um erro

TInfoAtomo reconhece_id(){
    TInfoAtomo lookahead;
    lookahead.atomo = ERRO;
    char *iniID = buffer;
    // ------------------------------------------------------------------------------------ estado inicial 
    if(islower(*buffer)){ // se o caractere for letra minuscula
        buffer++;
        goto q2;
    } else if (*buffer == '0'){ // se começar com 0 possivel que indique numero
        buffer++;
        if(*buffer == 'b'){ // se o proximo de 0 for b vem um binario
            buffer++;
            goto q3;
        } else {
            printf("ERRO Lexico, esperado caractere [b], encontrado [%c]\n",*buffer);
            exit(0);
        }
    } else if (*buffer == '+'){ //  se o caractere for o simbolo de adição define atomo MAIS
        lookahead.atomo = MAIS;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    } else if (*buffer == '-'){ // se o caractere for o simbolo de subtração define atomo MENOS
        lookahead.atomo = MENOS;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == '*'){ // se o caractere for o simbolo de multiplicação define atomo VEZES
        lookahead.atomo = VEZES;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == ','){ //  se o caractere for virgula define atomo VIRGULA
        lookahead.atomo = VIRGULA;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == '.'){ //  se o caractere for ponto define atomo PONTO
        lookahead.atomo = PONTO;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == ';'){ // se o caractere for "ponto e virgula" define atomo PONTO_VIRGULA
        lookahead.atomo = PONTO_VIRGULA;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == ':'){ // se o caractere for "dois pontos" define atomo D_PONTOS
        lookahead.atomo = D_PONTOS;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == '('){ // se o caractere for "abre parenteses" define atomo ABRE_PARENTESES
        lookahead.atomo = ABRE_PARENTESES;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == ')'){ // se o caractere for "fecha parenteses" define atomo FECHA_PARENTESES
        lookahead.atomo = FECHA_PARENTESES;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == '='){ // se o caractere for o simbolo de igualdade define atomo IGUAL
        lookahead.atomo = IGUAL;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        buffer++;
        return lookahead;
    }else if (*buffer == '/'){ // se o caractere for o simbolo de divisão pode ser dois atomos DIFERENTE ou DIVIDE
        buffer++;
        if (*buffer == '='){ // caso o proximo simbolo for de igualdade indica que é um atomo DIFERENTE
            lookahead.atomo = DIFERENTE;
            if (buffer > iniID) {
                strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
            }
            lookahead.atributo_ID[buffer-iniID] = '\0';
            buffer++;
            return lookahead;
        }
        lookahead.atomo = DIVIDE;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        return lookahead;
    }else if (*buffer == '<'){ // se o caractere for o simbolo de "menor que" pode ser dois atomos MENOR_IGUAL_Q ou MENOR_Q
        buffer++;
        if (*buffer == '='){ // caso o proximo simbolo for de igualdade indica que é um atomo MENOR_IGUAL_Q
            lookahead.atomo = MENOR_IGUAL_Q;
            if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
            lookahead.atributo_ID[buffer-iniID] = '\0';
            buffer++;
            return lookahead;
        }
        lookahead.atomo = MENOR_Q;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        return lookahead;
    }else if (*buffer == '>'){ // se o caractere for o simbolo de "maior que" pode ser dois atomos MAIOR_IGUAL_Q ou MAIOR_Q
        buffer++;
        if (*buffer == '='){ // caso o proximo simbolo for de igualdade indica que é um atomo MAIOR_IGUAL_Q
            lookahead.atomo = MAIOR_IGUAL_Q;
            if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
            lookahead.atributo_ID[buffer-iniID] = '\0';
            buffer++;
            return lookahead;
        }
        lookahead.atomo = MAIOR_Q;
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        return lookahead;
    }else{ // --------------------------------------------------------------------- provalvel comentario
        goto q4;
    }


    // identificadores -----------------------------------------------------------------------------------
    // ver se é identificador ou comando
    q2:
        if(islower(*buffer) || isdigit(*buffer) || *buffer == '_'){
            buffer++;
            goto q2;
        }
        if (buffer-iniID > 15){ // ver condição pra dar erro
            printf("ERRO Lexico, Token com mais de 15 caracteres\n");
            exit(0);
        }
        if (isupper(*buffer)){
            printf("ERRO Lexico, caractere [%c] em caixa alta\n",*buffer);
            exit(0);
        }
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';

        // caso encontre uma string que indique uma Palavra reservada muda o tipo de atomo
        if (strcmp(lookahead.atributo_ID,"program") == 0){
            lookahead.atomo = PROGRAM;
        } else if (strcmp(lookahead.atributo_ID,"and") == 0){ 
            lookahead.atomo = AND;
        } else if (strcmp(lookahead.atributo_ID,"begin") == 0){ 
            lookahead.atomo = BEGIN;
        } else if (strcmp(lookahead.atributo_ID,"boolean") == 0){ 
            lookahead.atomo = BOOLE;
        } else if (strcmp(lookahead.atributo_ID,"elif") == 0){ 
            lookahead.atomo = ELIF;
        } else if (strcmp(lookahead.atributo_ID,"end") == 0){ 
            lookahead.atomo = END;
        } else if (strcmp(lookahead.atributo_ID,"false") == 0){ 
            lookahead.atomo = FALSEe;
        } else if (strcmp(lookahead.atributo_ID,"for") == 0){ 
            lookahead.atomo = FOR;
        } else if (strcmp(lookahead.atributo_ID,"if") == 0){ 
            lookahead.atomo = IF;
        } else if (strcmp(lookahead.atributo_ID,"integer") == 0){ 
            lookahead.atomo = INTEGER;
        } else if (strcmp(lookahead.atributo_ID,"not") == 0){ 
            lookahead.atomo = NOT;
        } else if (strcmp(lookahead.atributo_ID,"of") == 0){ 
            lookahead.atomo = OF;
        } else if (strcmp(lookahead.atributo_ID,"or") == 0){ 
            lookahead.atomo = OR;
        } else if (strcmp(lookahead.atributo_ID,"read") == 0){ 
            lookahead.atomo = READ;
        } else if (strcmp(lookahead.atributo_ID,"set") == 0){ 
            lookahead.atomo = SET;
        } else if (strcmp(lookahead.atributo_ID,"to") == 0){ 
            lookahead.atomo = TO;
        } else if (strcmp(lookahead.atributo_ID,"true") == 0){ 
            lookahead.atomo = TRUEe;
        } else if (strcmp(lookahead.atributo_ID,"write") == 0){ 
            lookahead.atomo = WRITE;
        } else {                                            
            lookahead.atomo = IDENTIFICADOR;
        }
        return lookahead;


    q3: // -------------------------------------------------------------------------------- numero BINARIO 
        if(*buffer == '0' || *buffer == '1'){
            buffer++;
            goto q3;
        } 
        if (isalpha(*buffer)){ // TODO ver condição pra dar erro 
            printf("ERRO Lexico, esperado caractere [0|1], encontrado [%c]\n",*buffer);
            exit(0);
        }
        if (buffer > iniID) {
            strncpy(lookahead.atributo_ID, iniID, buffer - iniID);
        }
        lookahead.atributo_ID[buffer-iniID] = '\0';
        lookahead.atributo_numero = bin_to_int(lookahead.atributo_ID);
        lookahead.atomo = NUMERO;
        return lookahead;
    
    q4: //  ----------------------------------------------------------------------------------- Comentários
        if (*buffer == '{'){ // caso o comentario encontrado começar com "abre chaves" indica comentario de varias linhas
            buffer++;
            if (*buffer == '-'){
                linhas_de_comentario = 0;
                do {
                    if (*buffer == '\n') {
                        linhas_de_comentario++;
                    }
                    if (*buffer == '\0'){
                        printf("ERRO lexico, não encontrado o fechamento do comentario\n");
                        exit(0);
                    }
                    buffer++;
                }while (!(*buffer == '-' && *(buffer + 1) == '}'));
                buffer+=2;
            } else {
                printf("ERRO Lexico, esperado caractere [-], encontrado [%c]\n",*buffer);
                exit(0);
                return lookahead;
            }
        } else { // caso contrario é um comentario ate encontrar o quebra de linha
            do{
                buffer++;
            }while (*buffer != '\n');
        }
        strcpy(lookahead.atributo_ID,"");
        lookahead.atomo = COMENTARIO;
        return lookahead;
}
    
// LEXER -------------------------------------------------------------------------------------------------
// elimina os espaços e os outros caracteres ate achar um caractere minusculo 
TInfoAtomo obter_atomo(){
    TInfoAtomo lookahead;
    // Caracteres Delimitadores --------------------------------------------------------------------------
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' ||*buffer == '\r'){
        if( *buffer =='\n')
            linha++;
        buffer++;
    }
    // Comentários, indetificadores, Palavras reservadas, ou Números. ------------------------------------
    if (islower(*buffer) || *buffer == '#' || *buffer == '{' || *buffer == '0' || *buffer == '.'|| 
        *buffer == ';'|| *buffer == ',' || *buffer == '+' || *buffer == '-'|| *buffer == '*'|| 
        *buffer == '/'|| *buffer == '<'|| *buffer == '>' || *buffer == '='|| *buffer == ':'|| 
        *buffer == '('|| *buffer == ')'){
        lookahead = reconhece_id(); 
    }else if (*buffer == '\0'){
        strcpy(lookahead.atributo_ID,"\0");
        lookahead.atomo = EOS;
    } else {
        printf("ERRO Lexico, este caractere encontrado [%c] não corresponde a gramatica \n",*buffer);
        exit(0);
    }
    lookahead.linha = linha;
    linha += linhas_de_comentario;
    linhas_de_comentario = 0;
    return lookahead;
}


// função consome que recebe um Tipo de atomo e o compara com o tipo do ultimo atomo verificado pelo LEXER 
void consome(TAtomo atomo){
    while (lookahead.atomo == COMENTARIO){ // enquanto indentificar um comentario o imprime porem o ignora 
        printf("%d:%s\n",lookahead.linha,msgAtomo[lookahead.atomo]);
        lookahead = obter_atomo();
    }
    if(lookahead.atomo == atomo) { // se o tipo de atomo passado na função for igual ao tipo do atomo reconhecido pelo lexer indica que esta de acordo com a gramatica
        if (lookahead.atomo == IDENTIFICADOR){
            printf("%d:identificador | %s\n",lookahead.linha, lookahead.atributo_ID);
        } else if (lookahead.atomo == NUMERO){
            printf("%d:%d\n",lookahead.linha,lookahead.atributo_numero);
        }else {
            printf("%d:%s\n",lookahead.linha,msgAtomo[lookahead.atomo]);
        }
        lookahead = obter_atomo();
    } else { // caso não for igual indica um erro sintatico 
        printf("ERRO sintatico, encontrado [%s] esperado um [%s]\n",msgAtomo[lookahead.atomo],msgAtomo[atomo]);
        exit(0);
    }

}

// <tipo> ::= integer | boolean
void tipo(){
    if (lookahead.atomo == INTEGER){
        consome(INTEGER);
    }
    else{
        consome(BOOLE);
    }
}

// <lista_variavel> ::= identificador { “,” identificador }
void lista_variavel(){
    consome(IDENTIFICADOR);
    while (lookahead.atomo == VIRGULA){
        consome(VIRGULA);
        consome(IDENTIFICADOR);
    }
}

// essa declaração é somente para não dar erro na proxima função 
void expressao();

/* 
<fator> ::= identificador | numero | true |false | not <fator> | “(“ <expressao> “)”
*/
void fator(){
    if (lookahead.atomo == IDENTIFICADOR || lookahead.atomo == NUMERO || lookahead.atomo == TRUEe || lookahead.atomo == FALSEe){
        consome(lookahead.atomo);
    } else if (lookahead.atomo == NOT){
        consome(NOT);
        fator();
    } else{
        consome(ABRE_PARENTESES);
        expressao();
        consome(FECHA_PARENTESES);
    }
}

// <termo> ::= <fator> { ( “*” | “/” ) <fator> }
void termo(){
    fator();
    while (lookahead.atomo == VEZES || lookahead.atomo == DIVIDE){
        consome(lookahead.atomo);
        fator();
    }
}

// <expressao_simples> ::= <termo> { (“+” | “−” ) <termo> }
void expressao_simples(){
    termo();
    while (lookahead.atomo == MAIS || lookahead.atomo == MENOS){
        consome(lookahead.atomo);
        termo();
    }
}

//  <expressão_relacional> ::= <expressao_simples> [<op_relacional> <expressao_simples>]
// op_relacional foi solucionado pela linha do if
void expressao_relacional(){
    expressao_simples();
    if (lookahead.atomo == MAIOR_Q || lookahead.atomo == MENOR_Q || lookahead.atomo == MAIOR_IGUAL_Q || lookahead.atomo == MENOR_IGUAL_Q || lookahead.atomo == DIFERENTE || lookahead.atomo == IGUAL){
        consome(lookahead.atomo);
        expressao_simples();
    }
}

// <expressao_logica>::= <expressao_relacional> { and <expressao_relacional> }
void expressao_logica(){
    expressao_relacional();
        while (lookahead.atomo == AND){
        consome(AND);
        expressao_relacional();
    }
}

// <expressao> ::= <expressao_logica> { or <expressao_logica> }
void expressao(){
    expressao_logica();
    while (lookahead.atomo == OR){
        consome(OR);
        expressao_logica();
    }
}

// <comando> ::= <comando_atribuicao> | <comando_condicional> | <comando_repeticao> |<comando_entrada> | <comando_saida> | <comando_composto>
void comando(){
    if (lookahead.atomo == SET){ // para comando de atribuição
        consome(SET);
        consome(IDENTIFICADOR);
        consome(TO);
        expressao();
    } else if (lookahead.atomo == IF){ // para comando condicional 
        consome(IF);
        expressao();
        consome(D_PONTOS);
        comando();
        if (lookahead.atomo == ELIF){ // caso em seguida tenha um elif (no maximo 1 elif)
            consome(ELIF);
            comando();
        }
    } else if (lookahead.atomo == FOR){ // comando de repetição
        consome(FOR);
        consome(IDENTIFICADOR);
        consome(OF);
        expressao();
        consome(TO);
        expressao();
        consome(D_PONTOS);
        comando();
    }else if (lookahead.atomo == READ){ // comando de entrada
        consome(READ);
        consome(ABRE_PARENTESES);
        lista_variavel();
        consome(FECHA_PARENTESES);
    }else if (lookahead.atomo == WRITE){ // comando de saida
        consome(WRITE);
        consome(ABRE_PARENTESES);
        expressao();
        while (lookahead.atomo == VIRGULA){
            consome(VIRGULA);
            expressao();
        }
        consome(FECHA_PARENTESES);
    }else{ // comando composto
        consome(BEGIN);
        comando();
        while (lookahead.atomo == PONTO_VIRGULA){
            consome(PONTO_VIRGULA);
            comando();
        }
        consome(END);
    }
}

// <comando_composto> ::= begin <comando> {“;”<comando>} end
void comando_composto(){
    consome(BEGIN);
    comando();
    while(lookahead.atomo == PONTO_VIRGULA){
        consome(PONTO_VIRGULA);
        comando();
    }
    consome(END);
}

// <declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
void declaracao_de_variaveis(){
    while(lookahead.atomo == BOOLE || lookahead.atomo == INTEGER){
        tipo();
        lista_variavel();
        consome(PONTO_VIRGULA);
    }
}

// <bloco>::= <declaracao_de_variaveis> <comando_composto>
void bloco(){
    declaracao_de_variaveis();
    comando_composto();
}

// <programa> ::= program identificador “;” <bloco> “.”
void programa(){
    consome(PROGRAM);
    consome(IDENTIFICADOR);
    consome(PONTO_VIRGULA);
    bloco();
    consome(PONTO);
    consome(EOS);
}


int main(int argc, char *argv[]) {
    if (argc < 2) { // caso o usuario não informe um arquivo 
        printf("Por favor, informe o nome do arquivo como argumento.\n");
        return 0;
    }
    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) { // caso o usuario informe um arquivo não existente 
        printf("Não foi possível abrir o arquivo %s.\n", argv[1]);
        return 0;
    }
    
    buffer = ler_arquivo(arquivo);
    fclose(arquivo);

    lookahead = obter_atomo();
    programa();
    
    printf("\nfim da analise lexica :D \n");
    return 0;
}
