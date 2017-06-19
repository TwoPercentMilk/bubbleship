// BubbleShip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BubbleShip.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>


int main()
{
	char* playerOneField;
	char* playerTwoField;
	int playerOneNumber = 1;
	int playerTwoNumber = 2;
	int attackResults = 0;
	int numberOfShips;
	char* numShipsStr;

	int coord = 0;

	
	printf("How many ships do you want? (choose number between 1-19)");
	numShipsStr = (char*)malloc(4);
	gets_s(numShipsStr, 4);
	numberOfShips = atoi(numShipsStr);
	if (numberOfShips > 19)
	{
		printf("You can't place that many ships!");
		return -1;
	}

	playerOneField = BuildField();
	playerTwoField = BuildField();

	PlaceShips(numberOfShips, playerOneNumber, playerOneField);
	PlaceShips(numberOfShips, playerTwoNumber, playerTwoField);



	while (1==1) //Loops forever
	{

		//Player One's turn
		PromptPlayerForCoords(playerOneNumber, &coord);
		attackResults = LaunchAttackAgainstPlayer(playerOneNumber, playerTwoNumber, playerTwoField, coord);

		printf("Player One sunk %d ships! \n", attackResults);
		if (attackResults == 9999)
		{
			printf("Player One Wins!");
			return 0; //Exit main function
		}


		//Player Two's turn
		PromptPlayerForCoords(playerTwoNumber, &coord);
		attackResults = LaunchAttackAgainstPlayer(playerTwoNumber, playerOneNumber, playerOneField, coord);

		printf("Player Two sunk %d ships! \n", attackResults);
		if (attackResults == 9999)
		{
			printf("Player Two Wins!");
			return 0; //Exit main function
		}
	}




    return -1; //Should never get here;

}

char* BuildField()
{
	char* field;
	int index;

	field = (char*)malloc(21);

	index = 0;

	while(index <= 19)
	{
		*(field + index) = 'o';
		index = index + 1;
	}
	*(field + index) = 0;


	return field;

}


int PromptPlayerForCoords(int playerNumber, int* xpos)
{
	
	char* xCoord;

	printf("Player %d, enter x coord ", playerNumber);
	xCoord = (char*)malloc(3);
	gets_s(xCoord, 3);
	*xpos = atoi(xCoord);

	if(*xpos <= 19)
	{
		return 0;
	}

	else
	{
		return -1;
	}

}

void PlaceShips(int numShips, int playerNumber, char* field)
{
	int xpos;
	int shipNumber = 1;
	

	while (shipNumber <= numShips)
	{
		printf("Ship %d location. ", shipNumber);
		PromptPlayerForCoords(playerNumber, &xpos);
		field[xpos] = 's';
		shipNumber = shipNumber + 1;
	}

	return;
}

int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int xpos)
{
	int sunkShip = 0;
	int ret;
	char allShipsSunkField[21] = "oooooooooooooooooooo";

	if (fieldOfDefender[xpos] == 's')
	{
		fieldOfDefender[xpos] = 'o';
		printf("Player %d sunk Player %d's ship! \n", playerNumberOfAttacker, playerNumberOfDefender);
		sunkShip = sunkShip + 1;
	}

	else
	{
		printf("Player %d's ships are safe! \n", playerNumberOfDefender);
	}

	ret = strcmp(fieldOfDefender, allShipsSunkField);

	if (ret == 0)
	{
		return 9999;
	}

	else if (ret != 0)
	{
		return sunkShip;
	}

	else
	{
		return -1;
	}

}