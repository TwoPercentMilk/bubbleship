// BubbleShip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BubbleShip.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>


int main()
{
	char* xAxis;
	char* yAxis;
	int xAxisInt;
	int yAxisInt;
	int fieldSize;
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

	printf("Choose the size of your x axis. ");
	xAxis = (char*)malloc(MAX_PATH);
	gets_s(xAxis, MAX_PATH);
	xAxisInt = atoi(xAxis);

	printf("Choose the size of your y axis. ");
	yAxis = (char*)malloc(MAX_PATH);
	gets_s(yAxis, MAX_PATH);
	yAxisInt = atoi(yAxis);

	fieldSize = (xAxisInt * yAxisInt);


	playerOneField = BuildField(fieldSize);
	playerTwoField = BuildField(fieldSize);

	PlaceShips(numberOfShips, playerOneNumber, playerOneField, xAxisInt, fieldSize);
	PlaceShips(numberOfShips, playerTwoNumber, playerTwoField, xAxisInt, fieldSize);



	while (1==1) //Loops forever
	{

		//Player One's turn
		PromptPlayerForCoords(playerOneNumber, &coord, xAxisInt, fieldSize);
		attackResults = LaunchAttackAgainstPlayer(playerOneNumber, playerTwoNumber, playerTwoField, coord, fieldSize);

		printf("Player One sunk %d ships! \n", attackResults);
		if (attackResults == 9999)
		{
			printf("Player One Wins!");
			return 0; //Exit main function
		}


		//Player Two's turn
		PromptPlayerForCoords(playerTwoNumber, &coord, xAxisInt, fieldSize);
		attackResults = LaunchAttackAgainstPlayer(playerTwoNumber, playerOneNumber, playerOneField, coord, fieldSize);

		printf("Player Two sunk %d ships! \n", attackResults);
		if (attackResults == 9999)
		{
			printf("Player Two Wins!");
			return 0; //Exit main function
		}
	}




    return -1; //Should never get here;

}

char* BuildField(int fieldSize)
{
	
	char* field;
	int index;

	field = (char*)malloc(fieldSize);

	index = 0;

	while(index < fieldSize)
	{
		*(field + index) = 'o';
		index = index + 1;
	}

	*(field + index) = 0;

	return field;

}


int PromptPlayerForCoords(int playerNumber, int* coordPos, int maxX, int fieldSize)
{
	
	char* xCoord;
	char* yCoord;
	int xpos;
	int ypos;

	printf("Player %d, enter x coord ", playerNumber);
	xCoord = (char*)malloc(3);
	gets_s(xCoord, 3);
	xpos = atoi(xCoord);

	printf("Player %d, enter y coord ", playerNumber);
	yCoord = (char*)malloc(3);
	gets_s(yCoord, 3);
	ypos = atoi(yCoord);

	*coordPos = ((maxX * ypos) - (maxX - xpos));


	if(*coordPos <= fieldSize)
	{
		return 0;
	}

	else
	{
		return -1;
	}

}

void PlaceShips(int numShips, int playerNumber, char* field, int maxX, int fieldSize)
{
	int pos;
	int shipNumber = 1;
	

	while (shipNumber <= numShips)
	{
		printf("**Ship %d location.**\n", shipNumber);
		PromptPlayerForCoords(playerNumber, &pos, maxX, fieldSize);
		field[pos] = 's';
		shipNumber = shipNumber + 1;
	}

	return;
}

int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, int fieldSize)
{
	int sunkShip = 0;
	int ret;
	int indexOfField = 0;
	char* allShipsSunkField;
	
	allShipsSunkField = (char*)malloc(fieldSize + 1);

	while (indexOfField < fieldSize)
	{
		*(allShipsSunkField + indexOfField) = 'o';
		indexOfField = indexOfField + 1;
	}

	*(allShipsSunkField + indexOfField) = 0;

	if (fieldOfDefender[coordPos] == 's')
	{
		fieldOfDefender[coordPos] = 'o';
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