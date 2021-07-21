#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<math.h>

typedef struct card_s {
	char suit[9];
	int value;
	struct card_s* ptForward;
	struct card_s* ptBackward;
} card;

void readShuffleFile(FILE* inp, card** headp, card** tailp);

void dealCard(int i, card* playerHand, card** mainTailP);

void printHand(card* playerHand);

int initialBets(int turn, int *player1Money, int *player2Money);

void turnBets(int turn, int *player1Money, int *player2Money, int *pot);

int checkHand(card* playerHand);

void discardDrawnCard(card** chosenDeckHeadP, card** discardHeadP, card** discardTailP);

void discardPlayerCard(int i, card* playerHand, card** discardHeadP, card** discardTailP);

int rand_gen(int count);

void swap(card *mainHeadP, int i, int j);