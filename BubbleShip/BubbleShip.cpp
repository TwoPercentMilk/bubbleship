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

		while (playerNumber <= numberOfPlayers) 
		{
			defPlayerNumber = PrepareForAttack(playerNumber, numberOfPlayers); //Players choose who they want to attack

			ret = PromptPlayerForCoords(playerNumber, &coord, fieldInfo); //Players place ships on their fields
			if (ret == -1) //Occurs if player enters coordinates that are too large for field. Re-starts game.
			{
				LoopMain();
			}

			attackResults = CallLaunchAttackAgainstPlayer(playerNumber, defPlayerNumber, playerFields[PLAYER_NUMBER_TO_INDEX(defPlayerNumber)], coord, fieldInfo, currentNumOfPlayers);
			
			returnOfResults = ResultsOfMatch(attackResults, playerNumber, defPlayerNumber, currentNumOfPlayers);
			if (returnOfResults == 0)
			{
				return 0; //Exit main function
			}
			else if (returnOfResults == 10)
			{
				currentNumOfPlayers--; //Subtracts 1 player from current number of players (defending player is out of the game)
			}
			else
			{
				currentNumOfPlayers = currentNumOfPlayers; //Keeps current number of players the same (either not all ships were sunk, or no ships were sunk)
			}
			
			playerNumber = RotateThroughPlayerTurns(playerNumber, numberOfPlayers, playerFields, playerOutField); //Rotates through players in turn order, skipping players who lost
		}
	}
	
	return -1; //Should never get here.
}

//Player chooses how many players will be in the game
int ChooseNumberOfPlayers()
{
	char* playerNumberChar;
	int numberOfPlayers;

	printf("How many players? ");
	playerNumberChar = (char*)malloc(MAX_PLAYER_DIGITS);
	gets_s(playerNumberChar, MAX_PLAYER_DIGITS);
	numberOfPlayers = atoi(playerNumberChar);

	return numberOfPlayers;
}

//Player chooses how many ships can be placed on each player's field
int ChooseNumberOfShips()
{
	int numberOfShips;
	char* numShipsStr;

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

//Player chooses the x and y axes of the field, setting the size of the field
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

//Calls BuildField function, returning double pointer that is a pointer to an array of pointers for each player's field
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

//Builds a field of all o's for each player of specified field size
char* BuildField(Field fieldInfo)
{
	char* field;
	int index;
	index = 0;

	field = (char*)malloc(fieldInfo.fieldSize); 


	while (index < fieldInfo.fieldSize)
	{
		*(field + index) = 'o';
		index = index + 1;
	}

	*(field + index) = 0; //Sets null terminator

	return field;
}


//Calls PlaceShips function for each player
void CallPlaceShips(int numberOfPlayers, int playerNumber, int numberOfShips, char** playerFields, Field fieldInfo)
{
	while (playerNumber <= numberOfPlayers) //Loops through players' turns to place ships
	{
		PlaceShips(numberOfShips, playerNumber, playerFields, fieldInfo);
		playerNumber = playerNumber + 1;
	}
}

//Prompts player to enter x and y coordinates
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

//Places ships on each player's fields, changing 'o' to 's'
void PlaceShips(int numShips, int playerNumber, char** playerFields, Field fieldInfo)
{
	int pos;
	int shipNumber = 1;
	int ret;

	while (shipNumber <= numShips) //Loops until all of player's ships have been placed
	{
		printf("**Ship %d location.**\n", shipNumber);
		ret = PromptPlayerForCoords(playerNumber, &pos, fieldInfo); //Calls function so players can enter coordinates for where to place ships
		if (ret == -1) //If player enters coordinate for ship that is larger than the field size
		{
			LoopMain();
		}
		playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)][POSITION_NUMBER_TO_INDEX(pos)] = 's'; //Places s wherever player specified coordinates
		shipNumber = shipNumber + 1;
	}

	return;
}

//Create a field of all x's of specified field size, representing a field of a player who lost
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

//Asks attacking player who they want to attack
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

//Calls LaunchAttackAgainstPlayer and returns the results of the attack
int CallLaunchAttackAgainstPlayer(int playerNumber, int defPlayerNumber, char* fieldOfDefender, int coord, Field fieldInfo, int currentNumOfPlayers)
{
	int attackResults;

	attackResults = LaunchAttackAgainstPlayer(playerNumber, defPlayerNumber, fieldOfDefender, coord, fieldInfo, currentNumOfPlayers); //Return result of each turn
	
	return attackResults;
}

//Launches an attack against another player
int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, Field fieldInfo, int currentNumOfPlayers)
{
	int sunkShip = 0;
	int ret;
	int indexOfField = 0;
	char* allShipsSunkField;


	allShipsSunkField = (char*)malloc(fieldInfo.fieldSize + 1); //Allocate space for a field of fieldSize + 1 (for null terminator)

	while (indexOfField < fieldInfo.fieldSize) //Creates field of all 'o' of the size specified by player, to compare a field where all ships are sunk
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

	if (ret == 0) //Field of defending player and all sunk ships field are the same
	{
		indexOfField = 0;
		while (indexOfField < fieldInfo.fieldSize) //Replaces defending player's field with all x's, signifying that they lost
		{
			*(fieldOfDefender + indexOfField) = 'x';
			indexOfField = indexOfField + 1;
		}
		*(fieldOfDefender + indexOfField) = 0; //Sets null terminator

		if (currentNumOfPlayers == 2) //Runs if the defending player lost and there is only one other remaining player in the game
		{
			return 9999;
		}
		else //Runs if defending player lost but there are more at least 2 players still in the game
		{
			return 100;
		}
	}
	else if (ret != 0) //Runs if defending player still has at least 1 ship left
	{
		return sunkShip; //Returns how many ships were sunk on this turn
	}
	else
	{
		return -1; //Returns on error
	}

}

//Prints the results of the match
int ResultsOfMatch(int attackResults, int playerNumber, int defPlayerNumber, int currentNumOfPlayers)
{
	if (attackResults == 9999) //Occurs when defending player has all ships sunk, and only 1 player remains in the game
	{
		printf("Player %d sunk Player %d's last ship! ", playerNumber, defPlayerNumber);
		printf("Player %d Wins!", playerNumber);
		return 0;
	}
	else if (attackResults == 100) //Prints when defending player lost but there are more at least 2 players still in the game
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

//Rotates through player's turns, skipping players who are out of the game
int RotateThroughPlayerTurns(int playerNumber, int numberOfPlayers, char** playerFields, char* playerOutField)
{
	if (playerNumber < numberOfPlayers)
	{
		playerNumber++; //Go to next player
		while (strcmp(playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)], playerOutField) == 0) //Go to next player if current player is out of the game
		{
			playerNumber++;
			if (playerNumber > numberOfPlayers) //When player number is larger than the number of players in the game, go back to player 1
			{
				playerNumber = 1;
			}
		}
	}
	else
	{
		playerNumber = 1; //Goes to player 1 if last player was equal to the number of players in the game
		while (strcmp(playerFields[PLAYER_NUMBER_TO_INDEX(playerNumber)], playerOutField) == 0) //Go to next player if current player is out of the game
		{
			playerNumber++;
			if (playerNumber > numberOfPlayers) //When player number is larger than the number of players in the game, go back to player 1
			{
				playerNumber = 1;
			}
		}
	}
	return playerNumber;
}