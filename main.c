#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include<math.h>

//https://replit.com/@colesuddarth/List-creation-print-1#list.c for creation and deletion of nodes in list
//in cards.txt it goes number then suite, where 1 is ace, 11 is Jack, 12 is Queen, 13 is King

//discard linked list is the current problem right now


int main(void) {
  char userShuffle[15], comfortableWithHand = 'n', pilePick[8];
  FILE* inp = NULL;
  card* mainHeadP = NULL, * tailp = NULL, * discardHeadP = NULL, * discardTailP = NULL;
  card player1Hand[3], player2Hand[3];
  int player1Money = 100, player2Money = 100, dealTurn = 0, pot = 0, drawturn = 0, player1HandValue = 0, player2HandValue = 0, cardDiscard, j;

  srand((int)time(NULL));

  //playing of the game checks if they still have money
  while (player1Money > 0 && player2Money > 0) {
    printf("Please select to shuffle deck (shuffle) or enter a file with chosen card sequence (file name): ");
    scanf("%s", userShuffle);
    //check if wants to read a file for deck or shuffle deck
    strcat(userShuffle, "\0");
    //either shuffle or gets predetermined shuffled deck
    //open a file, read it into the linked list using the readShuffleFile function, and then if the user wants to shuffle, then we can do the swaps as shown in
    if (strcmp(userShuffle, "shuffle") == 0) {
      inp = fopen("cards.txt", "r");
      readShuffleFile(inp, &mainHeadP, &tailp);
      for (int i = 1; i < 1000; i++) {
        swap(mainHeadP, rand_gen(52), rand_gen(52));
      }
      rewind(inp);
    }
    else {//reads card order from file
      inp = fopen(userShuffle, "r");
      if (inp == NULL) {
        while (inp == NULL) {//incase the user enters in a typo, it won't reset the game
          printf("Could not open file\nEnter file name again:\n");
          scanf("%s", userShuffle);
          inp = fopen(userShuffle, "r");
        }
        readShuffleFile(inp, &mainHeadP, &tailp);
      }
      else {
        readShuffleFile(inp, &mainHeadP, &tailp);
      }
    }
    //if turn is even player 1 deals and player 2 starts betting, vice versa if turn is odd.
    //deals hands and removes card from mainDeck changing the head pointer of main deck
    if (dealTurn % 2 == 0) {//if turn is even player 1 deals
      for (int i = 0; i < 3; i++) {
        dealCard(i, player2Hand, &mainHeadP);
        dealCard(i, player1Hand, &mainHeadP);
      }
    }
    else {//if turn is odd player 2 deals
      for (int i = 0; i < 3; i++) {
        dealCard(i, player1Hand, &mainHeadP);
        dealCard(i, player2Hand, &mainHeadP);
      }
    }
    
    printf("Player 1, your cards are: \n");
    printHand(player1Hand);//prints first players hand
    printf("Player 2, your cards are: \n");
    printHand(player2Hand);//prints second players hand
    
    pot = initialBets(dealTurn, &player1Money, &player2Money);
    //printf("Player 1: $%d   Player 2: $%d  Pot: $%d\n", player1Money, player2Money, pot);
    player1HandValue = checkHand(player1Hand);
    player2HandValue = checkHand(player2Hand);
    //printf("Player1: %d points   Player2: %d points\n", player1HandValue, player2HandValue);
    //break;
    
    drawturn = dealTurn + 1;//sets the drawturn to the player who did not deal first
    j = 0;
    if (pot != 0 ) {// if player has not folded plays game
      while (checkHand(player1Hand) != 31 && checkHand(player2Hand)!= 31 && comfortableWithHand == 'n') {
        if (j % 2 == 0 && j != 0){
          turnBets(dealTurn, &player1Money, &player2Money, &pot);//does betting each round, except the first time when initial bets are placed for each game
        }
        j++;
        if(pot == 0){
          break;
        }
        if (drawturn % 2 == 0) {//checks if it is player1 turn to draw
          printf("Player 1 would you like to draw from discard or main pile?\n");//determines which pile to draw from
          scanf("%s", pilePick);
          while (strcmp(pilePick, "discard") != 0 && strcmp(pilePick, "main") != 0) {
            printf("Please type either 'discard' or 'main':\n");
            scanf("%s", pilePick);//ensures the user enters either 'discard' or 'main' for pile
          }
          if (strcmp(pilePick, "discard") == 0) {//if player1 chose to pick from the discard pile
            if (discardHeadP == NULL) {//if there are no cards in discard pile auto draw from main pile
              printf("There are currently no cards in the discard pile. You will now be drawn a card from the main pile.\n");
              printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);
              printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
              printf(", 2 to replace the second and 3 to replace the third:\n");
              scanf("%d", &cardDiscard);
              if (cardDiscard == 0) {//if they want to discard drawn card
                discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card from main pile
              }
              else {//if they want to discard a card from their hand
                discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
                dealCard(cardDiscard - 1, player1Hand, &mainHeadP);//deals card from main pile to players hand
              }
            }
            else {//if there is already an established discard pile
              printf("The card drawn from the discard pile is a %d of %s\n", discardHeadP->value, discardHeadP->suit);
              printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
              printf(", 2 to replace the second and 3 to replace the third:\n");
              scanf("%d", &cardDiscard);
              if (cardDiscard == 0) {//if they want to dsicard drawn card
                discardDrawnCard(&discardHeadP, &discardHeadP, &discardTailP);//function which discards card drawn from discard pile
              }
              else {//if they want to discard a card from their hand
                discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
                dealCard(cardDiscard - 1, player1Hand, &discardHeadP);//deals card from discard pile to players hand
              }
            }
          }
          else {//Chose to draw from main pile
            printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);//shows card drawn
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");//asks what the player would like to do
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {//discards the card drawn
              discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card
            }
            else {//discards player selected card, cardDiscard
              discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player1Hand, &mainHeadP);
            }
          }
        }
        else {//player 2 turn to draw the card
          printf("Player 2 would you like to draw from discard or main pile?\n");//asks which pile player would like to draw from
          scanf("%s", pilePick);
          while (strcmp(pilePick, "discard") != 0 && strcmp(pilePick, "main") != 0) {
            printf("Please type either 'discard' or 'main':\n");//ensures picks either 'discard' or 'main'
            scanf("%s", pilePick);
        }
          if (strcmp(pilePick, "discard") == 0) {//player chose to draw from discard pile
            if (discardHeadP == NULL) {//if there are no cards in discard pile auto draw from main pile
              printf("There are currently no cards in the discard pile. You will now be drawn a card from the main pile.\n");
              printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);
              printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
              printf(", 2 to replace the second and 3 to replace the third:\n");
              scanf("%d", &cardDiscard);
              if (cardDiscard == 0) {
                discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card from main pile
              }
              else {
                discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
                dealCard(cardDiscard - 1, player2Hand, &mainHeadP);//deals card from main pile to players hand
              }
            }
            else {//if there is already an established discard pile
              printf("The card drawn from the discard pile is a %d of %s\n", discardHeadP->value, discardHeadP->suit);
              printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
              printf(", 2 to replace the second and 3 to replace the third:\n");
              scanf("%d", &cardDiscard);
              if (cardDiscard == 0) {
                discardDrawnCard(&discardHeadP, &discardHeadP, &discardTailP);//function which discards card drawn from discard pile
              }
              else {
                discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
                dealCard(cardDiscard - 1, player1Hand, &discardHeadP);//deals card from discard pile to players hand
              }
            }
          }
          else {//Chose to draw from main pile
            printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);//shows card drawn
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");//asks what the player would like to do
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {//discards the card drawn
              discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card
            }
            else {//discards player selected card, cardDiscard
              discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player2Hand, &mainHeadP);
            }
          }
        }
        if (drawturn % 2 == 0) { //if player 1 drew, prints their new cards
          printf("Player 1, your current cards are: \n");
          printHand(player1Hand);
        }
        else { //if player 2 drew, prints their new cards
          printf("Player 2, you current cards are: \n");
          printHand(player2Hand);
        }
        printf("Are you comfortable with your hand?(y/n): ");
        do {
          scanf("%c", &comfortableWithHand);//scans if player is comfortable with their cards
        } while (comfortableWithHand != 'y' && comfortableWithHand != 'n');
        drawturn += 1;//changes the drawturn so next player draws
      }
    }
    //statements to allow other player to draw card, wont allow if a plyers hand is equal to 31
    if (checkHand(player1Hand) != 31 && checkHand(player2Hand) != 31 && pot != 0) {//checks if 31 was reached, and if player did not fold if not allows other player to draw
      if (drawturn % 2 == 0) {//checks if it is player1 turn to draw
        printf("Player 1 would you like to draw your final card from discard or main pile?\n");//determines which pile to draw from
        scanf("%s", pilePick);
        while (strcmp(pilePick, "discard") != 0 && strcmp(pilePick, "main") != 0) {
          printf("Please type either 'discard' or 'main':\n");
          scanf("%s", pilePick);//ensures the user enters either 'discard' or 'main' for pile
        }
        if (strcmp(pilePick, "discard") == 0) {//if player1 chose to pick from the discard pile
          if (discardHeadP == NULL) {//if there are no cards in discard pile auto draw from main pile
            printf("There are currently no cards in the discard pile. You will know be drawn a card from the main pile.\n");
            printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {//if they want to discard drawn card
              discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card from main pile
            }
            else {//if they want to discard a card from their hand
              discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player1Hand, &mainHeadP);//deals card from main pile to players hand
            }
          }
          else {//if there is already an established discard pile
            printf("The card drawn from the discard pile is a %d of %s\n", discardHeadP->value, discardHeadP->suit);
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {//if they want to dsicard drawn card
              discardDrawnCard(&discardHeadP, &discardHeadP, &discardTailP);//function which discards card drawn from discard pile
            }
            else {//if they want to discard a card from their hand
              discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player1Hand, &discardHeadP);//deals card from discard pile to players hand
            }
          }
        }
        else {//Chose to draw from main pile
          printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);//shows card drawn
          printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
          printf(", 2 to replace the second and 3 to replace the third:\n");//asks what the player would like to do
          scanf("%d", &cardDiscard);
          if (cardDiscard == 0) {//discards the card drawn
            discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card
          }
          else {//discards player selected card, cardDiscard
            discardPlayerCard(cardDiscard, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
            dealCard(cardDiscard - 1, player1Hand, &mainHeadP);
          }
        }
      }
      else {//player 2 turn to draw the card
        printf("Player 2 would you like to draw your final card from discard or main pile?\n");//asks which pile player would like to draw from
        scanf("%s", pilePick);
        while (strcmp(pilePick, "discard") != 0 && strcmp(pilePick, "main") != 0) {
          printf("Please type either 'discard' or 'main':\n");//ensures picks either 'discard' or 'main'
          scanf("%s", pilePick);
        }
        if (strcmp(pilePick, "discard") == 0) {//player chose to draw from discard pile
          if (discardHeadP == NULL) {//if there are no cards in discard pile auto draw from main pile
            printf("There are currently no cards in the discard pile. You will know be drawn a card from the main pile.\n");
            printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {
              discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card from main pile
            }
            else {
              discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player2Hand, &mainHeadP);//deals card from main pile to players hand
            }
          }
          else {//if there is already an established discard pile
            printf("The card drawn from the discard pile is a %d of %s\n", discardHeadP->value, discardHeadP->suit);
            printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
            printf(", 2 to replace the second and 3 to replace the third:\n");
            scanf("%d", &cardDiscard);
            if (cardDiscard == 0) {
              discardDrawnCard(&discardHeadP, &discardHeadP, &discardTailP);//function which discards card drawn from discard pile
            }
            else {
              discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
              dealCard(cardDiscard - 1, player1Hand, &discardHeadP);//deals card from discard pile to players hand
            }
          }
        }
        else {//Chose to draw from main pile
          printf("The card drawn from the main pile is a %d of %s\n", mainHeadP->value, mainHeadP->suit);//shows card drawn
          printf("Please enter a 0 if you want to discard this card, 1 if you would like to switch with the first card in your hand");
          printf(", 2 to replace the second and 3 to replace the third:\n");//asks what the player would like to do
          scanf("%d", &cardDiscard);
          if (cardDiscard == 0) {//discards the card drawn
            discardDrawnCard(&mainHeadP, &discardHeadP, &discardTailP);//function which discards drawn card
          }
          else {//discards player selected card, cardDiscard
            discardPlayerCard(cardDiscard, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
            dealCard(cardDiscard - 1, player2Hand, &mainHeadP);
          }
        }
      }
    }
    if (drawturn % 2 == 0) { //if player 1 drew, prints their new cards
      printf("Player 1, your current cards are: \n");
      printHand(player1Hand);
    }
    else { //if player 2 drew, prints their new cards
      printf("Player 2, you current cards are: \n");
      printHand(player2Hand);
    }
    player1HandValue = checkHand(player1Hand);//returns value(aka score) of player 1 hand
    player2HandValue = checkHand(player2Hand);//returns value(aka score) of player 2 hand
    if (player1HandValue > player2HandValue) {//if player 1 had greater score player 1 gets all money in pot
      printf("Player 1, your hand held a greater value, you won!!\n");
      player1Money = player1Money + pot;
    }
    else if (player1HandValue < player2HandValue){//if player 2 had greater score player 2 gets all money in pot
      printf("Player 2, your hand held a greater value, you won!!\n");
      player2Money = player2Money + pot;
    }
    else {//both players had the same score, results in a tie, they split the pot
      printf("You hands were equal the pot will be split equally!\n");
      pot = pot * 1 / 2;//splits the pot in half
      player1Money = player1Money + pot;
      player2Money = player2Money + pot;
    }
    printf("Player 1, you now have $%d\n", player1Money);
    printf("Player 2, you now have $%d\n", player2Money);
    for (int i = 1; i <= 3; i++) {//discard all cards from player 1 hand
      discardPlayerCard(i, player1Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
    }
    for (int i = 1; i <= 3; i++) {//dsicard all cards from player 2 hand
      discardPlayerCard(i, player2Hand, &discardHeadP, &discardTailP);//function which discards chosen card from players hand
    }
    dealTurn += 1;//end of round, other player deals
    comfortableWithHand = 'n';
  }

  return 0;
}