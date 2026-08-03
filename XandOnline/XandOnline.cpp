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

*/

int main()
{
    std::cout << "Hello World!\n";
}
