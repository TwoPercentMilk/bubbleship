#pragma once



/*BuildField
Builds a field. Player has control to choose size of field. The field is an array of characters and should
in the beginning every character should be an o. Do not use a null-terminator. This will
need to dynamically allocate space for a character array and return the pointer.

returns a pointer to an array of n characters
*/
char* BuildField(int fieldSize);


/*PromptPlayerForCoords
Prompts user for x coord
Example:
Player 1, enter x coord>

Prompts user for y coord
Example:
Player 1, enter x coord>

playerNumber: The id number of the player
coordPos: Address where PromptPlayerForCoords should put the coordinate retrieved from the user

returns 0 on success
returns -1 on error
*/
int PromptPlayerForCoords(int playerNumber, int* coordPos, int maxX, int maxY, int fieldSize);

/*PlaceShips
Prompts the user for where to place some number of ships.
The function will prompt the user for a position for each ship.
This should call PromptPlayerForCoords

Put an s anywhere there is a ship.

Example run (if looking for 4 ships):
Ship 1 location. Player 1 enter coord:> 3
Ship 2 location. Player 1 enter coord:> 7
Ship 3 location. Player 1 enter coord:> 13
Ship 4 location. Player 1 enter coord:> 17

This would result in a field like this:
ooosooosooooosooosoo

numShips: The number of ships to place
playerNumber: The id number of the player placing the ships
field: A pointer to the character array which represents the field.

returns nothing
*/
void PlaceShips(int numShips, int playerNumber, char* field, int maxX, int maxY, int fieldSize);




/*LaunchAttackAgainstPlayer
Allows one player to launch attack against another player

playerNumberOfAttacker: The id number of the attacking player
playerNumberOfDefender: The id number of the defending player
fieldOfDefender: A pointer to the defender's field (defender's field should be an array of characters)
xpos: The x coordinate of the attack

returns 9999 if all ships are sunk
returns the number of ships sunk
returns -1 on error
*/
int LaunchAttackAgainstPlayer(int playerNumberOfAttacker, int playerNumberOfDefender, char* fieldOfDefender, int coordPos, int fieldSize);


/*LoopMain
Loops main function when players enter incorrect coordinates
*/
int LoopMain();


