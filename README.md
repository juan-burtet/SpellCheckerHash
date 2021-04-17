# SpellCheckerHash
  
### [English]

A program made in C, that compares words already stored in a Hash Table.

#### Words Comparison Rules:

  * __One More Letter__: Compare if there is a word with one more letter.
    > __Example:__\
    >   Typed the word "_ca_"
    >   Print the word "_car_"
  * __One Less Letter__: Compare if there is a word with one less letter.
    > __Example:__\
    >   Typed the word "_caar_"
    >   Print the word "_car_"
  * __Changed Letter__: Compare if there is a word with neighboring letters exchanged
    > __Example:__\
    >   Typed the word "_cra_"
    >   Print the word "_car_"
  * __Wrong Letter__: Compare if there is a word with one wrong letter.
    > __Example:__\
    >   Typed the word "_caa_"
    >   Print the word "_car_"

#### Commands:
All commands are typed each line, in case you typed incorrectly, print __fail__


  * __word__
    > If there is a word, print __ok__\
    > If there is not a word, print similar words\
    > If there is a no similar words, print __not found__
  * __\+__
    > If the word previously typed is not on the table, the word is added. Print __ok _word___\
    > If the word previously typed is on the table, Print __fail _word___
  * __\-__
    > If the word previously typed is on the table, the word is deleted. Print __ok _word___\
    > If the word previously typed is not on the table, Print __fail _word___
  * __\*__
    > Finish the Program
    
#### Observation:
  The words are not store when the program is finished.\
  
  
  

### [Português]

Um programa feito em C, que compara palavras que estão armazenadas em uma Tabela Hash. 

#### Regras de Comparação de Palavras:

  * __Uma Letra a Mais__: Compara se existe alguma palavra com uma letra a mais.
    > __Exemplo:__\
    >   Digitou a palavra "_caro_"\
    >   Imprime a palavra "_carro_"
  * __Uma Letra a Menos__: Compara se existe alguma palavra com uma letra a menos.
    > __Exemplo:__\
    >   Digitou a palavra "_carrro_"\
    >   Imprime a palavra "_carro_"
  * __Letra Trocada__: Compara se existe alguma palavra com letras vizinhas trocadas.
    > __Exemplo:__\
    >   Digitou a palavra "craro"\
    >   Imprime a palavra "carro"
  * __Letra Errada__: Compara se existe alguma palavra com uma letra errada.
    > __Exemplo:__\
    >   Digitou a palavra "cerro"\
    >   Imprime a palavra "carro"

#### Comandos:
Todos os comandos são dados em cada linha, caso não for digitado corretamente, imprime __fail__
  * __palavra__
    > Se tiver tiver a palavra, imprime __ok__\
    > Se não tiver a palavra, imprime palavras parecidas\
    > Se não tiver nenhuma palavra parecida, imprime __not found__
  * __\+__
    > Se a palavra digitada anteriormente não estiver na Tabela, a palavra é adicionada. Imprime __ok _palavra___\
    > Se a palavra digitada anteriormente estiver na tabela, Imprime __fail _palavra___
  * __\-__
    > Se a palavra digitada anteriormente estiver na Tabela, a palavra é excluida. Imprime __ok _palavra___\
    > Se a palavra digitada anteriormente não estiver na tabela, Imprime __fail _palavra___
  * __\*__
    > Finaliza o programa

#### Observação:
  As palavras não ficam armazenadas quando o programa for finalizado.
