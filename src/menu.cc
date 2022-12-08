#include "game.h"
#include <iostream>
#include "board_set.h"
#include "window.h"
using namespace std;

// Basic I/O Driver To Start Games
int main(int argc, char const *argv[])
{
    cout << "+-----------------------------------------------------+" << endl;
    cout << "|   _____                  _____ _                    |" << endl;
    cout << "|  / ____|    _     _     / ____| |                   |" << endl;
    cout << "| | |       _| |_ _| |_  | |    | |__   ___  ___ ___  |" << endl;
    cout << "| | |      |_   _|_   _| | |    | '_ \\ / _ \\/ __/ __| |" << endl;
    cout << "| | |____    |_|   |_|   | |____| | | |  __/\\__ \\__ \\ |" << endl;
    cout << "|  \\_____|                \\_____|_| |_|\\___||___/___/ |" << endl;
    cout << "+-----------------------------------------------------+ " << endl;
    cout << "|    BY ABDULLAH SHAHID, ARIQ ISHFAR, & SOHAM NAGI    |" << endl;
    cout << "+-----------------------------------------------------+" << endl;

    double black_wins = 0;
    double white_wins = 0;
    double draw = 0;

    string command;
    Board *start_board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    cout << "Welcome to chess++!" << endl;
    cout << "You are in the main menu, here are your commands:" << endl;
    cout << "\"game\" to start a round of chess. " << endl;
    cout << "\"setup\" to enter setup mode. " << endl;
    cout << "\"quit\" to end the program and see results. " << endl;

    while (cin >> command)
    {
        if (command == "setup")
        {
            delete start_board;
            start_board = board_setup(nullptr, " w", 1);
            cout << "You are in the main menu, here are your commands:" << endl;
            cout << "\"game\" to start a round of chess. " << endl;
            cout << "\"setup\" to enter setup mode. " << endl;
            cout << "\"quit\" to end the program and see results. " << endl;
        }
        else if (command == "game")
        {
            if (start_board == nullptr)
            {
                start_board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            }
            string white;
            string black;

            cout << "Select if white is to be a human player or a computer player from levels 1 to 4." << endl;
            cout << "(human/1/2/3/4):" << endl;

            while (true)
            {
                cin >> white;

                if (cin.fail() || white == "human" || white == "1" || white == "2" || white == "3" || white == "4")
                {
                    break;
                }

                cout << "Invalid command for white! Try again." << endl;
            }


            cout << "Select if black is to be a human player or a computer player from levels 1 to 4." << endl;
            cout << "(human/1/2/3/4):" << endl;

            while (true)
            {
                cin >> black;

                if (cin.fail() || white == "human" || white == "1" || white == "2" || white == "3" || white == "4")
                {
                    break;
                }

                cout << "Invalid command for black! Try again." << endl;
            }
            Game *round = new Game(start_board, white, black);
            round->notifyObservers();
            round->start();
            int score = round->result;
            if (score == -1)
            {
                ++black_wins;
            }
            else if (score == 0)
            {
                ++draw;
            }
            else
            {
                ++white_wins;
            }
            delete round;
            delete start_board;
            start_board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }
        else if (command == "quit")
        {
            delete start_board;
            break;
        }
        else
        {
            cout << "Invalid command! Try again!" << endl;
        }
    }

    cout << "Final Score:" << endl;
    cout << "White: " << white_wins + (draw / 2) << endl;
    cout << "Black: " << black_wins + (draw / 2) << endl;
}
