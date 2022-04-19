#include <stdio.h>
#include <string.h>
#include "prog.h"                                   // Funções declaradas em um arquivo para nao ter problema na ordem de Declaração
#include <time.h>
#include <stdlib.h>

// Variaveis Globais
char wordSecret[TAMANHO_PALAVRA];
char chutes[26];
int numeroChutes = 0;

void escolheWordSecret (){
/*sprintf(wordSecret, "MELANCIA");*/                    //  Armazena cada letra da palavra ao inves de usar um char letra[7]

    FILE* f;                                            //Devolve um ponteiro de Arquivo
    f = fopen("wordSecret.txt", "r");                  //r de READ
    if(f == 0){
        printf("Desculpe, Banco de dados Indisponivel\n\n");
        exit(1);
    }

    int qntDePalavras;
    fscanf(f, "%d", &qntDePalavras);

    srand(time(0));
    int randomico = rand() % qntDePalavras;

    for (int i = 0; i <= randomico; i++){
        fscanf(f, "%s", wordSecret);
    }
    fclose(f);
}

void bemVindo(){        
// Criação de uma função para o codigo ficar melhor e menos poluido (EX função printf)

    printf("\n\n");
    printf(" _______________________________________________________________ \n");
    printf("|                                                               |\n");
    printf("|         []  ,----.___                                         |\n");
    printf("|       __||_/___      '.            BEM VINDO                  |\n");
    printf("|      / O||    /|       )            AO JOGO                   |\n");
    printf("|     /   ''   / /   =._/               DE                      |\n");
    printf("|    /________/ /                      FORCA                    |\n");
    printf("|    |________|/                                                |\n");
    printf("|                                                               |\n");
    printf("|_______________________________________________________________|\n");
    printf("\n\n\n");
}

// int* numeroChutes quer dizer que aqui vai receber um endereço de memoria de um INTEIRO
void chuta() {                                      // Variaveis Novas Dentro do parenteses é um PARAMETRO para usar as variaveis de outras funções  (main)

    // Captura um Novo Chute
    char chute;
    printf("Qual Letra? ");
    scanf(" %c", &chute);                           // ESPAÇO %c == Ignorar o ENTER na hora de digitar a LETRA  
    printf("\n");

    chutes[numeroChutes] = chute;                   // Variavel chutes é um array == PONTEIRO
    numeroChutes++;                                //  *numeroChutes ++ no conteudo do endereço de memoria
}

int jaChutou (char letra){     // Função devolve um INT

    int achou = 0;

    for(int j = 0; j < numeroChutes; j++){             
        if(chutes[j] == letra){                         // Pega uma Letra e diz se ela ja foi chutada ou não  // Se o chute da TENTATIVA for igual a LETRA de uma determinada posição da PALAVRA
            achou = 1;                                 // Entao achou = 1 e quebra o codigo para o IF onde imprime a LETRA achada
            break;
        }
    }

    return achou;                                       //Função retorna ao main com o INT ACHOU   
}

void desenhaForca(){

    int erros = chutesErrados();

    printf("    _________           \n");
    printf("   |/       |           \n");
    printf("   |       %c%c%c       \n", (erros >= 1 ? '(' : ' '),          // IF TERNARIO ==  erros >= 1 ? (<= Retorna) '(' : (<= Se nao, Retorna) ' ';
            (erros >= 1 ? '_' : ' '),    (erros >= 1 ? ')' : ' '));
    printf("   |       %c%c%c       \n", (erros >= 3 ? '\\' : ' '), 
            (erros >= 2 ? '|' : ' '),    (erros >= 3 ? '/' : ' '));
    printf("   |        %c          \n", (erros >= 2 ? '|' : ' '));
    printf("   |       %c %c        \n", (erros >= 4 ? '/' : ' '), 
            (erros >= 4 ? '\\' : ' '));
    printf("   |                    \n");
    printf("  _|___                 \n");
    printf("\n\n");

    printf("Voce Ja Deu %d Chutes\n", numeroChutes);

    // Imprime Palavra Secreta
    for (int i = 0; i < strlen(wordSecret); i++){                           // Passar por cada posição da PALAVRA SECRETA

        // A Letra Ja Foi Chutada?
        int achou = jaChutou (wordSecret[i]);

        if (achou){
            printf("%c ", wordSecret[i]);
        } else{
            printf("_ ");
        }
    }

    printf("\n");
}

int acertou(){
    for (int i = 0; i < strlen(wordSecret); i++){
        if(!jaChutou(wordSecret[i])){
            return 0;
        }
    }
    return 1;
}

int chutesErrados(){
    int erros = 0;

    for(int i = 0; i < numeroChutes; i++){
        int existe = 0;

            for( int j = 0; j < strlen(wordSecret); j++){
                if (chutes[i] == wordSecret[j]){
                    existe = 1;
                    break;
                }
            }
        if(!existe) erros++;
    }
    return erros;
}

int enforcou(){
    
    return chutesErrados() >=5;
}

void addWordSecret(){
    char quer;

    printf("Voce Deseja Adicionar uma Nova Palavra no Jogo? (S/N) ");
    scanf(" %c", &quer);

    if(quer == 'S'){
        char newWordSecret[TAMANHO_PALAVRA];
        printf("Qual a Nova Palavra? ");
        scanf("%s", newWordSecret);

        FILE* f;

        f = fopen("wordSecret.txt", "r+" );  //r+ Leitura e Escrita

        if(f == 0){
            printf("Desculpe, Banco de dados Indisponivel\n\n");
            exit(1);
        }

        int qnt;
        fscanf(f, "%d", &qnt);
        qnt++;

        fseek(f, 0, SEEK_SET);     //Posiciona o ponteiro de leitura do FILE para a 1º Linha Novamente 
        fprintf(f, "%d", qnt);

        fseek(f, 0, SEEK_END);      // Posiciona no Final do arquivo
        fprintf(f, "\n%s", newWordSecret);

        fclose(f);
    }
}

int main()
{
    escolheWordSecret();

    bemVindo(); 

    do{
        desenhaForca();

        chuta();                                    // &numeroChutes esta passando o endereço de memoria como um PONTEIRO para a função 

    } while (!acertou() && !enforcou());            // Função enforcou LIMITE

    if(acertou()){
        printf("\n==> %s <==\n", wordSecret);
        printf("\n\n");
        printf(" _______________________________________________________________ \n");
        printf("|                                                               |\n");
        printf("|                    Parabens! Voce Ganhou!                     |\n");
        printf("|             Jogue de Novo, Voce e um bom Jogador              |\n");
        printf("|_______________________________________________________________|\n");
        printf("\n\n");
        // printf("                 Voce Acertou em %d Tentativas!\n", tentativa);
        // printf("                     Total de Pontos: %.1f\n", pontos);
    }
    else{
        printf("\n\n");
        printf("*********************** Fim de Jogo! ***********************\n");
        printf("  _____                         _____                         \n");
        printf(" / ____|                       / ___ \\                       \n");
        printf(" | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __         \n");
        printf(" | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|   \n");
        printf(" | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |          \n");
        printf(" \\______|\\__,_|_| |_| |_|\\___| \\_____/  \\_/ \\___|_|     \n\n");
        printf("********************* JOGUE NOVAMENTE!! ********************\n\n");
        printf("               ==> A Palavra era: %s <==\n\n", wordSecret);
                                                                                                                                               
    }

    addWordSecret();
}

// CODE BY GHABRIEL MOLINA