// BubbleShip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BubbleShip.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>


int main()
{
	while(-1 == LoopMain());
}


int LoopMain()
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
	int ret;

	int coord = 0;

	printf("How many ships do you want? (choose number between 1-19)");
	numShipsStr = (char*)malloc(4);
	gets_s(numShipsStr, 4);
	numberOfShips = atoi(numShipsStr);
	if (numberOfShips > 19)
	{
		printf("You can't place that many ships! ");
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

	PlaceShips(numberOfShips, playerOneNumber, playerOneField, xAxisInt, yAxisInt, fieldSize);
	PlaceShips(numberOfShips, playerTwoNumber, playerTwoField, xAxisInt, yAxisInt, fieldSize);



	while (1 == 1) //Loops forever
	{

		//Player One's turn
		ret = PromptPlayerForCoords(playerOneNumber, &coord, xAxisInt, yAxisInt, fieldSize);
		if (ret == -1) //Occurs if player enters coordinates that are too large for field
		{
			LoopMain();
		}

		attackResults = LaunchAttackAgainstPlayer(playerOneNumber, playerTwoNumber, playerTwoField, coord, fieldSize);
		
		if (attackResults == 9999)
		{
			printf("Player One sunk all of Player 2's ships! ");
			printf("Player One Wins!");

			return 0; //Exit main function
		}

		else
		{
			printf("Player One sunk %d ships! \n", attackResults);
		}

		//Player Two's turn
		PromptPlayerForCoords(playerTwoNumber, &coord, xAxisInt, yAxisInt, fieldSize);
		attackResults = LaunchAttackAgainstPlayer(playerTwoNumber, playerOneNumber, playerOneField, coord, fieldSize);

		if (attackResults == 9999)
		{
			printf("Player Two sunk all of Player One's ships! ");
			printf("Player Two Wins!");

			return 0; //Exit main function
		}

		else
		{
			printf("Player Two sunk %d ships! \n", attackResults);
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


int PromptPlayerForCoords(int playerNumber, int* coordPos, int maxX, int maxY, int fieldSize)
{
	
	char* xCoord;
	char* yCoord;
	int xpos;
	int ypos;

	printf("Player %d, enter x coord ", playerNumber);
	xCoord = (char*)malloc(3);
	gets_s(xCoord, 3);
	xpos = atoi(xCoord);
	
	if (xpos > maxX)
	{
		printf("That x coordinate is too big! ");
		return -1;
	}

	printf("Player %d, enter y coord ", playerNumber);
	yCoord = (char*)malloc(3);
	gets_s(yCoord, 3);
	ypos = atoi(yCoord);

	if (ypos > maxY)
	{
		printf("That y coordinate is too big! ");
	}

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

void PlaceShips(int numShips, int playerNumber, char* field, int maxX, int maxY, int fieldSize)
{
	int pos;
	int shipNumber = 1;
	int ret;
	

	while (shipNumber <= numShips)
	{
		printf("**Ship %d location.**\n", shipNumber);
		ret = PromptPlayerForCoords(playerNumber, &pos, maxX, maxY, fieldSize);
		if (ret == -1)
		{
			LoopMain();
		}
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