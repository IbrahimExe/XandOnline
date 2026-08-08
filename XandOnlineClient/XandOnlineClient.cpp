#include <iostream>
#include <string>
#include <cctype>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// Client Side
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

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    cout << "Connecting to server...\n";
    connect(sock, (sockaddr*)&addr, sizeof(addr));
    cout << "Connected.\n";

    string symMsg = recvLine(sock);
    char mySym = symMsg[4];
    string firstMsg = recvLine(sock);

    if (firstMsg == "FIRST")
        cout << "You are " << mySym << ". You go first.\n";
    else
        cout << "You are " << mySym << ". Server goes first.\n";

    cout << "Press enter to start...";
    cin.get();
    printBoard();

    bool gameOver = false;

    while (!gameOver) {
        string msg = recvLine(sock);

        if (msg.rfind("BOARD:", 0) == 0) {
            string b = msg.substr(6, 9);
            for (int i = 0; i < 9; i++) board[i] = b[i];
            printBoard();
        }
        else if (msg == "TURN") {
            bool validMove = false;
            while (!validMove) {
                cout << "Your turn (" << mySym << "). Enter Coordinates: ";
                string input;
                cin >> input;
                if (input.size() == 2 && isdigit(input[0]) && isdigit(input[1])) {
                    sendLine(sock, "MOVE:" + input);
                    string reply = recvLine(sock);
                    if (reply == "INVALID") {
                        cout << "Invalid move, try again.\n";
                    }
                    else if (reply.rfind("BOARD:", 0) == 0) {
                        string b = reply.substr(6, 9);
                        for (int i = 0; i < 9; i++) board[i] = b[i];
                        printBoard();
                        validMove = true;
                    }
                }
                else {
                    cout << "Invalid input, try again.\n";
                }
            }
        }
        else if (msg.rfind("WIN:", 0) == 0) {
            cout << "Player " << msg[4] << " wins!\n";
            gameOver = true;
        }
        else if (msg == "DRAW") {
            cout << "It's a draw!\n";
            gameOver = true;
        }
    }

    closesocket(sock);
    WSACleanup();

    cout << "Press enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}