// BubbleShip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>


//Creates structure called "Field"
typedef struct _Field {
	int fieldSize;
	int xAxisInt;
	int yAxisInt;
} Field;

#include "BubbleShip.h"

int main()
{
	while (-1 == LoopMain()); //Restarts the game when player enters coordinates that are too big
}


int LoopMain()
{
	int currentNumOfPlayers;
	int numberOfPlayers;
	int numberOfShips;
	Field fieldInfo;
	char** playerFields;
	int ret;
	int playerNumber = 1;
	int attackResults;
	int defPlayerNumber;
	int coord = 0;
	char* playerOutField;
	int returnOfResults;
	
	
	numberOfPlayers = ChooseNumberOfPlayers(); //Prompts user to choose number of players. Returns the number of players.
	currentNumOfPlayers = numberOfPlayers;

	numberOfShips = ChooseNumberOfShips(); //Prompts user to choose number of ships per person. Returns the number of ships.

	fieldInfo = ChooseFieldSize(); //Prompts user to choose x and y axes of field, and determines size of field. Returns structure containing x axis, y axis, and field size.
		
	playerFields = CallBuildField(numberOfPlayers, fieldInfo); //Calls BuildField function. Returns pointer to players' fields.

	CallPlaceShips(numberOfPlayers, playerNumber, numberOfShips, playerFields, fieldInfo);
	playerNumber = 1; //Prepares for first turn, starting with player 1


	while (1 == 1) //Loops forever
	{
		playerOutField = CreateOutField(fieldInfo); //Creates field of all x's, which represents field of player that is out of the game.

		while (playerNumber <= numberOfPlayers) //Players choose who they want to attack
		{
			defPlayerNumber = PrepareForAttack(playerNumber, numberOfPlayers);

			ret = PromptPlayerForCoords(playerNumber, &coord, fieldInfo);
			if (ret == -1) //Occurs if player enters coordinates that are too large for field. Re-starts game.
			{
				LoopMain();
			}

			attackResults = CallLaunchAttackAgainstPlayer(playerNumber, defPlayerNumber, playerFields[PLAYER_NUMBER_TO_INDEX(defPlayerNumber)], coord, fieldInfo, currentNumOfPlayers);
			
			returnOfResults = ResultsOfMatch(attackResults, playerNumber, defPlayerNumber, currentNumOfPlayers); //Exits main function upon only 1 player being left in game

			if (returnOfResults == 0)
			{
				return 0;
			}
			else if (returnOfResults == 10)
			{
				currentNumOfPlayers--;
			}
			else
			{
				currentNumOfPlayers = currentNumOfPlayers;
			}
			
			playerNumber = RotateThroughPlayerTurns(playerNumber, numberOfPlayers, playerFields, playerOutField);
		}
	}
	
	return -1; //Should never get here.
}


int ChooseNumberOfPlayers()
{
	char* playerNumberChar;
	int numberOfPlayers;

	//Choose number of players
	printf("How many players? ");
	playerNumberChar = (char*)malloc(MAX_PLAYER_DIGITS);
	gets_s(playerNumberChar, MAX_PLAYER_DIGITS);
	numberOfPlayers = atoi(playerNumberChar);

	return numberOfPlayers;
}


int ChooseNumberOfShips()
{
	int numberOfShips;
	char* numShipsStr;

	//Player chooses how many ships
	printf("How many ships do you want? ");
	numShipsStr = (char*)malloc(10);
	gets_s(numShipsStr, 10);
	numberOfShips = atoi(numShipsStr);

	if (numberOfShips > 9999999999) //Occurs if player wants to place too many ships. Re-starts the game.
	{
		printf("You can't place that many ships! ");
		return -1;
	}

	return numberOfShips;
}


Field ChooseFieldSize()
{
	char* xAxis;
	char* yAxis;
	Field fieldAxesAndSize;

	//Player chooses the sizes of the x axis
	printf("Choose the size of your x axis. ");
	xAxis = (char*)malloc(3);
	gets_s(xAxis, 3);
	fieldAxesAndSize.xAxisInt = atoi(xAxis);

	//Player chooses the size of the y axis
	printf("Choose the size of your y axis. ");
	yAxis = (char*)malloc(3);
	gets_s(yAxis, 3);
	fieldAxesAndSize.yAxisInt = atoi(yAxis);

	fieldAxesAndSize.fieldSize = (fieldAxesAndSize.xAxisInt * fieldAxesAndSize.yAxisInt); //Size of the field

	return fieldAxesAndSize;
}


char** CallBuildField(int numberOfPlayers, Field fieldInfo)
{
	int playerIndex = 0;
	char** fieldOfPlayerPointer;

	fieldOfPlayerPointer = (char**)malloc(numberOfPlayers * 4);

	//Builds the field for each player, with all empty spaces (i.e., all 'o')
	while (playerIndex < numberOfPlayers)
	{
		fieldOfPlayerPointer[playerIndex] = BuildField(fieldInfo);
		playerIndex = playerIndex + 1;
	}

	return fieldOfPlayerPointer;
}


char* BuildField(Field fieldInfo)
{
	char* field;
	int index;
	index = 0;

	field = (char*)malloc(fieldInfo.fieldSize); //Dynamically allocates space of the user's chosen field size, and gives that space to a pointer


	while (index < fieldInfo.fieldSize)
	{
		*(field + index) = 'o';
		index = index + 1;
	}

	*(field + index) = 0; //Sets null terminator

	return field;
}

void CallPlaceShips(int numberOfPlayers, int playerNumber, int numberOfShips, char** playerFields, Field fieldInfo)
{
	while (playerNumber <= numberOfPlayers) //Loops through players' turns to place ships
	{
		PlaceShips(numberOfShips, playerNumber, playerFields, fieldInfo);
		playerNumber = playerNumber + 1;
	}
}


int PromptPlayerForCoords(int playerNumber, int* coordPos, Field fieldInfo)
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

	if (xpos > fieldInfo.xAxisInt) //If player enters x coord that is too large, restarts game
	{
		printf("That x coordinate is too big! ");
		return -1;
	}

	//Prompts player for y coordinates
	printf("Player %d, enter y coord ", playerNumber);
	yCoord = (char*)malloc(3);
	gets_s(yCoord, 3);
	ypos = atoi(yCoord);

	if (ypos > fieldInfo.yAxisInt) //If player enters y coord that is too large, restarts game
	{
		printf("That y coordinate is too big! ");
	}

	*coordPos = ((fieldInfo.xAxisInt * ypos) - (fieldInfo.xAxisInt - xpos)); //Determines specific value for each coordinate pair

	if (*coordPos <= fieldInfo.fieldSize) //Returns 0 when coordinates fit into field size
	{
		return 0;
	}

	else
	{
		return -1;
	}

}

void PlaceShips(int numShips, int playerNumber, char** playerFields, Field fieldInfo)
{
	int pos;
	int shipNumber = 1;
	int ret;

	while (shipNumber <= numShips) //Loops until all of player's ships have been placed
	{
		printf("**Ship %d location.**\n", shipNumber);
		ret = PromptPlayerForCoords(playerNumber, &pos, fieldInfo); //Calls function so players can place their ship coordinates
		if (ret == -1) //If player enters coordinate for ship that is larger than the field size
		{
			LoopMain();
		}
		playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)][POSITION_NUMBER_TO_INDEX(pos)] = 's'; //Places s wherever player specified coordinates
		shipNumber = shipNumber + 1;
	}

	return;
}

char* CreateOutField(Field fieldInfo)
{
	char* playerOutField; 
	int indexOfOutField = 0;

	playerOutField = (char*)malloc(fieldInfo.fieldSize + 1); //Allocate space for a field of fieldSize + 1 (for null terminator)

	while (indexOfOutField < fieldInfo.fieldSize) //Creates field of all 'x' of the size specified by player
	{
		*(playerOutField + indexOfOutField) = 'x';
		indexOfOutField = indexOfOutField + 1;
	}

	*(playerOutField + indexOfOutField) = 0; //Sets null terminator

	indexOfOutField = 0;

	return playerOutField;
}

int PrepareForAttack(int playerNumber, int numberOfPlayers)
{
	char* defPlayerNumberChar;
	int defPlayerNumber;
	
		printf("Player %d, who do you want to attack? ", playerNumber);
		defPlayerNumberChar = (char*)malloc(MAX_PLAYER_DIGITS);
		gets_s(defPlayerNumberChar, MAX_PLAYER_DIGITS);
		defPlayerNumber = atoi(defPlayerNumberChar);

	return defPlayerNumber;
}

int CallLaunchAttackAgainstPlayer(int playerNumber, int defPlayerNumber, char* fieldOfDefender, int coord, Field fieldInfo, int currentNumOfPlayers)
{
	int attackResults;

	attackResults = LaunchAttackAgainstPlayer(playerNumber, defPlayerNumber, fieldOfDefender, coord, fieldInfo, currentNumOfPlayers); //Return result of each turn
	
	return attackResults;
}

int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, Field fieldInfo, int currentNumOfPlayers)
{
	int sunkShip = 0;
	int ret;
	int indexOfField = 0;
	char* allShipsSunkField;


	allShipsSunkField = (char*)malloc(fieldInfo.fieldSize + 1); //Allocate space for a field of fieldSize + 1 (for null terminator)

	while (indexOfField < fieldInfo.fieldSize) //Creates field of all 'o' of the size specified by player
	{
		*(allShipsSunkField + indexOfField) = 'o';
		indexOfField = indexOfField + 1;
	}

	*(allShipsSunkField + indexOfField) = 0; //Sets null terminator

	//Changes 's' to 'o' if attacking player correctly guesses defender's ships location
	if (fieldOfDefender[POSITION_NUMBER_TO_INDEX(coordPos)] == 's') 
	{
		fieldOfDefender[POSITION_NUMBER_TO_INDEX(coordPos)] = 'o';
		printf("Player %d sunk Player %d's ship! \n", playerNumberOfAttacker, playerNumberOfDefender);
		sunkShip = sunkShip + 1;
	}

	else //Prints if attacking player guesses incorrect location of defending player's ship
	{
		printf("Player %d's ships are safe! \n", playerNumberOfDefender);
	}

	ret = strcmp(fieldOfDefender, allShipsSunkField); //Compares field of defending player to a field of all sunk ships

	if (ret == 0)
	{
		indexOfField = 0;
		while (indexOfField < fieldInfo.fieldSize) //Creates field of all 'x' of the size specified by player
		{
			*(fieldOfDefender + indexOfField) = 'x';
			indexOfField = indexOfField + 1;
		}
		*(fieldOfDefender + indexOfField) = 0; //Sets null terminator

		if (currentNumOfPlayers == 2)
		{
			return 9999;
		}
		else
		{
			return 100;
		}
	}
	else if (ret != 0) //If strings are different
	{
		return sunkShip; //Returns 1 (how many ships are sunk this turn)
	}
	else
	{
		return -1; //Returns on error
	}

}

int ResultsOfMatch(int attackResults, int playerNumber, int defPlayerNumber, int currentNumOfPlayers)
{
	if (attackResults == 9999) //Occurs when one player sinks all (or the last remaining) of another player's ships
	{
		printf("Player %d sunk Player %d's last ship! ", playerNumber, defPlayerNumber);
		printf("Player %d Wins!", playerNumber);
		return 0;
	}
	else if (attackResults == 100)
	{
		printf("All of Player %d's ships have been sunk. Player %d is out of the game! \n", defPlayerNumber, defPlayerNumber);
		return 10;
	}
	else //Prints how many ships a player has sunk in the current turn.
	{
		printf("Player %d sunk %d ship! \n", playerNumber, attackResults);
		return 100;
	}
}

int RotateThroughPlayerTurns(int playerNumber, int numberOfPlayers, char** playerFields, char* playerOutField)
{
	//Rotates through players turns in sequential order. When all players have had a turn, player 1 takes their next turn.
	if (playerNumber < numberOfPlayers)
	{
		playerNumber++;
		while (strcmp(playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)], playerOutField) == 0)
		{
			playerNumber++;
			if (playerNumber > numberOfPlayers)
			{
				playerNumber = 1;
			}
		}
	}
	else
	{
		playerNumber = 1;
		while (strcmp(playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)], playerOutField) == 0)
		{
			playerNumber++;
			if (playerNumber > numberOfPlayers)
			{
				playerNumber = 1;
			}
		}
	}
	return playerNumber;
}