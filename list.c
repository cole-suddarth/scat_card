#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include<math.h>


void readShuffleFile(FILE* inp, card** headp, card** tailp) {
  card* temp;
  card* previous;
  //reads cards into a double-linked list from user file
  while (!feof(inp)) {
    temp = (card*)malloc(sizeof(card));
    fscanf(inp, "%d %c", &temp->value, temp->suit);
    if (*temp->suit == 's') {
      strcpy(temp->suit, "Spades");
    }
    else if (*temp->suit == 'h') {
      strcpy(temp->suit, "Hearts");
    }
    else if (*temp->suit == 'd') {
      strcpy(temp->suit, "Diamonds");
    }
    else if (*temp->suit == 'c') {
      strcpy(temp->suit, "Clubs");
    }

    //similar to Lazos list creation main but signifcant differences to create a double-linked list
    if (*headp == NULL) {
      temp->ptBackward = NULL;//no elements which point forward
      *headp = temp;//initializes the start of linked list if it did not exist
    }
    else {
      (temp)->ptBackward = *tailp;//element being added point backwards(toward headp) to the previous end
      (*tailp)->ptForward = temp;//element that was last points forward(towards tailp) to the element being added
    }
    temp->ptForward = NULL;//sets the element being added point forward(towards tailp) to null because it is the most recent element
    *tailp = temp;//sets new tail pointer to most recent card added
	}
  fclose(inp);
}





void dealCard(int i, card* playerHand, card** mainHeadP) {
  card* temp = *mainHeadP;//assigns variable temp with address of current top card of deck
  playerHand[i] = *(*mainHeadP);//adds the top card from main deck to players hand
  playerHand[i].ptForward = NULL;//remove connection to linked list when moved to hand
  playerHand[i].ptBackward = NULL;//remove connection to linked list when moved to hand
  *mainHeadP = (*mainHeadP)->ptForward;//assigns mainHead pointer to the next card
  //not sure how to get full points using free
}





void printHand(card* playerHand) {
  //prints the suit and value of each card in players hand
  for (int i = 0; i < 3; i++) {
    if (playerHand[i].value == 1) {
      printf("%d. Ace of %s\n", (i+1), playerHand[i].suit);
    }
    else if (playerHand[i].value == 11) {
      printf("%d. Jack of %s\n", (i+1), playerHand[i].suit);
    }
    else if (playerHand[i].value == 12) {
      printf("%d. Queen of %s\n", (i+1), playerHand[i].suit);
    }
    else if (playerHand[i].value == 13) {
      printf("%d. King of %s\n", (i+1), playerHand[i].suit);
    }
    else {
      printf("%d. %d of %s\n", (i+1), playerHand[i].value, playerHand[i].suit);
    }
  }
}


void discardDrawnCard(card** chosenDeckHeadP, card** discardHeadP, card** discardTailP) {
  //adding discarded card to bottom of discard deck
  card* temp = *chosenDeckHeadP;//sets temp equal to the head pointer of chosen deck
  if (*discardHeadP == NULL) {//if there is no card in the discard pile
    *chosenDeckHeadP = temp->ptForward;//assigns drawn deck pointer(main) to the next card
    temp->ptForward = NULL;//no card to point forwards(towards TailPointer) to
    temp->ptBackward = NULL;//no card to point backwards(toward headP) to
    *discardHeadP = temp;//cards will be added to the bottom of discard deck
  }
  else if (*discardHeadP == temp){
    *chosenDeckHeadP = temp->ptForward;//top goes to next card in deck
    (*chosenDeckHeadP)->ptBackward = NULL;//new head pointer for chosen deck does not point towards head pointer bc it is headpointer
    temp->ptBackward = *discardTailP;//card being discarded goes to the bottom
  }
  else {
    *chosenDeckHeadP = temp->ptForward;//assigns drawn deck pointer(main) to the next card
    temp->ptBackward = *discardTailP;//sets point backward(toward headP) equal to the current bottom card of discard pile
    (*discardTailP)->ptForward = temp;//set point forward(toward tailP) of current bottom card equal to new bottom card
  }
  temp->ptForward = NULL;//sets temp(card being discarded) point forward(toward tailP) to nothing since it is now the bottom of deck
  *discardTailP = temp;//changes the card discarded to the head pointer or top
  //don't quite know how to get full points using the free
}


void discardPlayerCard(int i, card* playerHand, card** discardHeadP, card** discardTailP) {
  //adding discarded card to bottom of discard deck
  if (*discardHeadP == NULL) {//if there is no card in the discard pile
    (&playerHand[i - 1])->ptForward = NULL;//address of discard card has no card to point forwards(towards TailPointer) to
    (&playerHand[i - 1])->ptBackward = NULL;//address of discard card has no card to point backwards(towards HeadPointer) to
    *discardHeadP = &playerHand[i - 1];//changes the card discarded to the head pointer or top
  }
  else {
    (&playerHand[i - 1])->ptForward = NULL;//sets address of discard card point forward(toward tailP) equal to NULL since it is going on bottom of discardd pile
    (*discardTailP)->ptForward = &playerHand[i - 1];//sets address of discard card point forward(toward tailP) of current top card equal to the address of discarded card
  }
  (&playerHand[i - 1])->ptBackward = *discardTailP;//sets address of card being discarded point backward(toward headP) equal to nothing since it is now top card
  *discardTailP = (&playerHand[i - 1]);//cards will be added to the bottom
}


//link to test placeBets function   https://replit.com/join/nllgrhkb-ajjamesjames     manually change variable amounts to test different scenarios
int initialBets(int turn, int *player1Money, int *player2Money) {
  int initialBet = 0, maxAmount, pot, raiseAmount;
  char callRaiseFold[6], callFold[5];


  if( player1Money > player2Money){//makes sure players cannot bet more than the last place player can put into pot
    maxAmount = *player2Money;
  }
  else{
    maxAmount = *player1Money;
  }

  if (turn % 2 == 0){//player1 deals and player2 starts bets
    printf("Player2, how much would you like to bet?\n");
    scanf("%d", &initialBet);
    while(initialBet > maxAmount){
      printf("Bet must be $%d or lower\n", maxAmount);
      scanf("%d", &initialBet);
    }
    while(initialBet <= 0){
      printf("Initial bet must be greater than 0\n");
      scanf("%d", &initialBet);
    }
    
    while( (strcmp(callRaiseFold, "call") != 0) && (strcmp(callRaiseFold, "fold") != 0) && (strcmp(callRaiseFold, "raise") != 0) ){//player1 chooses one of the three options
      printf("Player1, the initial bet is $%d. Would you like to call, raise or fold?\n", initialBet);
      scanf("%s", callRaiseFold);
    }
    if((strcmp(callRaiseFold, "raise") == 0) && (initialBet == maxAmount)){//prevents a bug if the initial bet is the max allowed and player1 chooses to raise
      printf("Cannot raise above $%d, player1 has called\n", initialBet);
      pot = initialBet * 2;
      *player1Money = *player1Money - initialBet;
      *player2Money = *player2Money - initialBet;
    }
    else if(strcmp(callRaiseFold, "call") == 0){//if player1 calls
      pot = initialBet * 2;
      *player1Money = *player1Money - initialBet;
      *player2Money = *player2Money - initialBet;
    }
    else if(strcmp(callRaiseFold, "fold") == 0){//if player1 folds
      pot = 0;
    }
    else{// if player1 raises
      printf("Player1, what would you like to raise to?\n");
      scanf("%d", &raiseAmount);
      while ((raiseAmount <= initialBet) || (raiseAmount > maxAmount)){ //checks if the raise value is higher than the initial bet and lower than the max bet
        if(raiseAmount <= initialBet){
        printf("You must bet more than $%d\n", initialBet);
        scanf("%d", &raiseAmount);
        }
        else if(raiseAmount > maxAmount){
          printf("Bet must be $%d or lower\n", maxAmount);
          scanf("%d", &raiseAmount);
        }
      }
      while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){ //Player2 chooses one of the two options
        printf("Player2, the bet has been raised to $%d. Would you like to call or fold?\n", raiseAmount);
        scanf("%s", callFold);
      }
      if(strcmp(callFold, "call") == 0){//Player2 calls
        pot = raiseAmount * 2;
        *player1Money = *player1Money - raiseAmount;
        *player2Money = *player2Money - raiseAmount;
      }
      else{//player2 folds, Player 1 wins the initial bet and player 2 loses the initial bet
        pot = 0;
        *player1Money = *player1Money + initialBet;
        *player2Money = *player2Money - initialBet;
      }
    }
  }
  else { //player2 deals and player1 starts betting
    printf("Player1, how much would you like to bet?\n");
    scanf("%d", &initialBet);
    while(initialBet > maxAmount){
      printf("Bet must be $%d or lower\n", maxAmount);
      scanf("%d", &initialBet);
    }
    while(initialBet <= 0){
      printf("Initial bet must be greater than 0\n");
      scanf("%d", &initialBet);
    }
    
    while( (strcmp(callRaiseFold, "call") != 0) && (strcmp(callRaiseFold, "fold") != 0) && (strcmp(callRaiseFold, "raise") != 0) ){//player2 chooses one of the three options
      printf("Player2, the initial bet is $%d. Would you like to call, raise or fold?\n", initialBet);
      scanf("%s", callRaiseFold);
    }
    if((strcmp(callRaiseFold, "raise") == 0) && (initialBet == maxAmount)){//prevents a bug if the initial bet is the max allowed and player2 chooses to raise
      printf("Cannot raise above $%d, player2 has called\n", initialBet);
      pot = initialBet * 2;
      *player1Money = *player1Money - initialBet;
      *player2Money = *player2Money - initialBet;
    }
    else if(strcmp(callRaiseFold, "call") == 0){//if player2 calls
      pot = initialBet * 2;
      *player1Money = *player1Money - initialBet;
      *player2Money = *player2Money - initialBet;
    }
    else if(strcmp(callRaiseFold, "fold") == 0){//if player2 folds
      pot = 0;
    }
    else{// if player2 raises
      printf("Player2, what would you like to raise to?\n");
      scanf("%d", &raiseAmount);
      while ((raiseAmount <= initialBet) || (raiseAmount > maxAmount)){ //checks if the raise value is higher than the initial bet and lower than the max bet
        if(raiseAmount <= initialBet){
        printf("You must bet more than $%d\n", initialBet);
        scanf("%d", &raiseAmount);
        }
        else if(raiseAmount > maxAmount){
          printf("Bet must be $%d or lower\n", maxAmount);
          scanf("%d", &raiseAmount);
        }
      }
      while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){ //Player1 chooses one of the two options
        printf("Player1, the bet has been raised to $%d. Would you like to call or fold?\n", raiseAmount);
        scanf("%s", callFold);
      }
      if(strcmp(callFold, "call") == 0){//Player1 calls
        pot = raiseAmount * 2;
        *player1Money = *player1Money - raiseAmount;
        *player2Money = *player2Money - raiseAmount;
      }
      else{//player1 folds, Player 2 wins the initial bet and player 1 loses the initial bet
        pot = 0;
        *player1Money = *player1Money - initialBet;
        *player2Money = *player2Money + initialBet;
      }
    }
  }
  return pot;
}


// test for this function is    https://replit.com/join/dvdqsppo-ajjamesjames
void turnBets(int turn, int *player1Money, int *player2Money, int *pot){//takes bets at the beginning of each round of turns
  int currentBet, newBet, newBet2, maxAmount, initialPlayer1Money, initialPlayer2Money;
  char raiseCall[6] = " ", raiseCall2[6] = " ", callFold[5] = " ", callRaiseFold[6] = " ";
  
  currentBet = *pot/2;// the amount each player has put in so far
  initialPlayer1Money = currentBet + *player1Money; // the amount each player had at the beginning of the round
  initialPlayer2Money = currentBet + *player2Money;

  if( initialPlayer1Money > initialPlayer2Money){//makes sure players cannot bet more than the last place player can put into pot
    maxAmount = initialPlayer2Money;
  }
  else{
    maxAmount = initialPlayer1Money;
  }

  if( turn % 2 == 0){//player2 is the first one to play/bet each round
    printf("The current bet is $%d, Player2 would you like to call or raise?\n", currentBet);
    while( (strcmp(raiseCall, "call") != 0) && (strcmp(raiseCall, "raise") != 0) ){
      scanf("%s", raiseCall);
    }
    if(strcmp(raiseCall, "call") == 0){//if player2 calls
      printf("Player2 has called, Player1 the current bet is $%d, would you like to call or raise?\n", currentBet);
      while( (strcmp(raiseCall2, "call") != 0) && (strcmp(raiseCall2, "raise") != 0) ){
        scanf("%s", raiseCall2);
      }
      if(strcmp(raiseCall2,"call") == 0){//Player1 calls
        printf("Player1 has called, all bets remain the same.\n");
      }
      else{//player1 raises
        printf("Player1 what would you like to raise your bet to?\n");
        scanf("%d", &newBet);
        while( (newBet <= currentBet) || (newBet > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
          if (newBet <= currentBet){
            printf("Bet must be greater than $%d.\n", currentBet);
            scanf("%d", &newBet);
          }
          else{
            printf("Bet cannot be greater than $%d.\n", maxAmount);
            scanf("%d", &newBet);
          }
        }
        printf("Player1 has raised the bet to $%d. Player2, would you like to call or fold?", newBet);
        scanf("%s", callFold);
        while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){
          scanf("%s", callFold);
        }
        if ( strcmp(callFold, "call") == 0 ){//if player2 calls
          *pot = newBet * 2;
          *player1Money = *player1Money - (newBet - currentBet);
          *player2Money = *player2Money - (newBet - currentBet);
        }
        else{ //player2 folds
          *player1Money += *pot;
          *pot = 0;
        }
      }
    }
    else{//player2 raises
      printf("Player2, what would you like to raise the bet to? \n");
      scanf("%d", &newBet);
      while( (newBet <= currentBet) || (newBet > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
        if (newBet <= currentBet){
          printf("Bet must be greater than $%d.\n", currentBet);
          scanf("%d", &newBet);
        }
        else{
          printf("Bet cannot be greater than $%d.\n", maxAmount);
          scanf("%d", &newBet);
        }
      }
      while( (strcmp(callRaiseFold, "call") != 0) && (strcmp(callRaiseFold, "fold") != 0) && (strcmp(callRaiseFold, "raise") != 0) ){
        printf("Player2 has raised to $%d. Player1 would you like to call, raise or fold?\n", newBet);
        scanf("%s", callRaiseFold);
      }
      if (strcmp(callRaiseFold, "call") == 0){// player1 calls
        printf("Player1 has called.\n");
        *pot = newBet * 2;
        *player1Money = *player1Money - (newBet - currentBet);
        *player2Money = *player2Money - (newBet - currentBet);
      }
      else if(strcmp(callRaiseFold, "raise") == 0){// player1 raises
        if(newBet == maxAmount){// prevents a bug if Player1 raises but cannot due to the newbet being the max allowed
          printf("Cannot raise above $%d, player1 has called\n", newBet);
          *pot = newBet * 2;
          *player1Money = *player1Money - (newBet - currentBet);
          *player2Money = *player2Money - (newBet - currentBet);
        }
        else{
          printf("Player1 what would you like to raise your bet to?\n");
          scanf("%d", &newBet2);
          while( (newBet2 <= newBet) || (newBet2 > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
            if (newBet <= currentBet){
              printf("Bet must be greater than $%d.\n", newBet);
              scanf("%d", &newBet);
            }
            else{
              printf("Bet cannot be greater than $%d.\n", maxAmount);
              scanf("%d", &newBet);
            }
          }
          printf("Player1 has raised the bet to $%d. Player2, would you like to call or fold?", newBet2);
          scanf("%s", callFold);
          while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){
            scanf("%s", callFold);
          }
          if ( strcmp(callFold, "call") == 0){//if player2 calls
            *pot = newBet2 * 2;
            *player1Money = *player1Money - (newBet2 - currentBet);
            *player2Money = *player2Money - (newBet2 - currentBet);
          }
          else{ //player2 folds
            *player1Money = *player1Money + *pot + (newBet - currentBet);
            *player2Money = *player2Money - (newBet - currentBet);
            *pot = 0;
          }
        }
      }
      else{// player1 folds
        *player2Money += *pot;
        *pot = 0;
      }
    }
  }
  else{//player1 is the first one to play/bet each round
    printf("The current bet is $%d, Player1 would you like to call or raise?\n", currentBet);
    while( (strcmp(raiseCall, "call") != 0) && (strcmp(raiseCall, "raise") != 0) ){
      scanf("%s", raiseCall);
    }
    if(strcmp(raiseCall, "call") == 0){//if player1 calls
      printf("Player1 has called, Player2 the current bet is $%d, would you like to call or raise?\n", currentBet);
      while( (strcmp(raiseCall2, "call") != 0) && (strcmp(raiseCall2, "raise") != 0) ){
        scanf("%s", raiseCall2);
      }
      if(strcmp(raiseCall2,"call") == 0){//Player2 calls
        printf("Player2 has called, all bets remain the same.\n");
      }
      else{//player2 raises
        printf("Player2 what would you like to raise your bet to?\n");
        scanf("%d", &newBet);
        while( (newBet <= currentBet) || (newBet > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
          if (newBet <= currentBet){
            printf("Bet must be greater than $%d.\n", currentBet);
            scanf("%d", &newBet);
          }
          else{
            printf("Bet cannot be greater than $%d.\n", maxAmount);
            scanf("%d", &newBet);
          }
        }
        printf("Player2 has raised the bet to $%d. Player1, would you like to call or fold?", newBet2);
        scanf("%s", callFold);
        while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){
          scanf("%s", callFold);
        }
        if ( strcmp(callFold, "call") == 0 ){//if player1 calls
          *pot = newBet * 2;
          *player2Money = *player2Money - (newBet - currentBet);
          *player2Money = *player2Money - (newBet - currentBet);
        }
        else{ //player1 folds
          *player2Money += *pot;
          *pot = 0;
        }
      }
    }
    else{//player1 raises
      printf("Player1, what would you like to raise the bet to? \n");
      scanf("%d", &newBet);
      while( (newBet <= currentBet) || (newBet > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
        if (newBet <= currentBet){
          printf("Bet must be greater than $%d.\n", currentBet);
          scanf("%d", &newBet);
        }
        else{
          printf("Bet cannot be greater than $%d.\n", maxAmount);
          scanf("%d", &newBet);
        }
      }
      while( (strcmp(callRaiseFold, "call") != 0) && (strcmp(callRaiseFold, "fold") != 0) && (strcmp(callRaiseFold, "raise") != 0) ){
        printf("Player1 has raised to $%d. Player2 would you like to call, raise or fold?\n", newBet);
        scanf("%s", callRaiseFold);
      }
      if (strcmp(callRaiseFold, "call") == 0){// player2 calls
        printf("Player2 has called.\n");
        *pot = newBet * 2;
        *player2Money = *player2Money - (newBet - currentBet);
        *player1Money = *player1Money - (newBet - currentBet);
      }
      else if(strcmp(callRaiseFold, "raise") == 0){// player2 raises
        if(newBet == maxAmount){// prevents a bug if Player2 raises but cannot due to the newbet being the max allowed
          printf("Cannot raise above $%d, player2 has called\n", newBet);
          *pot = newBet * 2;
          *player2Money = *player2Money - (newBet - currentBet);
          *player1Money = *player1Money - (newBet - currentBet);
        }
        else{
          printf("Player2 what would you like to raise your bet to?\n");
          scanf("%d", &newBet2);
          while( (newBet2 <= newBet) || (newBet2 > maxAmount) ){// makes sure the raise amount is above previous amount and below max amount
            if (newBet <= currentBet){
              printf("Bet must be greater than $%d.\n", newBet);
              scanf("%d", &newBet);
            }
            else{
              printf("Bet cannot be greater than $%d.\n", maxAmount);
              scanf("%d", &newBet);
            }
          }
          printf("Player2 has raised the bet to $%d. Player1, would you like to call or fold?", newBet2);
          scanf("%s", callFold);
          while( (strcmp(callFold, "call") != 0) && (strcmp(callFold, "fold") != 0) ){
            scanf("%s", callFold);
          }
          if ( strcmp(callFold, "call") == 0 ){//if player1 calls
            *pot = newBet2 * 2;
            *player2Money = *player2Money - (newBet2 - currentBet);
            *player1Money = *player1Money - (newBet2 - currentBet);
          }
          else{ //player1 folds
            *player2Money = *player2Money + *pot + (newBet - currentBet);
            *player1Money = *player1Money - (newBet - currentBet);
            *pot = 0;
          }
        }
      }
      else{// player2 folds
        *player1Money += *pot;
        *pot = 0;
      }
    }
  }
}




int checkHand(card* playerHand) {
  int handValue = 0, i;
   
  if((strcmp(playerHand[0].suit, playerHand[1].suit) == 0) && (strcmp(playerHand[0].suit, playerHand[2].suit) == 0)){// if all three cards are the same suit
    for(i = 0; i < 3; i++){//checks all three cards because all three count for points
      if(playerHand[i].value >= 10){//checks for 10's, jacks, queens and kings which are all worth 10 points
        handValue = handValue + 10;
      }
      else if(playerHand[i].value == 1){//checks for aces which are worth 11 points
        handValue = handValue + 11;
      }
      else{//checks for value of card since 2 through 9 are worth their respective value
        handValue = handValue + playerHand[i].value;
      }
    }
  }
  // if the above if-statement does not execute, we know that not all three cards are the same suit, therefore we need to check if at least two match
  else if((strcmp(playerHand[0].suit, playerHand[1].suit) == 0)){// if two cards are the same suit
    if( playerHand[0].value == 1){//checks value of first card
      handValue += 11;
    }
    else if(playerHand[0].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[0].value;
    }
    if( playerHand[1].value == 1){// chacks value of second card
      handValue += 11;
    }
    else if(playerHand[1].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[1].value;
    }
  }
  else if((strcmp(playerHand[0].suit, playerHand[2].suit) == 0)){// if two cards are the same suit
    if( playerHand[0].value == 1){//checks value of first card
      handValue += 11;
    }
    else if(playerHand[0].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[0].value;
    }
    if( playerHand[2].value == 1){// chacks value of second card
      handValue += 11;
    }
    else if(playerHand[2].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[2].value;
    }
  }
  else if((strcmp(playerHand[1].suit, playerHand[2].suit) == 0)){// if two cards are the same suit
    if( playerHand[1].value == 1){//checks value of first card
      handValue += 11;
    }
    else if(playerHand[1].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[1].value;
    }
    if( playerHand[2].value == 1){// chacks value of second card
      handValue += 11;
    }
    else if(playerHand[2].value >= 10){
      handValue += 10;
    }
    else{
      handValue += playerHand[2].value;
    }
  }
  // if none of these else if statements execute, we know all three cards are different suits, so we need to check if all three cards have the same value
  
  else if((playerHand[0].value == playerHand[1].value) && (playerHand[0].value == playerHand[2].value)){//if all three cards have the same value
    handValue = 30;
  }
  else{//if all the above is false, we know that the hand is worth the single highest card value
    //ace is the highest value card, so we'll check that first, followed by 10 and face cards, followed by cards 2-9
    for(i = 0; i < 3; i++){
      if(playerHand[i].value == 1){//aces
        handValue = 11;
      }
      else if((playerHand[i].value >= 10) && (handValue != 11)){// 10's, jacks, queens, kings and checks if there is no aces
        handValue = 10;
      }
      else if((playerHand[i].value > handValue)){// finds the max of the face cards
        handValue = playerHand[i].value;
      }
    }
  }
  return handValue;
}





//THESE TWO FUNCTIONS WERE TAKEN FROM THE SWAP LAB< AND ADJUSTED TO SWITCH THE CARD AND SUIT
int rand_gen(int count) {
    double frac;
    frac = (double)rand()/((double)RAND_MAX+1); // gives a different frac every time is executed
    return floor(count * frac); //random index in [0, max]
}
void swap(card *headP, int i, int j) {
// swaps the contents of the ith and jth list element
    card *pt1 = headP, *pt2 = headP;
    char temp[9] = "\0";
    while(i>0){
        pt1=pt1->ptForward; // advance to the ith element
        i--;
    }
    while(j>0){
        pt2 = pt2->ptForward;  // advance to the jth element
        j--;
    }
    i = pt1->value; // temp card number
    strcpy(temp, pt1->suit);// temp card suit
    pt1->value = pt2->value;//swap card number for 1
    pt2->value = i;//swap card number for 2
    strcpy(pt1->suit, pt2->suit);//swap card suit for 1
    strcpy(pt2->suit, temp);//swap card suit for 2
}