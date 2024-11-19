# Compilador

## [Fase 1 - Léxico e Sintático](https://github.com/Victor-Vaglieri/compilador/tree/main/fase1)

Este compilador analisa a linguagem Pascal+- cuja sintaxe está descrita em notação EBNF. 

- **Não-terminais** da gramática são representados entre parênteses angulares (`< >`).
- **Terminais** (átomos do analisador léxico) estão em caixa alta ou entre aspas (e.g., `" ; "`).
- **Notações especiais**:
  - `{ α }`: indica repetição de zero, uma ou mais vezes (`α*`).
  - `[ β ]`: representa que a cadeia `β` é opcional.

O símbolo inicial da gramática é `<programa>`.


### Gramática da Linguagem

```ebnf
<programa> ::= PROGRAM IDENTIFICADOR “;” <bloco> “.”
<bloco> ::= <declaracao_de_variaveis> <comando_composto>
<declaracao_de_variaveis> ::= {<tipo> <lista_variavel> “;”}
<tipo> ::= INTEGER | BOOLEAN
<lista_variavel> ::= IDENTIFICADOR { “,” IDENTIFICADOR }
<comando_composto> ::= BEGIN <comando> {“;”<comando>} END
<comando> ::= 
    <comando_atribuicao> | 
    <comando_condicional> | 
    <comando_repeticao> | 
    <comando_entrada> | 
    <comando_saida> | 
    <comando_composto>
<comando_atribuicao> ::= SET IDENTIFICADOR TO <expressao>
<comando_condicional> ::= IF <expressao> “:” <comando> [ELIF <comando>]
<comando_repeticao> ::= FOR IDENTIFICADOR OF <expressao> TO <expressao> “:” <comando>
<comando_entrada> ::= READ “(“ <lista_variavel> “)”
<comando_saida> ::= WRITE “(“ <expressao> { “,” <expressao> } “)”
<expressao> ::= <expressao_logica> { OR <expressao_logica> }
<expressao_logica> ::= <expressao_relacional> { AND <expressao_relacional> }
<expressao_relacional> ::= <expressao_simples> [<op_relacional> <expressao_simples>]
<op_relacional> ::= “<” | “<=” | “=” | “/=” | “>” | “>=”
<expressao_simples> ::= <termo> { (“+” | “−”) <termo> }
<termo> ::= <fator> { ( “*” | “/” ) <fator> }
<fator> ::= 
    IDENTIFICADOR | 
    NUMERO | 
    TRUE | 
    FALSE | 
    NOT <fator> | 
    “(“ <expressao> “)”
```
### Especificação Léxica

- **Delimitadores**: 
  - Espaços em branco, quebras de linha (`\n`), tabulações (`\t`) e retorno de carro (`\r`) são ignorados pelo analisador léxico, mas o controle de linhas deve ser mantido.
  
- **Comentários**:
  - **Linha única**: começam com `#` e terminam no caractere `\n`.
  - **Múltiplas linhas**: começam com `{-` e terminam com `-}`. Esses comentários devem ser reportados ao analisador sintático antes de serem descartados.
  
- **Identificadores**:
  - Iniciam com uma letra minúscula, seguidas de até 14 caracteres compostos por letras, dígitos ou `_`.
  - Identificadores com mais de 15 caracteres geram erro léxico.
  - Definição regular:
    ```plaintext
    letra → a|b|...|z
    digito → 0|1|...|9
    identificador → letra (letra|digito|_)*
    ```

- **Números**:
  - Apenas números inteiros na notação binária.
  - Definição regular:
    ```plaintext
    numero → 0b(0|1)+
    ```

### Funcionalidades do Compilador

1. Lê o arquivo fonte informado na linha de comando.
2. Reconhece e exibe os átomos presentes no arquivo e que linha esta presente no arquivo.
4. Indica se o programa está sintaticamente correto.
5. Em caso de erro léxico ou sintático, emite uma mensagem explicativa e encerra a execução.

### Compilação

Para compilar o código, utilize o comando:

```bash
gcc -g -Og -Wall compilador.c -o compilador
```

### Execução

Para execução do código, use o comando:

```bash
./compilador teste.txt
```
---

## [Fase 2 - Semântico](https://github.com/Victor-Vaglieri/compilador/tree/main/fase2)

### Verificação Semântica

O compilador realiza a verificação semântica em dois momentos principais:

#### 1. Declaração de Variáveis
- Na seção `<declaracao_de_variaveis>`, o compilador verifica se os identificadores usados nos nomes das variáveis são **únicos**.  
  - **Erro Semântico**: Caso haja repetição de identificadores, o compilador será encerrado e uma mensagem explicativa será exibida.
- É implementada uma **mini tabela de símbolos** para armazenar as variáveis declaradas, contendo:
  - **Identificador**: Nome da variável.
  - **Endereço**: Ordem de declaração (a primeira variável recebe endereço 0, a segunda recebe 1, e assim por diante).

#### 2. Corpo do Programa
- As variáveis declaradas podem ser referenciadas em comandos de atribuição, expressões e chamadas de funções de entrada e saída.
- Sempre que uma variável for referenciada, o compilador verifica se ela foi **corretamente declarada**.  
  - **Erro Semântico**: Caso a variável não tenha sido declarada, o compilador é encerrado com uma mensagem explicativa.

> **Nota**: Para simplificar a análise semântica e a geração de código intermediário, o compilador considera apenas variáveis e expressões do tipo **integer**.  
Não são permitidas:
- Expressões mistas como `25+(x>y)`.
- Atribuições de valores lógicos como `var=false`.


### Geração de Código Intermediário

A geração do código intermediário segue a abordagem descrita no livro *Implementação de Linguagem de Programação* de Tomasz Kowaltowiski (Seção 10.3).  
- **MEPA**: As instruções da linguagem MEPA são geradas nas funções mutuamente recursivas do analisador sintático e semântico.
- As principais funções envolvidas são:
  - **`proximo_rotulo()`**: Retorna o próximo rótulo consecutivo positivo (e.g., 1, 2, 3, ...).
  - **`busca_tabela_simbolos(atributo_ID)`**:
    - Recebe como parâmetro o identificador atual.
    - Retorna o endereço da variável na tabela de símbolos.
    - **Erro Semântico**: Se o identificador não estiver na tabela, um erro será gerado e o programa será encerrado.

#### Saída do Compilador
Se o programa não apresentar erros léxicos, sintáticos ou semânticos, o compilador exibirá:
1. A tradução do código-fonte para a linguagem MEPA.
2. A tabela de símbolos, listando as variáveis declaradas e seus respectivos endereços.


### instruções MEPA utilizadas

- CRCT **k** (carregar constante **k**)

- CRVL **n** (carregar valor de variável **n**)

- SOMA (soma)

- SUBT (subtração)

- MULT (multiplicação)

- DIVI (divisão)

- CONJ (**E** logico)

- DISJ (**OU** logico)

- CMME (comparar menor)

- CMMA (comparar maior)

- CMIG (comparar igual)

- CMDG (comparar desigual)

- CMEG (comparar menor igual)

- CMAG (comparar maior igual)

- DSVS **p** (Desviar sempre para **p** que representa um endereço do programa na memória)

- DSVF p (Desviar se falso para **P**)

- AMEM **m** (aloca memoria de tamanho **m**)

- INPP (Iniciar programa principal)

- PARA (encerra a execução)

- LEIT (função de entrada)

- IMPR (função de saida)

- ARMZ **n** (armazenar valor no endereço **n** da tabela de simbolos)


### Compilação

Para compilar o código, use o comando:

```bash
gcc -g -Og -Wall compilador.c -o compilador
```
### Execução

Para execução do código, use o comando:

```bash
./compilador teste.txt
```
