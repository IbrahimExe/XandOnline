#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

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


// Server Side
using namespace std;

char board[9];

void printBoard() {
    system("cls");
    cout << "   0  1  2\n";
    for (int r = 0; r < 3; r++) {
        cout << r << " ";
        for (int c = 0; c < 3; c++)
            cout << "[" << board[r * 3 + c] << "]";
        cout << "\n";
    }
}

bool checkWin(char sym) {
    int lines[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (int i = 0; i < 8; i++) {
        if (board[lines[i][0]] == sym && board[lines[i][1]] == sym && board[lines[i][2]] == sym)
            return true;
    }
    return false;
}

bool boardFull() {
    for (int i = 0; i < 9; i++)
        if (board[i] == ' ') return false;
    return true;
}

void sendLine(SOCKET s, string msg) {
    msg += "\n";
    send(s, msg.c_str(), (int)msg.size(), 0);
}

string recvLine(SOCKET s) {
    string result;
    char ch;
    while (recv(s, &ch, 1, 0) > 0) {
        if (ch == '\n') break;
        result += ch;
    }
    return result;
}

int main() {
    for (int i = 0; i < 9; i++) board[i] = ' ';

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(54000);

    bind(listenSock, (sockaddr*)&addr, sizeof(addr));
    listen(listenSock, 1);

    cout << "Waiting for client to connect...\n";
    SOCKET clientSock = accept(listenSock, NULL, NULL);
    cout << "Client connected.\n";

    srand((unsigned int)time(0));
    bool serverIsX = (rand() % 2 == 0);
    bool serverGoesFirst = (rand() % 2 == 0);

    char serverSym = serverIsX ? 'X' : 'O';
    char clientSym = serverIsX ? 'O' : 'X';

    sendLine(clientSock, string("SYM:") + clientSym);
    sendLine(clientSock, serverGoesFirst ? "SECOND" : "FIRST");

    bool serverTurn = serverGoesFirst;
    bool gameOver = false;

    while (!gameOver) {
        printBoard();

        int row = -1, col = -1;
        bool validMove = false;

        if (serverTurn) {
            while (!validMove) {
                cout << "Your turn (" << serverSym << "). Enter Coordinates: ";
                string input;
                cin >> input;
                if (input.size() == 2 && isdigit(input[0]) && isdigit(input[1])) {
                    col = input[0] - '0';
                    row = input[1] - '0';
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row * 3 + col] == ' ')
                        validMove = true;
                    else
                        cout << "Invalid move, try again.\n";
                }
                else {
                    cout << "Invalid input, try again.\n";
                }
            }
            board[row * 3 + col] = serverSym;
        }
        else {
            sendLine(clientSock, "TURN");
            while (!validMove) {
                string msg = recvLine(clientSock);
                if (msg.rfind("MOVE:", 0) == 0 && msg.size() == 7) {
                    col = msg[5] - '0';
                    row = msg[6] - '0';
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row * 3 + col] == ' ')
                        validMove = true;
                    else
                        sendLine(clientSock, "INVALID");
                }
                else {
                    sendLine(clientSock, "INVALID");
                }
            }
            board[row * 3 + col] = clientSym;
        }

        sendLine(clientSock, "BOARD:" + string(board, 9));

        char justMoved = serverTurn ? serverSym : clientSym;
        if (checkWin(justMoved)) {
            printBoard();
            cout << "Player " << justMoved << " wins!\n";
            sendLine(clientSock, string("WIN:") + justMoved);
            gameOver = true;
        }
        else if (boardFull()) {
            printBoard();
            cout << "It's a draw!\n";
            sendLine(clientSock, "DRAW");
            gameOver = true;
        }
        else {
            serverTurn = !serverTurn;
        }
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();

    cout << "Press enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}