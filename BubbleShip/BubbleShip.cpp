// BubbleShip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BubbleShip.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>


int main()
{
	while(-1 == LoopMain()); //Restarts the game when player enters coordinates that are too big
}


int LoopMain()
{
	char* xAxis;
	char* yAxis;
	int xAxisInt;
	int yAxisInt;
	int fieldSize;
	char** playerFields;
	int playerNumber = 1;
	char* playerNumberChar;
	int playerIndex = 0;
	int numberOfPlayers;
	int attackResults = 0;
	int numberOfShips;
	char* numShipsStr;
	int ret;
	int attPlayerNumber = 1;
	int defPlayerNumber;
	char* defPlayerNumberChar;

	int coord = 0;


	//Choose number of players
	printf("How many players?");
	playerNumberChar = (char*)malloc(MAX_PLAYER_DIGITS);
	gets_s(playerNumberChar, MAX_PLAYER_DIGITS);
	numberOfPlayers = atoi(playerNumberChar);

	//Player chooses how many ships
	printf("How many ships do you want? ");
	numShipsStr = (char*)malloc(10);
	gets_s(numShipsStr, 10);
	numberOfShips = atoi(numShipsStr);
	if (numberOfShips > 9999999999)
	{
		printf("You can't place that many ships! ");
		return -1;
	}

	//Player chooses the sizes of the x and y axes
	printf("Choose the size of your x axis. ");
	xAxis = (char*)malloc(3);
	gets_s(xAxis, 3);
	xAxisInt = atoi(xAxis);

	printf("Choose the size of your y axis. ");
	yAxis = (char*)malloc(3);
	gets_s(yAxis, 3);
	yAxisInt = atoi(yAxis);

	fieldSize = (xAxisInt * yAxisInt); //Size of the field

	playerFields = (char**)malloc(numberOfPlayers * 4); //Allocates 4 bytes of space for playerFields (which is a char**)

	//Builds the correct size of the field for each player, with all empty spaces (i.e., all 'o')
	while (playerIndex < numberOfPlayers)
	{
		playerFields[playerIndex] = BuildField(fieldSize);
		playerIndex = playerIndex + 1;
	}

	playerIndex = 0;

	//Places ships on each player's fields
	while (playerNumber <= numberOfPlayers) //Loops through players' turns
	{
		PlaceShips(numberOfShips, playerNumber, playerFields, xAxisInt, yAxisInt, fieldSize);
		playerNumber = playerNumber + 1;

	}

	playerNumber = 1;

		while (1 == 1) //Loops forever
		{
			while (attPlayerNumber <= numberOfPlayers)
			{
				printf("Player %d, who do you want to attack? ", attPlayerNumber);
				defPlayerNumberChar = (char*)malloc(MAX_PLAYER_DIGITS);
				gets_s(defPlayerNumberChar, MAX_PLAYER_DIGITS);
				defPlayerNumber = atoi(defPlayerNumberChar);
				

			ret = PromptPlayerForCoords(playerNumber, &coord, xAxisInt, yAxisInt, fieldSize);
			if (ret == -1) //Occurs if player enters coordinates that are too large for field
			{
				LoopMain();
			}

			attackResults = LaunchAttackAgainstPlayer(playerNumber, defPlayerNumber, playerFields, coord, fieldSize);

			if (attackResults == 9999)
			{
				printf("Player %d sunk all of Player %d's ships! ", attPlayerNumber, defPlayerNumber);
				printf("Player %d Wins!", attPlayerNumber);

				return 0; //Exit main function	
			}

			else
			{
				printf("Player %d sunk %d ships! \n", playerNumber, attackResults);
			}

			if (playerNumber < numberOfPlayers)
			{
				playerNumber = playerNumber + 1;
			}
			else
			{
				playerNumber = 1;
			}

			if (attPlayerNumber < numberOfPlayers)
			{
				attPlayerNumber = attPlayerNumber + 1;
			}
			else
			{
				attPlayerNumber = 1;
			}


			
		}
	}
	return -1; //Should never get here;
}

char* BuildField(int fieldSize)
{
	char* field;
	int index;
	index = 0;

	field = (char*)malloc(fieldSize);

	while(index < fieldSize)
	{
		*(field + index) = 'o';
		index = index + 1;
	}

	*(field + index) = 0; //Sets null terminator

	return field;

}


int PromptPlayerForCoords(int playerNumber, int* coordPos, int maxX, int maxY, int fieldSize)
{
	
	char* xCoord;
	char* yCoord;
	int xpos;
	int ypos;

	//Prompts player for x coordinates
	printf("Player %d, enter x coord ", playerNumber);
	xCoord = (char*)malloc(3);
	gets_s(xCoord, 3);
	xpos = atoi(xCoord);
	
	if (xpos > maxX) //If player enters x coord that is too large
	{
		printf("That x coordinate is too big! ");
		return -1;
	}

	//Prompts player for y coordinates
	printf("Player %d, enter y coord ", playerNumber);
	yCoord = (char*)malloc(3);
	gets_s(yCoord, 3);
	ypos = atoi(yCoord);

	if (ypos > maxY) //If player enters y coord that is too large
	{
		printf("That y coordinate is too big! ");
	}

	*coordPos = ((maxX * ypos) - (maxX - xpos)); //Determines specific value for each coordinate pair
	
	if(*coordPos <= fieldSize)
	{
		return 0;
	}

	else
	{
		return -1;
	}

}

void PlaceShips(int numShips, int playerNumber, char** playerFields, int maxX, int maxY, int fieldSize)
{
	int pos;
	int shipNumber = 1;
	int ret;

	while (shipNumber <= numShips) //Loops until all of player's ships have been placed
	{
		printf("**Ship %d location.**\n", shipNumber);
		ret = PromptPlayerForCoords(playerNumber, &pos, maxX, maxY, fieldSize);
 		if (ret == -1) //If player enters coordinate for ship that is larger than the field size
		{
			LoopMain();
		}
		playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)][POSITION_NUMBER_TO_INDEX(pos)] = 's'; //Places s wherever player specified coordinates
		shipNumber = shipNumber + 1;
		
	}

	return;
}

int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char** fieldOfDefender, int coordPos, int fieldSize)
{
	int sunkShip = 0;
	int ret;
	int indexOfField = 0;
	char* allShipsSunkField;

	
	allShipsSunkField = (char*)malloc(fieldSize + 1); //Allocate space for a field of fieldSize + 1 (for null terminator)

	while (indexOfField < fieldSize) //Creates field of all 'o' of the size specified by player
	{
		*(allShipsSunkField + indexOfField) = 'o';
		indexOfField = indexOfField + 1;
	}

	*(allShipsSunkField + indexOfField) = 0; //Sets null terminator
	if (fieldOfDefender[PLAYER_NUMBER_TO_INDEX(playerNumberOfDefender)][POSITION_NUMBER_TO_INDEX(coordPos)] == 's') //Changes 's' to 'o' if attacking player correctly guesses defender's ships location
	{
		fieldOfDefender[PLAYER_NUMBER_TO_INDEX(playerNumberOfDefender)][POSITION_NUMBER_TO_INDEX(coordPos)] = 'o';
		printf("Player %d sunk Player %d's ship! \n", playerNumberOfAttacker, playerNumberOfDefender);
		sunkShip = sunkShip + 1;
	}

	else
	{
		printf("Player %d's ships are safe! \n", playerNumberOfDefender);
	}

	ret = strcmp(fieldOfDefender[PLAYER_NUMBER_TO_INDEX(playerNumberOfDefender)], allShipsSunkField); //Compares field of defending player to a field of all sunk ships

	if (ret == 0) //If strings are the same
	{
		return 9999;
	}

	else if (ret != 0) //If strings are different
	{
		return sunkShip; //Returns 1 (how many ships are sunk)
	}

	else
	{
		return -1;
	}

}