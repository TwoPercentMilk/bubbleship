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

/*CallBuildField
Calls the function BuildField for each player.

numberOfPlayers: the number of players in the game
fieldInfo: structure including x axis, y axis, and size of field

returns nothing*/
char** CallBuildField(int numberOfPlayers, Field fieldInfo);


/*BuildField
Builds a field. Player has control to choose size of field. The field is an array of characters and should
in the beginning every character should be an o. This will
need to dynamically allocate space for a character array and return the pointer.

fieldInfo: structure including x axis, y axis, and size of field

returns a pointer to an array of n characters
*/
char* BuildField(Field fieldInfo);

/*CallPlaceShips
Calls the function PlaceShips for each player in the game.

numberOfPlayers: the number of players in the game
playerNumber: the id number of the current attacking player
numberOfShips: the number of ships the players can place
playerFields: a double pointer; points to a pointer for a specific player, that points to a field array
fieldInfo: structure including x axis, y axis, and size of field

returns nothing*/
void CallPlaceShips(int numberOfPlayers, int playerNumber, int numberOfShips, char** playerFields, Field fieldInfo);


/*PromptPlayerForCoords
Prompts user for x coord
Example:
Player 1, enter x coord>

Prompts user for y coord
Example:
Player 1, enter x coord>

playerNumber: The id number of the player
coordPos: Address where PromptPlayerForCoords should put the coordinate retrieved from the user
fieldInfo: structure including x axis, y axis, and size of field

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
playerField: a double pointer; points to a pointer for a specific player, that points to a field array
fieldInfo: structure including x axis, y axis, and size of field

returns nothing
*/
void PlaceShips(int numShips, int playerNumber, char** playerField, Field fieldInfo);

/*CreateOutField
Create a field of all x's, which represents a player's field when they are out of the game.

fieldInfo: structure including x axis, y axis, and size of field

returns field of all x's.
*/
char* CreateOutField(Field fieldInfo);

/*PrepareForAttack
Asks player who they want to attack. Calls PromptPlayerForCoords to place guess coordinate.

playerNumber: the id number of the attacking player
numberOfPlayers: number of players in the game

returns player number of defender*/
int PrepareForAttack(int playerNumber, int numberOfPlayers);

/*CallLaunchAttackAgainstPlayer
Calls the function LaunchAttackAgainstPlayer.

playerNumber: the id number of the attacking player
defPlayerNumber: the id number of the defending player
fieldOfDefender: A pointer to the defender's field, which is an array of characters
coord: the specific spot on the field where the attacking player is attacking
fieldInfo: structure including x axis, y axis, and size of field
currentNumOfPlayers: the current number of players, not including players who already lost

returns results of the attack*/
int CallLaunchAttackAgainstPlayer(int playerNumber, int defPlayerNumber, char* fieldOfDefender, int coord, Field fieldInfo, int currentNumOfPlayers);

/*LaunchAttackAgainstPlayer
Allows one player to launch attack against another player

playerNumberOfAttacker: The id number of the attacking player
playerNumberOfDefender: The id number of the defending player
fieldOfDefender: A pointer to the defender's field (defender's field should be an array of characters)
coordPos: Address where PromptPlayerForCoords should put the coordinate retrieved from the user
fieldInfo: structure including x axis, y axis, and size of field
currentNumOfPlayers: number of players still in the game, not including players who already lost

returns 9999 if all ships are sunk and only 1 player remains
returns 100 if all ships were sunk but more than 2 or more players remain
returns the number of sunken ships if 1 ship is sunk, but others still remain on the players' field
returns -1 on error
*/
int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, Field fieldInfo, int currentNumOfPlayers);

/*ResultsOfMatch
Prints results of match.

attackResults: return value of LaunchAttackAgainstPlayer
playerNumber: the id number of the current attacking player
defPlayerNumber: the id number of the current defending player
currentNumOfPlayers: the current number of players, not including players who already lost

returns 0 if only one player remains
returns 10 if the attacking player sunk a shihp
returns 100 if attacking player misses*/
int ResultsOfMatch(int attackResults, int playerNumber, int defPlayerNumber, int currentNumOfPlayers);


/*RotateThroughPlayerTurns
Rotates through players' turns, skipping players who are out of the game.

playerNumber: the id number of the current attacking player
numberOfPlayers: number of players in the game
playerFields: a double pointer; points to a pointer for a specific player, that points to a field array
playerOutField: an array to a field of all x's

return playerNumber*/
int RotateThroughPlayerTurns(int playerNumber, int numberOfPlayers, char** playerFields, char* playerOutField);



