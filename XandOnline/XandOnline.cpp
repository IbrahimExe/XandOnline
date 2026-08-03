#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdlib>

// TIC TAC TOE ONLINE
/*

Goal is to create a console application that can play a simple Tic Tac Toe game.
Create a display that shows the board in the console.

- Display a coordinate for the board cell display
- Simple method is a 3x3 grid (std::cout << “[ ][ ][ ]\n”; 3 times)
- Can make more detailed if desired
- Use system(“cls”) to rebuild the board each step
- Should look something like:
 0  1  2
[ ][ ][ ] 0
[ ][ ][ ] 1
[ ][ ][ ] 2

Create a server that will manage the game.
- Listen for the client and reads responses
- Validates the input
- Sends updated board data to client

Create a client:
- Connects to the server
- Sends inputs to the server
- Updates the board when there is a response from the server

Main Flow:
- Create a server that waits for a client
- Create a client that connects to the server
- Server chooses who goes first and who is “X” and “O” (could be random)

- Players turn:
o Text displays “Enter Coordinates:”
o Player enters 2 digits to place their mark (eg: “02” is bottom left)
o Server reads the digits and confirms if the input is valid, if not, respond to the
player (or themself) and repeat the input until something is valid
    ▪ For simplicity, assume all inputs are integers and only need to check
    for something on the grid that is not already occupied
o If someone gets 3 marks in a row, that player wins and both players need to
display the winner
o If the board is full and there is no winner, both player should state there is a
draw

*/

int main()
{
    std::cout << "Hello World!\n";
}
