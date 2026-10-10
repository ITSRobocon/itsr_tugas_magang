#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;
const char playerAi = 'O';

char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

int evaluate(char board[3][3], char playerAi) {
    char playerX = (playerAi == 'X') ? 'O' : 'X'; // Determine the opponent's mark
    // Check rows for winner
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == playerAi) return 10;
            else if (board[row][0] == playerX) return -10;
        }
    }
    // Check columns for winner
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == playerAi) return 10;
            else if (board[0][col] == playerX) return -10;
        }
    }
    // Check diagonals for winner
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == playerAi) return 10;
        else if (board[0][0] == playerX) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == playerAi) return 10;
        else if (board[0][2] == playerX) return -10;
    }
    return 0; // No winner
}

int isBoardFull(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return 0; // Board is not full
        }
    }
    return 1; // Board is full
}

int minimax(char board[3][3], int depth, bool isMaximizing, char playerAi) {
    // Check for terminal states (win/loss/draw)
    int score = evaluate(board, playerAi);
    if (score == 10) return score - depth; // AI wins
    if (score == -10) return score + depth; // Player wins
    if (isBoardFull(board)) return 0; // Draw

    if (isMaximizing) {
        int bestScore = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = playerAi;
                    bestScore = max(bestScore, minimax(board, depth + 1, false, playerAi));
                    board[i][j] = ' ';
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        char playerX = (playerAi == 'X') ? 'O' : 'X'; // Determine the opponent's mark
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = playerX;
                    bestScore = min(bestScore, minimax(board, depth + 1, true, playerAi));
                    board[i][j] = ' ';
                }
            }
        }
        return bestScore;
    }
}

class AI {
public:
    // Function to find the best move for the AI
    pair<int, int> findBestMove(char board[3][3], char playerAi) {
        int bestScore = INT_MIN;
        pair<int, int> bestMove = {-1, -1};
        // Iterate through all cells to find the best move
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // Check if the cell is empty
                if (board[i][j] == ' ') {
                    // Make the move
                    board[i][j] = playerAi;
                    // Evaluate the move
                    int score = minimax(board, 0, false, playerAi);
                    // Undo the move
                    board[i][j] = ' ';
                    // Update the best score and move if necessary
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = {i, j};
                    }
                }
            }
        }
        return bestMove;
    }
};

void aiMove() {
    AI ai;
    pair<int, int> move = ai.findBestMove(board, playerAi);
    if (move.first != -1 && move.second != -1) {
        board[move.first][move.second] = playerAi;
    }
}