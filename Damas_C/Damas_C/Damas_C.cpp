// Damas.cpp : Defines the entry point for the console application.

//Alunos: Tayná C. Tinôco, n° 10729
//			Vitor Meira, °n11554


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)


//array que desenha o tabuleiro
int tabuleiro[8][8]; // 0 1 ou 2

typedef struct jogador
{
	char nome[10];
	int turno;//registra o turno em que foi feita a jogada	
	int coordxO;//cordenada da origem
	int coordyO;
	int coordxD;//coordenada destino
	int coordyD;
	char tipo; // N- normal || D-dama

	struct jogador *seguinte;//ultima jogada realizada
	struct jogador *anterior;//jogada realizada anterior a ultima jogada

}*Player;

typedef struct  movimentos
{
	char nome[10];
	int x;
	int y;
	int turno;
	char tipo;

	struct movimentos *nomal1;
	struct movimentos *nomal2;
	struct movimentos *dama1;
	struct movimentos *dama2;

}*Movimentos;


///Armazena os movimentos de cada jogador na lista ligada do tipo Player
Player inserirMovimento(Player jog, char nomePlayer[10], char tipo, int origX, int origY, int destX, int destY, int round) {

	Player novo = (Player)malloc(sizeof(struct jogador));
	strcpy(novo->nome, nomePlayer);
	novo->turno = round;
	novo->coordxO = origX;
	novo->coordyO = origY;
	novo->coordxD = destX;
	novo->coordyD = destY;
	novo->tipo = tipo;

	if (jog == NULL) {
		novo->seguinte = NULL;
		novo->anterior = NULL;

		tabuleiro[destX][destY] = tabuleiro[origX][origY];
		tabuleiro[origX][origY] = 0;


		return novo;
	}
	else
	{
		novo->anterior = jog;
		novo->seguinte = NULL;
		jog->seguinte = novo;

		tabuleiro[destX][destY] = tabuleiro[origX][origY];
		tabuleiro[origX][origY] = 0;

		return novo;

	}
}

///constroi arvore com os movimentos possiveis que o jogador pode fazer
Movimentos inserirMovimentoPossivel(Movimentos apt, int x, int y, char tipo, char nome[10], int turno) {

	if (apt == NULL) {
		Movimentos novo = (Movimentos)malloc(sizeof(struct movimentos));

		novo->x = x;
		novo->y = y;
		strcpy(novo->nome, nome);
		novo->turno = turno;

		novo->nomal1 = NULL;
		novo->nomal2 = NULL;
		novo->dama1 = NULL;
		novo->dama2 = NULL;

		return novo;
	}

	if ((apt->tipo == 'N')|| (apt->tipo == 'D')) {
		if (x < apt->x) {
			apt->nomal1 = inserirMovimentoPossivel(apt->nomal1, x, y, tipo, nome, turno);
			return apt;
		}
		else {
			apt->nomal2 = inserirMovimentoPossivel(apt->nomal2, x, y, tipo, nome, turno);
			return apt;
		}
	}
	if (apt->tipo == 'D') {

		if (x < apt->x) {
			apt->dama1 = inserirMovimentoPossivel(apt->dama1, x, y, tipo, nome, turno);
			return apt;
		}
		else {
			apt->dama2 = inserirMovimentoPossivel(apt->dama2, x, y, tipo, nome, turno);
			return apt;
		}
	}

}

///muda o tipo da peça para dama
void fazseDama(Player apt, int x, int y) {
	while ((apt->coordxD != x) && (apt->coordyD != y))
	{
		apt = apt->seguinte;
	}

	apt->tipo = 'D';
}

///Desfaz ultimas jogadas do adversario
Player undo(Player apt, int trn) {

	while (apt != NULL || apt->turno != trn - 1) {
		Player aux = apt;
		tabuleiro[apt->coordxO][apt->coordyO] = tabuleiro[apt->coordxD][apt->coordyD];
		tabuleiro[apt->coordxD][apt->coordyD] = 0;
		apt = apt->anterior;

		if (apt != NULL)
			apt->seguinte = NULL;
		free(aux);
	}
	return apt;
}


///condiciona os movimentos, de acordo com a disponibilidades das casas. Adicionando os possiveis a arvore
void possibleMoves(Movimentos tree, int x, int y, char tipo, char nome [10], int turno) {

	int contador = 0;

	if (tabuleiro[x][y] == 1) {

		if (tabuleiro[x + 1][y + 1] == 0)
			inserirMovimentoPossivel(tree, x + 1, y + 1, tipo, nome, turno);
			
		if (tabuleiro[x - 1][y + 1] == 0)
			inserirMovimentoPossivel(tree, x + 1, y + 1, tipo, nome, turno);
				
		while (contador < 5) {
			//recorre 5 vezes a função 		
			
			possibleMoves(tree, x + 1, y + 1, tipo, nome, turno);
			possibleMoves(tree, x - 1, y + 1, tipo, nome, turno);
			contador++;
		}
	}

	if (tabuleiro[x][y] == 2) {

		if (tabuleiro[x + 1][y - 1] == 0)
			inserirMovimentoPossivel(tree, x + 1, y + 1, tipo, nome, turno);
				
		if (tabuleiro[x - 1][y - 1] == 0)
			inserirMovimentoPossivel(tree, x + 1, y + 1, tipo, nome, turno);
		

		while (contador < 5) {//recorre 5 vezes a função 
			possibleMoves(tree, x - 1, y - 1, tipo, nome, turno);
			possibleMoves(tree, x + 1, y - 1, tipo, nome, turno);
			contador++;
		}
	}

}

/*
void formarCaminhos(Movimentos apt,Player aptLista, char jogadorAtual[10], int x, int y, int trn) {

	Player aux = aptLista;
	
	while (strcmp(aux->nome, jogadorAtual) != 0)
	{
		aux = aux->seguinte;
	}

	if (strcmp(jogadorAtual, nomeJog1)==0) {
		if (tabuleiro[x + 1][y + 1] == 0) {
			inserirPosição(apt, x, y, x + 1, y + 1, jogadorAtual, trn);
			formarCaminhos(apt, x + 1, y + 1, trn);
		}
		if (tabuleiro[x - 1][y + 1] == 0) {
			inserirPosição(apt, x, y, x - 1, y + 1, jogadorAtual, trn);
			formarCaminhos(apt, x - 1, y + 1, trn);
		}
		if (tabuleiro[x + 1][y + 1] == 2 && tabuleiro[x + 2][y + 2] == 0) {
			inserirPosição(apt, x, y, x + 2, y + 2, jogadorAtual, trn);
			formarCaminhos(apt, x + 2, y + 2, trn);
		}
		if (tabuleiro[x - 1][y + 1] == 2 && tabuleiro[x - 2][y + 2] == 0) {
			inserirPosição(apt, x, y, x - 2, y + 2, jogadorAtual, trn);
			formarCaminhos(apt, x - 2, y + 2, trn);
		}
	}
	else {
		if (tabuleiro[x + 1][y - 1] == 0) {
			inserirPosição(apt, x, y, x + 1, y - 1, jogadorAtual, trn);
			formarCaminhos(apt, x + 1, y - 1, trn);
		}
		if (tabuleiro[x - 1][y - 1] == 0) {
			inserirPosição(apt, x, y, x - 1, y - 1, jogadorAtual, trn);
			formarCaminhos(apt, x - 1, y - 1, trn);
		}
		if (tabuleiro[x + 1][y - 1] == 1 && tabuleiro[x + 2][y - 2] == 0) {
			inserirPosição(apt, x, y, x + 2, y - 2, jogadorAtual, trn);
			formarCaminhos(apt, x + 2, y - 2, trn);
		}
		if (tabuleiro[x - 1][y - 1] == 1 && tabuleiro[x - 2][y - 2] == 0) {
			inserirPosição(apt, x, y, x - 2, y - 2, jogadorAtual, trn);
			formarCaminhos(apt, x - 2, y - 2, trn);
		}
	}
}
*/

///gera matriz para o tabuleiro 
void criarTabuleiro() {
	int i, j;
	for (i = 0; i <= 7; i++) {
		for (j = 0; j <= 7; j++)
			tabuleiro[i][j] = '0';
	}
	for (int i = 0; i <= 2; i++) {
		if (i == 0 || i == 2) {
			for (j = 1; j <= 7; j = j + 2)
				tabuleiro[i][j] = '1';
		}
		else {
			for (j = 0; j <= 7; j = j + 2)
				tabuleiro[i][j] = '1';
		}
	}
	for (i = 5; i <= 7; i++) {
		if (i == 5 || i == 7) {
			for (j = 0; j <= 7; j = j + 2)
				tabuleiro[i][j] = '2';
		}
		else {
			for (j = 1; j <= 7; j = j + 2)
				tabuleiro[i][j] = '2';
		}
	}
}

void fazerMovimento(int x0, int y0, int xf, int yf) {
	int peca;

	peca = tabuleiro[x0][y0];
	tabuleiro[xf][yf] = peca;
	tabuleiro[x0][y0] = 0;

}

void mostrarTabuleiro() {
	int i, j;
	int x = 0;
	printf("\t0\t1\t2\t3\t4\t5\t6\t7\t\n");
	printf("\t---------------------------------------------------------\n");
	//	printf("_");
	for (i = 0; i <= 7; i++) {
		printf("%d|\t", x++);
		for (j = 0; j <= 7; j++)
			//printf("%d|\n", j);
			printf("%c\t", tabuleiro[i][j]);
		printf("\n");
	}
}



int vencerPartida() {
	int i, j;
	int contadorJog1 = 0;
	int contadorJog2 = 0;
	for (i = 0; i <= 8; i++) {
		for (j = 0; j <= 8; j++)
			if (tabuleiro[i][j] == '1')
				contadorJog1++;
			else if (tabuleiro[i][j] == '2')
				contadorJog2++;
	}
	if (contadorJog1 == 0) {
		return 1;
	}
	else if (contadorJog2 == 0) {
		return 2;
	}
	else
		return 0;
}



int main()
{
	criarTabuleiro();
	mostrarTabuleiro();

	Player jogo = NULL;

	int turno = 1;
	int fim = 0;
	int x0, y0, xf, yf, jAtual = 0;
	char jogador1[10], jogador2[10], jogadorAtual[10];


	puts(" ");
	do
	{
		if (turno == 1) {
			printf("=== JOGADOR 1 insira o nome ===\n");
			scanf("%s", jogador1);
			printf("=== JOGADOR 2 insira o nome ===\n");
			scanf("%s", jogador2);
			
			system("cls");
			fflush(stdin);
		}

		strcpy(jogadorAtual, jogador1);

		if (jAtual > 2) {
			strcpy(jogadorAtual, jogador1);
			jAtual = 0;
		}
		if (jAtual == 1)
			strcpy(jogadorAtual, jogador2);

		printf("======================================================\n");
		printf("TURNO: %d\nJOGADOR: %s", turno, jogadorAtual);
		puts(" ");
		mostrarTabuleiro();

		printf("==== Insira um movimento ===\n");
		printf("\t ESCOLHA A CORDENADA DA PEÇA A MOVIMENTAR [x0, y0]\n");
		printf("\t x0 = \n");
		scanf("%d", &x0);
		printf("\t y0 = \n");
		scanf("%d", &y0);
		printf("\t ESCOLHA A CORDENADA O DESTINO DA PEÇA [xf, yf]");

		scanf("%d %d", &xf, &yf);
		fflush(stdin);

		//fazerMovimento(x0)
		turno++;

		mostrarTabuleiro();
		printf("FIM DE JOGADA ? [1-sim, 0-não]\n");
		scanf("%d", &fim);

		fflush(stdin);
		jAtual++;

		if (vencerPartida() == 1)
		{
			system("cls");
			printf("PARABéNS O JOGADOR 1 VENCEU A PARTIDA!!!");
			break;
		}
		if (vencerPartida() == 2)
		{
			system("cls");
			printf("PARABéNS O JOGADOR 2 VENCEU A PARTIDA!!!");
			break;
		}

	} while (vencerPartida() == 0);


	getchar();
	return 0;
}

