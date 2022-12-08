#include "computer.h"
#include "player.h"
#include "pieces.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <random>

// Lookup function for piece weights
int materialWeight(char type) {
    if (type == 'p' || type == 'P') return 1;
    if (type == 'n' || type == 'N') return 3;
    if (type == 'b' || type == 'B') return 3;
    if (type == 'r' || type == 'R') return 5;
    if (type == 'q' || type == 'Q') return 9;
    return 0;
}

// Computer constructor
Computer::Computer(bool isWhite, Board* board):
    Player{isWhite, board}, personalMoves{std::vector<int> (std::vector<int> (0))},
    enemyMoves{std::vector<int> (std::vector<int> (0))}
{};

// Promote function for computer, defaults to queen
void Computer::promote(int piece) {
    bool isWhiteReplace = board->boardState[piece]->isWhite;
    delete board->boardState[piece];
    board->boardState[piece] = new Queen(board, isWhiteReplace, piece, 'Q');
}


// Adjusts move weights for level 2 computer
int returnWeightLevel2(Computer* comp, int start, int end) {
    if (!comp->board->boardState[end]->isEmpty) {
        return 5;
    }

    comp->board->boardState[start]->location = end;
    comp->board->boardState[end]->location = start;
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    int checkState = 0;
    checkState = comp->board->boardInCheck(false);
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    comp->board->boardState[start]->location = start;
    comp->board->boardState[end]->location = end;
    for (auto i : comp->board->boardState)
    {
        i->updateMoves(false);
    }

    if ((comp->isWhite && checkState == -2) || (!comp->isWhite && checkState == 2))
    {
        return 20;
    }
    if ((comp->isWhite && checkState == -1) || (!comp->isWhite && checkState == 1))
    {
        return 10;
    }
    return 1;
}

// Level 1 computer constructor
Level1::Level1(bool isWhite, Board* board): Computer(isWhite, board) {};

// Move function for level 1 computer
void Level1::getmove() {
    updateData(1);
    int randomMove = (std::rand() % (personalMoves.size()));
    move(personalMoves[randomMove][0], personalMoves[randomMove][1]);
}


// Level 2 computer constructor
Level2::Level2(bool isWhite, Board* board): Computer(isWhite, board) {};

// Move function for level 2 computer
void Level2::getmove() {
    updateData(2);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(personalMoves.begin(), personalMoves.end(), g);
    int bestMove = 0;
    int bestMoveWeight = 0;
    int length = personalMoves.size();
    for (int i = 0; i < length; i++)
    {
        if (personalMoves[i][2] > bestMoveWeight)
        {
            bestMove = i;
            bestMoveWeight = personalMoves[i][2];
        }
    }
    move(personalMoves[bestMove][0], personalMoves[bestMove][1]);
}


// Adjusts move weights for level 3 computer
int returnWeightLevel3(Computer* comp, int start, int end) {
    std::cout << "test 1" << std::endl;
    int weight = 0;
    int change = 0;

    if (!comp->board->boardState[end]->isEmpty) {
        change = materialWeight(comp->board->boardState[start]->type) - materialWeight(comp->board->boardState[end]->type);
        if (change < 0) {
            weight += -change*3;
        } else {
            weight += change;
        }
    }
    std::cout << "test 2" << std::endl;

    for (auto moveVec : comp->enemyMoves) {
        std::cout << "test 2.1" << std::endl;
        for (auto i : moveVec) std::cout << i << ",";
        if (moveVec[1] == start) {
            std::cout << "test 2.2" << std::endl;
            change = materialWeight(comp->board->boardState[moveVec[0]]->type) - materialWeight(comp->board->boardState[start]->type);
            if (change < 0) {
                weight += change*3;
            } else {
                weight += change;
            }
        }
    }
    std::cout << "test 3" << std::endl;

    for (auto moveVec : comp->enemyMoves) {
        if (moveVec[1] == end) {
            change = materialWeight(comp->board->boardState[moveVec[0]]->type) - materialWeight(comp->board->boardState[start]->type);
            if (change < 0) {
                weight -= change*3;
            } else {
                weight -= change;
            }
        }
    }

    std::cout << "test 4" << std::endl;
    comp->board->boardState[start]->location = end;
    comp->board->boardState[end]->location = start;
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    int checkState = 0;
    checkState = comp->board->boardInCheck(false);
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    comp->board->boardState[start]->location = start;
    comp->board->boardState[end]->location = end;
    for (auto i : comp->board->boardState)
    {
        i->updateMoves(false);
    }
    std::cout << "test 5" << std::endl;
    if ((comp->isWhite && checkState == 2) || (!comp->isWhite && checkState == -2)) {
        weight = -9999;
    }
    if ((comp->isWhite && checkState == -2) || (!comp->isWhite && checkState == 2)) {
        weight = 9999;
    }
    if ((comp->isWhite && checkState == 1) || (!comp->isWhite && checkState == -1)) {
        weight -= 20;
    }
    if ((comp->isWhite && checkState == -1) || (!comp->isWhite && checkState == 1)) {
        weight += 20;
    }
    return weight;
}

// Level 3 computer constructor
Level3::Level3(bool isWhite, Board* board): Computer(isWhite, board) {};

// Move function for level 3 computer
void Level3::getmove() {
    updateData(3);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(personalMoves.begin(), personalMoves.end(), g);
    int bestMove = 0;
    int bestMoveWeight = 0;
    int length = personalMoves.size();
    for (int i = 0; i < length; i++)
    {
        if (personalMoves[i][2] > bestMoveWeight)
        {
            bestMove = i;
            bestMoveWeight = personalMoves[i][2];
        }
    }
    move(personalMoves[bestMove][0], personalMoves[bestMove][1]);
}


// Adjusts move weights for level 4 computer
int returnWeightLevel4(Computer* comp, int start, int end) {
    int weight = 0;
    int change = 0;

    if (!comp->board->boardState[end]->isEmpty) {
        change = materialWeight(comp->board->boardState[start]->type) - materialWeight(comp->board->boardState[end]->type);
        if (change < 0) {
            weight += -change*3;
        } else {
            weight += change;
        }
    }

    if (comp->board->boardState[start]->type == 'k') {
        int piecesCount = 0;
        for (auto piece : comp->board->boardState) {
            if (!piece->isEmpty) piecesCount++;
        }
        weight += (8 - (start - (start % 8)) / 8) * (64 - piecesCount) / 15;
    }
    if (comp->board->boardState[start]->type == 'K') {
        int piecesCount = 0;
        for (auto piece : comp->board->boardState) {
            if (!piece->isEmpty) piecesCount++;
        }
        weight += ((start - (start % 8)) / 8) * (64 - piecesCount) / 15;
    }

    if (comp->board->boardState[start]->type == 'p') {
        weight += (8 - (start - (start % 8)) / 8) / 2;
    }
    if (comp->board->boardState[start]->type == 'P') {
        weight += ((start - (start % 8)) / 8) / 2;
    }

    for (auto moveVec : comp->enemyMoves) {
        if (moveVec[1] == start) {
            change = materialWeight(comp->board->boardState[moveVec[0]]->type) - materialWeight(comp->board->boardState[start]->type);
            if (change < 0) {
                weight += change*3;
            } else {
                weight += change;
            }
        }
    }

    for (auto moveVec : comp->enemyMoves) {
        if (moveVec[1] == end) {
            change = materialWeight(comp->board->boardState[moveVec[0]]->type) - materialWeight(comp->board->boardState[start]->type);
            if (change < 0) {
                weight -= change*3;
            } else {
                weight -= change;
            }
        }
    }

    comp->board->boardState[start]->location = end;
    comp->board->boardState[end]->location = start;
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    int checkState = 0;
    checkState = comp->board->boardInCheck(false);
    std::iter_swap(comp->board->boardState.begin() + start, comp->board->boardState.begin() + end);
    comp->board->boardState[start]->location = start;
    comp->board->boardState[end]->location = end;
    for (auto i : comp->board->boardState)
    {
        i->updateMoves(false);
    }
    if ((comp->isWhite && checkState == 2) || (!comp->isWhite && checkState == -2)) {
        weight = -9999;
    }
    if ((comp->isWhite && checkState == -2) || (!comp->isWhite && checkState == 2)) {
        weight = 9999;
    }
    if ((comp->isWhite && checkState == 1) || (!comp->isWhite && checkState == -1)) {
        weight -= 20;
    }
    if ((comp->isWhite && checkState == -1) || (!comp->isWhite && checkState == 1)) {
        weight += 20;
    }
    return weight;
}


// Level 4 computer constructor
Level4::Level4(bool isWhite, Board* board): Computer(isWhite, board) {};

// Move function for level 4 computer
void Level4::getmove() {
    updateData(4);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(personalMoves.begin(), personalMoves.end(), g);
    int bestMove = 0;
    int bestMoveWeight = 0;
    int length = personalMoves.size();
    for (int i = 0; i < length; i++)
    {
        if (personalMoves[i][2] > bestMoveWeight)
        {
            bestMove = i;
            bestMoveWeight = personalMoves[i][2];
        }
    }
    move(personalMoves[bestMove][0], personalMoves[bestMove][1]);
}


// Updates the computer's fields with data about the game
void Computer::updateData(int level) {
    for (auto moveVec : personalMoves) {
        moveVec.clear();
    }
    personalMoves.clear();
    for(auto i: board->boardState){
        if (!i->isEmpty && i->isWhite == isWhite) {
            for (auto move: i->legalmoves) {
                std::vector<int> moveVec = {i->location, move};
                personalMoves.emplace_back(moveVec);
            }
        }
        if (level >= 3 && !i->isEmpty && i->isWhite != isWhite) {
            for (auto move: i->legalmoves) {
                std::vector<int> moveVec = {i->location, move};
                enemyMoves.emplace_back(moveVec);
            }
        }
    }

    if (level >= 2) {
        int size = personalMoves.size();
        for (int j = 0; j < size; j++)
        {
            int add = 0;
            if (level == 2) add = returnWeightLevel2(this, personalMoves[j][0], personalMoves[j][1]);
            if (level == 3) add = returnWeightLevel3(this, personalMoves[j][0], personalMoves[j][1]);
            if (level == 4) add = returnWeightLevel4(this, personalMoves[j][0], personalMoves[j][1]);
            personalMoves[j].emplace_back(add);
        }
    }

}