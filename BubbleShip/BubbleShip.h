#pragma once

#define MAX_PLAYER_DIGITS 2 //Maximum number of digits for the amount of players is 2 (includes null) (i.e., no more than 9 players
#define MAX_PLAYERS 9 //Maximum number of players
#define PLAYER_NUMBER_TO_INDEX(num) (num-1) //Subtracts 1 from player number, in order to start at 0th index
#define POSITION_NUMBER_TO_INDEX(num) (num-1) //Subtracts 1 from pos int, in order to start at 0th index


/*LoopMain
Loops main function when players enter incorrect coordinates
*/
int LoopMain();

/*ChooseNumberOfPlayers
Allows player to choose how many players will participate in the game.

returns the number of players*/
int ChooseNumberOfPlayers();

/*ChooseNumberOfShips
Allows player to choose how many ships per person.

returns the number of ships*/
int ChooseNumberOfShips();

/*ChooseFieldSize
Allows player to choose the x and y coordinates of the field. Determines the size of the field.*/

Field ChooseFieldSize();


/*BuildField
Builds a field. Player has control to choose size of field. The field is an array of characters and should
in the beginning every character should be an o. This will
need to dynamically allocate space for a character array and return the pointer.

fieldSize: size of the field that the player decides (x axis * y axis)

returns a pointer to an array of n characters
*/
char* BuildField(Field fieldInfo);


/*PromptPlayerForCoords
Prompts user for x coord
Example:
Player 1, enter x coord>

Prompts user for y coord
Example:
Player 1, enter x coord>

playerNumber: The id number of the player
coordPos: Address where PromptPlayerForCoords should put the coordinate retrieved from the user
maxX: size of the x axis
maxY: size of the y axis
fieldSize: size of the field (x axis * y axis)

returns 0 on success
returns -1 on error
*/
int PromptPlayerForCoords(int playerNumber, int* coordPos, Field fieldInfo);

/*PlaceShips
Prompts the user for where to place some number of ships.
The function will prompt the user for a position for each ship.
This should call PromptPlayerForCoords

Put an s anywhere there is a ship.

Example run (if looking for 2 ships):
Ship 1 location. 
Player 1 enter x coord:> 3
Player 1 enter y coord:> 2
Ship 2 location.
Player 1 enter x coord:> 7
Player 1 enter y coord:> 1
Ship 1 location.
Player 2 enter x coord:> 1
Player 2 enter y coord:> 8
Ship 2 location.
Player 2 enter x coord:> 9
Player 2 enter y coord:> 4

numShips: The number of ships to place
playerNumber: The id number of the player placing the ships
field: A pointer to the character array which represents the field.
maxX: size of the x axis
maxY: size of the y axis
fieldSize: size of the field (x axis * y axis)

returns nothing
*/
void PlaceShips(int numShips, int playerNumber, char** playerField, Field fieldInfo);




/*LaunchAttackAgainstPlayer
Allows one player to launch attack against another player

playerNumberOfAttacker: The id number of the attacking player
playerNumberOfDefender: The id number of the defending player
fieldOfDefender: A pointer to the defender's field (defender's field should be an array of characters)
coordPos: Address where PromptPlayerForCoords should put the coordinate retrieved from the user
fieldSize: size of the field (x axis * y axis)
numberOfPlayers: number of players still in the game. Subtracts by 1 if a player gets all of his ships sunk

returns 9999 if all ships are sunk
returns the number of ships sunk
returns -1 on error
*/
int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, Field fieldInfo, int currentNumOfPlayers);


