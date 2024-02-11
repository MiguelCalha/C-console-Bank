#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h> //Biblioteca para a hora

#define CONTAS_MAX 100
#define TITULARES_MAX 5
#define HISTORICO_MAX 3

typedef struct
{
    int dia;
    int mes;
    int ano;
} Data;

typedef struct
{
    char nome[20+1];
    long nif;
} Cliente;

typedef struct
{
    int numeroConta;
    Cliente titulares[TITULARES_MAX];
    char modalidade;
    float saldoActual;
    float historicoSaldo[HISTORICO_MAX];
    Data dataAbertura;
} Conta;


typedef struct
{
    Conta contasBanco[CONTAS_MAX];
    int totalContasAtivas; //contasExistentes
    int numSequecialConta; //Incrementar o numero de conta
    float totalFundos;
} Banco;


void abrirConta(Banco *);
void listarContas(Banco);
void carregarDadosDeBin(Banco *);
void gravarDadosParaBin(Banco *);
void exportarListagemContasParaTxt(Banco);
void consultarConta(Banco);
void depositar(Banco *);
void levantar(Banco *);
void transferir(Banco *);
void eliminarConta(Banco *);
void editarConta(Banco *);


int main()
{
    char resposta=0;
    int resposta1 = 0;

    Banco bancoAdR = {}; //inicializa tudo a 0 e '\0'
    bancoAdR.numSequecialConta = 1;
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    //CARREGAR DADOS
    do
    {
        printf("Deseja carregar dados? (s/n)"); //Pergunta ao utilizador se este deseja carregar os dados de bin
        scanf("%c", &resposta);
    }
    while(resposta !='s' && resposta!='n' && resposta !='S' && resposta!='N' );

    if(resposta=='s' || resposta == 'S')
    {
        carregarDadosDeBin(&bancoAdR);
    }
    system("CLS");
    do
    {
        //menu principal do programa
        system("color 6"); // cor da consola (6)
        system("cls");
        printf(" \t\t\t|||||||||||||||||| BANCO ALVES DOS REIS |||||||||||||||||| \n");
        printf("\n");
        printf(" \t\t\t\t\t (1). Abrir Conta\n");
        printf("\n");
        printf(" \t\t\t\t\t (2). Listar Contas\n");
        printf("\n");
        printf(" \t\t\t\t\t (3). Consultar\n");
        printf("\n");
        printf(" \t\t\t\t\t (4). Depositar Dinheiro\n");
        printf("\n");
        printf(" \t\t\t\t\t (5). Levantar Dinheiro\n");
        printf("\n");
        printf(" \t\t\t\t\t (6). Transferir Dinheiro\n");
        printf("\n");
        printf("\t\t \t\t\t (7). Editar conta\n");
        printf("\n");
        printf("\t\t \t\t\t (8). Eliminar Conta\n");
        printf("\n");
        printf("\t\t \t\t\t (9). Gravar Dados\n");
        printf("\n");
        printf("\t\t \t\t\t (0). Sair\n");

        scanf("%d", &resposta1);



        switch(resposta1)
        {


        case 1:


            abrirConta(&bancoAdR);
            break;

        case 2:

            listarContas(bancoAdR);
            break;

        case 3:

            consultarConta(bancoAdR);
            break;

        case 4:
            depositar(&bancoAdR);
            break;

        case 5:
            levantar(&bancoAdR);
            break;

        case 6:
            transferir(&bancoAdR);
            break;

        case 7:
            editarConta(&bancoAdR);
            break;

        case 8:
            eliminarConta(&bancoAdR);
            break;
        case 9:
            gravarDadosParaBin(&bancoAdR);
            break;

        case 0:
            exit(0);
            break;

        }

    }
    while(resposta1 != 0);
    system("cls");
    return 0;

}


//-------------------------------------------------------------------------------
void abrirConta(Banco *banco)
{

    system("cls");
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    // char resposta;


    if(banco->totalContasAtivas < CONTAS_MAX) //ainda temos espaco para abrir contas
    {

        int proximaPosVazia = banco->totalContasAtivas;
        int numTitulares = 0;
        int i;
        double dinheiroDepositar=0;
        int idade=0;
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        fflush(stdin);
        printf("Processo de abertura de conta número %d\n", banco->numSequecialConta);
        banco->contasBanco[proximaPosVazia].numeroConta = banco->numSequecialConta; //atribuicao automatica do numero

        do
        {
            printf("Introduza número de titulares (max %d):\n", TITULARES_MAX);
            scanf("%d", &numTitulares);
            fflush(stdin);

            if(numTitulares < 1 || numTitulares > TITULARES_MAX)
            {
                printf("\tERRO: deve introduzir entre 1 e %d titulares\n\n", TITULARES_MAX);
            }
        }
        while(numTitulares < 1 || numTitulares > TITULARES_MAX);

        for(i=0; i<numTitulares; i++)
        {
            printf("\tTitular %d\n", i+1);

            printf("\tIntroduza nome\t: ");
            gets(banco->contasBanco[proximaPosVazia].titulares[i].nome);
            fflush(stdin);
            do
            {
                printf("\tIntroduza nif (9 carateres)\t: ");
                scanf("%ld", &banco->contasBanco[proximaPosVazia].titulares[i].nif);
                fflush(stdin);
            }
            while (banco->contasBanco[proximaPosVazia].titulares[i].nif<=99999999 || banco->contasBanco[proximaPosVazia].titulares[i].nif>999999999);
            //While de cima impede que o utilizador insir aum nif com menos de 9 carateres


            printf("\n   Insira a sua idade: \t");
            scanf("%d", &idade);
            fflush(stdin);

            if(idade >= 18)
            {
                banco->contasBanco[i].modalidade = 'n'; //modalidade normal
            }
            else
            {
                banco->contasBanco[i].modalidade = 'i'; //modalidade insenta
            }


            printf("\n");

            do
            {
                printf("\tIntroduza quanto deseja depositar (minimo 150)\t: ");
                scanf("%lf", &dinheiroDepositar);
                banco->contasBanco[proximaPosVazia].saldoActual += dinheiroDepositar; // adiciona o dinheiro ao saldo da conta

            } while(dinheiroDepositar <150);


            fflush(stdin);

             for (int k=2; k>0; k--)
                {
                    banco->contasBanco[i].historicoSaldo[k] = banco->contasBanco[i].historicoSaldo[k-1];
                }
                banco->contasBanco[i].historicoSaldo[0] = banco->contasBanco[i].saldoActual;

            //Fazer a validaçao (NAO PODE DEPOSITAR MENOS QUE 150€)
        }

        printf("Data de abertura: ");
        printf("%d\\",banco->contasBanco[banco->totalContasAtivas].dataAbertura.dia=t->tm_mday); //Vai buscar o dia atual ao sistema
        printf("%d\\",banco->contasBanco[banco->totalContasAtivas].dataAbertura.mes=t->tm_mon + 1); // vai buscar o mes atual ao sistema
        printf("%d",banco->contasBanco[banco->totalContasAtivas].dataAbertura.ano=t->tm_year + 1900); // vai buscar o ano atual ao sistema
        printf("\n");

        //...outras recolhas de dados

        banco->totalContasAtivas++;
        banco->numSequecialConta++;


    }
    else
    {
        //nao ha espaco para mais contas
        printf("\n\tANTINGIU O LIMITE DE CONTAS ATIVAS! (max %d)\n", CONTAS_MAX);
    }
    system("pause");
    system("cls");


//return 0;
}

void listarContas(Banco b)
{
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    system("cls");
    int i=0;
    printf("\n\n----------Listagem de contas----------\n\n");

    if(b.totalContasAtivas>0)
    {
        for(i=0; i<CONTAS_MAX; i++)
        {
            if(b.contasBanco[i].numeroConta!=0) //se numConta != 0 significa que existe uma conta nesta posicao i
            {
                printf("\nNúmero de Conta:\t%d\n\n", b.contasBanco[i].numeroConta);

                int pos = 0; //indice da casa do array dos titulares

                while(b.contasBanco[i].titulares[pos].nif!=0) //se o nif for != 0 significa que existe mais um titular
                {
                    printf("\tTitular %d\n", pos+1);
                    printf("\t\tNome: %s\n", b.contasBanco[i].titulares[pos].nome);
                    printf("\t\tNif: %ld\n", b.contasBanco[i].titulares[pos].nif);
                    //printf("\t\t Data de Abertura %d / %d / %d\n",  b.contasBanco[i]);

                    if(b.contasBanco[i].modalidade == 'i')
                    {
                        printf("\t\tModalidade: Insenta\n");
                    }
                    else if (b.contasBanco[i].modalidade == 'n')
                    {
                        printf("\t\t Modalidade: Normal");
                    }
                    pos++;
                }

                printf("\n");
                printf("\tSaldo %03f\n", b.contasBanco[i].saldoActual);
                //...
            }
            else
            {
                break; //ja nao ha mais contas e podemos terminar o ciclo de listagem
            }

            printf("\n");
        }
    }
    else
    {
        printf("...não existem contas para listar...");
    }
    system("pause");
}

void carregarDadosDeBin(Banco *refBanco)
{
    system("cls");
    FILE *ficheiroBinario;

    ficheiroBinario = fopen("banco.bin", "rb");

    if(ficheiroBinario == NULL)
    {
        perror("Erro ao abrir: ");
    }
    else
    {
        fread(refBanco, sizeof(Banco), 1, ficheiroBinario);
        puts("Dados bin carregados com sucesso.");
    }

    fclose(ficheiroBinario);

    system("pause");
}

void gravarDadosParaBin(Banco *refBanco)
{
    system("cls");
    FILE *ficheiroBinario;

    ficheiroBinario = fopen("banco.bin", "wb");

    fwrite(refBanco, sizeof(Banco), 1, ficheiroBinario);

    fclose(ficheiroBinario);

    puts("Dados bin gravados com sucesso.");

    system("pause");
}

void exportarListagemContasParaTxt(Banco b)
{
    system("cls");
    int i=0;
    FILE *ficheiro;
    ficheiro = fopen("ContasBanco.txt", "w");

    fputs("\n\n----------Listagem de contas----------\n\n", ficheiro);

    if(b.totalContasAtivas>0)
    {
        for(i=0; i<CONTAS_MAX; i++)
        {
            if(b.contasBanco[i].numeroConta!=0) //se numConta != 0 significa que existe uma conta nesta posicao i
            {
                fprintf(ficheiro, "\n.Numero de Conta:\t%d\n\n", b.contasBanco[i].numeroConta);

                int pos = 0; //indice da casa do array dos titulares

                while(b.contasBanco[i].titulares[pos].nif!=0) //se o nif for != 0 significa que existe mais um titular
                {
                    fprintf(ficheiro, "\tTitular %d\n", pos+1);
                    fprintf(ficheiro, "\t\tNome: %s\n", b.contasBanco[i].titulares[pos].nome);
                    fprintf(ficheiro, "\t\tNif: %ld\n", b.contasBanco[i].titulares[pos].nif);
                    pos++;
                }

                fputs("\n",ficheiro);
                fprintf(ficheiro, "\tSaldo %f\n", b.contasBanco[i].saldoActual);
                //...
            }
            else
            {
                //ja nao ha mais contas e podemos terminar o ciclo de listagem
                break;
            }

            printf("\n");

        }
    }
    else
    {
        fputs("...não existem contas para listar...", ficheiro);
    }

    fclose(ficheiro);

    puts("Listagem exportada para TXT com sucesso.");

    system("pause");
}



//--------------------------------------------------------------------------------------------------------------
void consultarConta(Banco b)
{

    // int escolha=0;
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    int numConta=0;
    system("cls");

    printf("\n Insira o seu numero de conta: \t");
    scanf("%d", &numConta);

    for (int i = 0; i < CONTAS_MAX; i++) //Percorre todas as contas
    {
        if (b.contasBanco[i].numeroConta == numConta) // verifica se o numero de conta guardado é igual ao que o utilizador inseriu
        {
            printf("\n\n----------DADOS DA CONTA----------\n\n");
            for (int j = 0; j < TITULARES_MAX; j++)
            {
                printf("Nome: %s\n", b.contasBanco[i].titulares[j].nome); //imprime o nome
                fflush(stdin);
                printf("\n");
                printf("Nif: %ld\n", b.contasBanco[i].titulares[j].nif); // imprime o NIF
                fflush(stdin);
                printf("\n");
                printf("Número de conta: %d\n", b.contasBanco[i].numeroConta); //imprime o numer de conta
                fflush(stdin);
                printf("\n");
                if(b.contasBanco[i].modalidade == 'i') // se a modalidade guardada no array for i faz print de insenta
                {
                    printf("Modalidade: Insenta\n");
                }
                else if (b.contasBanco[i].modalidade == 'n')
                {
                    printf("Modalidade: Normal");
                }
                printf("\n");
                printf("Data de Abertura de Conta: %d ",b.contasBanco[i].dataAbertura.dia);
                fflush(stdin);
                printf("/%d ",b.contasBanco[i].dataAbertura.mes);
                fflush(stdin);
                printf("/%d",b.contasBanco[i].dataAbertura.ano);
                fflush(stdin);
                printf("\n");
                printf("Saldo Atual: %f\n", b.contasBanco[i].saldoActual);
                fflush(stdin);


                break;

            }
            printf("\n");

            printf("Histórico de Saldo: \n");
            printf("\n");

            for(int m=0; m<3; m++)
            {

                printf("%f\n",b.contasBanco[i].historicoSaldo[m]);
                fflush(stdin);
            }
        }
    }

    system("pause");

}

//----------------------------------------------------------------------------------------------------------------------------------

void depositar(Banco *b)
{
    system("cls");
    long nif1 = 0;
    double dinheiroDepositar = 0;

    printf("Insira o nif da sua conta.\n");
    scanf("%ld", &nif1);
    do
    {
        printf("Quanto dinheiro deseja depositar na sua conta?\n");
        scanf("%lf", &dinheiroDepositar);
    }
    while (dinheiroDepositar < 5); //Não permite o utilizador depositar menos que 5 euros

    for (int i = 0; i < CONTAS_MAX; i++) //percorre as contas todas
    {
        for (int j  = 0; j < TITULARES_MAX; j++)
        {
            if (b->contasBanco[i].titulares[j].nif == nif1) // verifica se o NIF que o utilizador inseriu é o mesmo que está guardado
            {
                if(b->contasBanco[i].modalidade == 'i')
                {

                    b->contasBanco[i].saldoActual += dinheiroDepositar; //se a modalidade for insenta não há descontos
                }
                else
                {
                    b->contasBanco[i].saldoActual = dinheiroDepositar + b->contasBanco[i].saldoActual - 5;

                }


                for (int k=2; k>0; k--)
                {
                    printf("\n Dinheiro depositado com sucesso!");
                    b->contasBanco[i].historicoSaldo[k] = b->contasBanco[i].historicoSaldo[k-1];
                }
                b->contasBanco[i].historicoSaldo[0] = b->contasBanco[i].saldoActual;
            }
        }
    }


    system("cls");
}


void levantar(Banco *b)
{

    system("cls");
    long nif1 = 0;
    double dinheiroLevantar = 0;

    printf("Insira o nif da sua conta.\n");
    scanf("%ld", &nif1);
    do
    {
        printf("Quanto dinheiro deseja levantar?\n");
        scanf("%lf", &dinheiroLevantar);

    }
    while (dinheiroLevantar < 0);

    for (int i = 0; i < CONTAS_MAX; i++)
    {
        for (int j  = 0; j < TITULARES_MAX; j++)
        {
            if (b->contasBanco[i].titulares[j].nif == nif1)
            {
                if(b->contasBanco[i].saldoActual < dinheiroLevantar)
                {
                    printf("\n Nao tem saldo suficiente");
                    break;
                }
                if(b->contasBanco[i].modalidade == 'n')
                {
                    b->contasBanco[i].saldoActual -= dinheiroLevantar;
                    b->contasBanco[i].saldoActual -= 5;
                    printf("Retire o seu dinheiro");
                }
                else
                {
                    b->contasBanco[i].saldoActual -= dinheiroLevantar;
                    printf("Retire o seu dinheiro");

                }


                for (int k=2; k>0; k--)
                {
                    b->contasBanco[i].historicoSaldo[k] = b->contasBanco[i].historicoSaldo[k-1];
                }
                b->contasBanco[i].historicoSaldo[0] = b->contasBanco[i].saldoActual;
            }
        }
    }


    system("cls");
}





void transferir(Banco *b)
{
    //Adicionar a modalidade da conta (Jovem ou Aulto)
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    system("cls");
    int i=0;
    int numeroContaRemetente=0;
    int numeroContaDestinatario=0;
    int existeRemetente=0;
    int existeDestinatario=0;
    double dinheiroTranferir=0;

    printf("\n Por favor insira o número de conta de origem (conta que vai transferir dinheiro para a outra): \t");
    scanf("%d", &numeroContaRemetente);
    fflush(stdin);
    printf("\n Por favor insira o número de conta destinatária (conta que vai receber o dinheiro): \t");
    scanf("%d", &numeroContaDestinatario);
    fflush(stdin);

    printf("\n Insira a quantia que deseja transferir:\t");
    scanf("%lf", &dinheiroTranferir);

    if(b->totalContasAtivas>0)
    {
        for(i=0; i<CONTAS_MAX; i++)
        {


            if(b->contasBanco[i].numeroConta==numeroContaRemetente) // verifica o numero de conta remetente
            {
                existeRemetente = 1;

                for(int j=0; j<CONTAS_MAX; j++)
                {
                    if(b->contasBanco[j].numeroConta==numeroContaDestinatario) // verifica o numero conta destinatario
                    {

                        existeDestinatario=1;
                        if(b->contasBanco[i].saldoActual>=dinheiroTranferir) // verifica se o utilizador tem dinheiro na conta
                        {
                            if(b->contasBanco[i].modalidade == 'n')
                            {

                                b->contasBanco[i].saldoActual -= dinheiroTranferir;
                                b->contasBanco[i].saldoActual -= 5;

                                b->contasBanco[j].saldoActual += dinheiroTranferir;
                                printf("\n O dinheiro foi transferido com sucesso!!");
                                break;
                            }
                            else
                            {

                                b->contasBanco[i].saldoActual -= dinheiroTranferir;

                                b->contasBanco[j].saldoActual += dinheiroTranferir;
                                printf("\n O dinheiro foi transferido com sucesso!!");
                                break;
                            }

                        }
                        else
                        {
                            printf("\n Não tem saldo suficiente na sua conta");
                        }

                    }
                }


            }
            if(existeDestinatario!=0)
            {
                break;
            }
        }
        if(existeRemetente==0) // verific se a conta do remetente existe
        {

            printf("\n A conta do remetente nao existe");
        }
        if(existeDestinatario==0)
        {

            printf("\n A conta do destinatario nao existe");
        }

    }
    else
    {
        puts("...não existem contas para listar...");
    }



}

void editarConta(Banco *b)
{
    int numConta=0;
    int numTitulares=0;
    char modalidade = ' ';
    int opc=0;

    int feito=0;//se for 1 significa que já realizou a tarefa, se for 0 significa que ainda não realizou a tarefa
    system("cls"); // Limpa o ecran
    setlocale(LC_ALL, "Portuguese"); // Permite a acentuação
    printf("\n Processo de edição de conta...");
    printf("\n Insira o seu numero de conta: \t");
    printf("\n");
    scanf("%d", &numConta);

    for (int i = 0; i < CONTAS_MAX; i++) //percorre as contas
    {
        if (b->contasBanco[i].numeroConta == numConta) // verifica se o numero de conta inserido corresponde ao da conta
        {
            printf("\n\n----------Editar Conta----------\n\n");

            printf("\n Escolha uma opção:\t");
            printf("\n [1]. Editar modalidade");
            printf("\n [2]. Editar Titulares");
            printf("\n:");
            scanf("%d", &opc);
            fflush(stdin);
            printf("\n");
            do
            {

                if(opc == 1)
                {

                    printf("\n Editar Modalidade (n ou i):\t");
                    scanf(" %c", &modalidade);
                    fflush(stdin);

                    // system("pause");
                    if(modalidade== 'i')
                    {
                        b->contasBanco[i].modalidade = 'i';
                        feito=1;
                    }
                    else
                    {
                        b->contasBanco[i].modalidade = 'n';
                        feito=1;
                    }
                    break;

                }
                //esta função contém bugs e não está a funcionar, não foi encontrada solução por parte do grupo
                else if (opc == 2)
                {

                    int sair=0;
                    int nTitulares=0;
                    do
                    {

                        for(int h=0; h<5; h++)
                        {
                            if(b->contasBanco[i].titulares[h].nif==0)
                            {
                                nTitulares++;
                            }
                        }
                        if(nTitulares<5)
                        {
                            do
                            {
                                printf("Introduza numero de novos titulares (max %d):\n", TITULARES_MAX);
                                scanf("%d", &numTitulares);
                                if((nTitulares+numTitulares)<5)
                                {
                                    sair=1;
                                }

                            }
                            while(sair==1);

                            for(int j=0; j<numTitulares; j++)
                            {
                                fflush(stdin);
                                printf("\tAdicionar nome do novo Titular\t: ");
                                gets(b->contasBanco[i].titulares[i].nome);
                                printf("Adicione um NIF: \t");
                                scanf("%ld",b->contasBanco[i].titulares[i].nif);

                            }

                            fflush(stdin);

                            break;
                            if(numTitulares < 1 || numTitulares > TITULARES_MAX)
                            {
                                printf("\tERRO: deve introduzir entre 1 e %d titulares\n\n", TITULARES_MAX);
                            }
                        }
                        else
                        {
                            printf("Esta conta já tem 5 titulares");
                        }

                    }
                    while(numTitulares < 1 || numTitulares > TITULARES_MAX);
                    break;

                }

                else
                {

                    printf("\n Insira uma opção válida");

                }


            }
            while (opc <1 || opc > 2);


        }
        system("pause");
        if(feito==1)
        {

            break;
        }

    }
}




void eliminarConta(Banco *b)
{
    system("cls");
    setlocale(LC_ALL, "Portuguese");

    int contaEliminada=0;
    int posicaoArray=0;
    int nif=0;

    printf("\nInsira o número da conta que deseja eliminar:\t ");
    scanf("%d", &contaEliminada);





    for(int i=0; i<CONTAS_MAX; i++)
    {
        printf("\n Insira o seu NIF para validar a sua identidade:\t");
        scanf("%d", &nif);
        if(nif == b->contasBanco[i].titulares[i].nif) // verifica o nif da conta a ser elininada
        {

            if(b->contasBanco[i].numeroConta == contaEliminada) // verifica o numero da conta a ser eliminada
            {

                posicaoArray = i; // iguala a posicao i do array ao index
            }

            printf("\n Processo de eliminação de conta inciado...");

            for(int j = posicaoArray; j <CONTAS_MAX; j++)
            {
                b->contasBanco[j] = b->contasBanco[j+1]; // iguala o index a j, eliminando a conta
            }
            break;
        }
        else
        {

            printf("\n ERRO! O seu NIF esta errado, a voltar para o menu principal...");
            break;
        }


        system("pause");
        system("cls");


    }
}

