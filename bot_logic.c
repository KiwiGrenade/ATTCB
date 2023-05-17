#include <stdbool.h>
#include <stdio.h>

#include "bot_logic.h"
#include "board.h"

#define player_max 2
#define player_min 1

int32_t max(const int32_t num1, const int32_t num2)
{
    if (num1 > num2) return num1;
    return num2;
}

int32_t min(const int32_t num1, const int32_t num2)
{
    if (num1 < num2) return num1;
    return num2;
}

int32_t checkForFault(const int32_t boardCopy[5][5])
{
    if(dummyLoseCheck(boardCopy, player_min))
    {
        return 100000;
    }
    else if(dummyLoseCheck(boardCopy, player_max))
    {
        return -100000;
    }
    return 0;
}

int32_t checkForWin(const int32_t boardCopy[5][5])
{
    if(dummyWinCheck(boardCopy, player_min))
    {
        return -100000;
    }
    else if(dummyWinCheck(boardCopy, player_max))
    {
        return 100000;
    }
    return 0;
}

int32_t changeScore(const int32_t player, const int points, int32_t score) {
    if (player == player_max)
    {
        return (score + points);
    }
    else if(player == player_min)
    {
        return (score - points);
    }
    return score;
}

int32_t evaluate(const int32_t boardCopy[ROWS][COLS])
{
    int32_t score = 0;
    size_t i, j;

    const int32_t winningPoints = 100000;
    const int32_t triplePoints = 100;
    const int32_t doublePoints = 30;
    const int32_t singlePoints = 1;

//
    score += checkForFault(boardCopy);
//    score += checkForWin(boardCopy);
//    printf(" Val:%d", score);


    //check rows
    for (i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS - 1; j++)
        {
            // |x x - - -| or |- x x - -| or |- - x x - | or |- - - x x|
            score = changeScore(boardCopy[i][j], singlePoints, score);
            if (boardCopy[i][j] == boardCopy[i][j + 1])
            {
                score = changeScore(boardCopy[i][j], doublePoints, score);
                if(j < 2)
                {
                    if(boardCopy[i][j + 3] == boardCopy[i][j])
                    {
                        if(boardCopy[i][j + 2] == boardCopy[i][j])
                        {
                            return changeScore(boardCopy[i][j], winningPoints, score);
                        }
                        else if(boardCopy[i][j + 2] == 0)
                        {
                            score = changeScore(boardCopy[i][j], triplePoints, score);
                        }
                    }
                }
                else
                {
                    if (boardCopy[i][j - 1] == 0 && boardCopy[i][j - 2] == boardCopy[i][j])
                    {
                        score = changeScore(boardCopy[i][j], triplePoints, score);
                    }
                }
            }
        }
    }

    //check columns
    for (j = 0; j < COLS; j++)
    {
        for(i = 0; i < ROWS; i++)
        {
            if (boardCopy[i][j] == boardCopy[i + 1][j])
            {
                score = changeScore(boardCopy[i][j], doublePoints, score);
                if(i < 2)
                {
                    if(boardCopy[i + 3][j] == boardCopy[i][j])
                    {
                        if(boardCopy[i + 2][j] == boardCopy[i][j])
                        {
                            return changeScore(boardCopy[i][j], winningPoints, score);
                        }
                        else if (boardCopy[i + 2][j] == 0)
                        {
                            score = changeScore(boardCopy[i][j], triplePoints, score);
                        }
                    }
                }
                else
                {
                    if(boardCopy[i - 1][j] == 0 && boardCopy[i - 2][j] == boardCopy[i][j])
                    {
                        score = changeScore(boardCopy[i][j], triplePoints, score);
                    }
                }
            }
        }
    }

    //check left diagonal
    for (i = 0; i < ROWS - 1; i++)
    {
        for(j = 0; j < COLS - 1; j++)
        {
            if (boardCopy[i][j] == boardCopy[i + 1][j + 1])
            {
                score = changeScore(boardCopy[i][j], doublePoints, score);
                if(j < 2)
                {
                    if(boardCopy[i + 3][j + 3] == boardCopy[i][j])
                    {
                        if(boardCopy[i + 2][j + 2] == boardCopy[i][j])
                        {
                            return changeScore(boardCopy[i][j], winningPoints, score);
                        }
                        else if(boardCopy[i + 2][j + 2] == 0)
                        {
                            score = changeScore(boardCopy[i][j], triplePoints, score);
                        }
                    }
                }
//                else
//                {
//                    score = changeScore(boardCopy[i][j], 2, score);
//                }
            }
        }
    }

    //check right diagonal
    for (i = 0; i < ROWS - 1; i++)
    {
        for(j = 1; j < COLS; j++)
        {
            if (boardCopy[i][j] == boardCopy[i + 1][j - 1])
            {
                score = changeScore(boardCopy[i][j], doublePoints, score);
                if(boardCopy[i + 3][j - 3] == boardCopy[i][j])
                {
                    if(boardCopy[i + 2][j - 2] == boardCopy[i][j])
                    {
                        return changeScore(boardCopy[i][j], winningPoints, score);
                    }
                    else if(boardCopy[i + 2][j - 2] == 0)
                    {
                        score = changeScore(boardCopy[i][j], triplePoints, score);
                    }
                }
//                else
//                {
//                    score = changeScore(boardCopy[i][j], 2, score);
//                }
            }
        }
    }

    return score;
}

int32_t minimax(int32_t boardCopy[ROWS][COLS], const uint8_t depth, const uint8_t is_maximizing_player, int32_t alpha, int32_t beta)
{
    int32_t score = evaluate(boardCopy);
    if (score > 1000 || score < -1000 || depth == 0)
        return score;

    int32_t val;
    if (is_maximizing_player) {
        int32_t maxEval = -100000;
        for (size_t i = 0; i < ROWS; i++) {
            for (size_t j = 0; j < COLS; j++) {
                if (boardCopy[i][j] == 0) {
                    boardCopy[i][j] = player_max;

                    val = minimax(boardCopy, depth - 1, false, alpha, beta);
                    maxEval = max(maxEval, val);
                    alpha = max(alpha, maxEval);

                    boardCopy[i][j] = 0;

                    if (beta <= alpha) break;
                }
            }
        }
        return maxEval;
    }
    else
    {
        int32_t minEval = 100000;
        for (size_t i = 0; i < ROWS; i++)
        {
            for (size_t j = 0; j < COLS; j++)
            {
                if (boardCopy[i][j] == 0)
                {
                    boardCopy[i][j] = player_min;

                    val = minimax(boardCopy, depth - 1, true, alpha, beta);
                    minEval = min(minEval, val);
                    beta = min(beta, minEval);

                    boardCopy[i][j] = 0;

                    if (beta <= alpha) break;
                }
            }
        }
        return minEval;
    }
}

uint8_t bestMove(const uint8_t depth, const uint8_t player)
{
    int32_t best_val;
    uint8_t best_move = 0;
    uint8_t is_maximizing_player = player - 1;

    if (is_maximizing_player)
    {
        best_val = -100000;
    }
    else
    {
        best_val = 100000;
    }
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
        {
            if (board[i][j] == 0)
            {
                if (is_maximizing_player)
                {
                    board[i][j] = player_max;

                    int8_t move_val = minimax(board, depth - 1, false, -100000, 100000);

                    board[i][j] = 0;

                    if (move_val >= best_val) {
                        best_val = move_val;
                        best_move = ((i + 1) * 10) + (j + 1);
                    }
                }
                else
                {
                    board[i][j] = player_min;

                    int8_t move_val = minimax(board, depth - 1, true, -100000, 100000);

                    board[i][j] = 0;

                    if (move_val <= best_val) {
                        best_val = move_val;
                        best_move = ((i + 1) * 10) + (j + 1);
                    }
                }
            }
        }
    }
    printf("%d ", best_val);
    return best_move;
}
