#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define SIZE 9

int minutes = 0;
int seconds = 0;
bool gameOver = false;

int calculateScore(int difficulty, int timeTaken) {
    int baseScore = 1000; // Pontuação base
    double difficultyFactor;
    // Fator de ajuste com base na dificuldade
    switch (difficulty) {
        case 1:
            difficultyFactor = 0.5;
            break;
        case 2:
            difficultyFactor = 1.0;
            break;
        case 3:
            difficultyFactor = 1.5;
            break;
        case 4:
            difficultyFactor = 2.0;
            break;
        default:
            difficultyFactor = 1.0;
            break;
    }
    // Ajuste do tempo para calcular a pontuação
    double adjustedTime = timeTaken / (difficultyFactor * 60.0); // Tempo em minutos
    int finalScore = baseScore * difficultyFactor / adjustedTime; // Pontuação final

    return finalScore;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}

void printBoard(int board[SIZE][SIZE], int difficulty) {
    clearScreen();
    printf("Tempo restante: %02d:%02d\n", minutes, seconds);
    for (int row = 0; row < SIZE; row++) {
        if (row % 3 == 0 && row != 0) {
            printf("-----------\n");
        }
        for (int col = 0; col < SIZE; col++) {
            if (col % 3 == 0 && col != 0) {
                printf("|");
            }
            if (difficulty == 4) {
                if (board[row][col] == 0) {
                    printf(".");
                } else {
                    printf("%c", board[row][col]);
                }
            } else {
                if (board[row][col] == 0) {
                    printf(".");
                } else {
                    printf("%d", board[row][col]);
                }
            }
        }
        printf("\n");
    }
}

bool isValidMove(int board[SIZE][SIZE], int row, int col, int num, int difficulty) {
    if (difficulty == 4 && num >= 'a' && num <= 'i') {
        num = num - 'a' + 1;
    }
    for (int x = 0; x < SIZE; x++) {
        if (board[row][x] == num || board[x][col] == num) {
            return false;
        }
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    return true;
}

bool findEmptyLocation(int board[SIZE][SIZE], int *row, int *col) {
    for (*row = 0; *row < SIZE; (*row)++) {
        for (*col = 0; *col < SIZE; (*col)++) {
            if (board[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool solveSudoku(int board[SIZE][SIZE]) {
    int row, col;
    if (!findEmptyLocation(board, &row, &col)) {
        return true;
    }
    for (int num = 1; num <= SIZE; num++) {
        if (isValidMove(board, row, col, num, 0)) {
            board[row][col] = num;
            if (solveSudoku(board)) {
                return true;
            }
            board[row][col] = 0;
        }
    }
    return false;
}

void fillDiagonal(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i += 3) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                int num;
                do {
                    num = rand() % SIZE + 1;
                } while (!isValidMove(board, i + j, i + k, num, 0));
                board[i + j][i + k] = num;
            }
        }
    }
}

void removeNumbers(int board[SIZE][SIZE], int cellsToRemove) {
    while (cellsToRemove > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        while (board[row][col] == 0) {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }
        int backup = board[row][col];
        board[row][col] = 0;
        int boardCopy[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                boardCopy[i][j] = board[i][j];
            }
        }
        if (!solveSudoku(boardCopy)) {
            board[row][col] = backup;
        } else {
            cellsToRemove--;
        }
    }
}

void generateSudoku(int board[SIZE][SIZE], int difficulty) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    fillDiagonal(board);
    solveSudoku(board);
    
    int cellsToRemove;
    if (difficulty == 1) {
        minutes = 30;
        cellsToRemove = SIZE * SIZE / 2;
    } else if (difficulty == 2) {
        minutes = 25;
        cellsToRemove = SIZE * SIZE / 3;
    } else if (difficulty == 3) {
        minutes = 20;
        cellsToRemove = SIZE * SIZE / 4;
    }
    removeNumbers(board, cellsToRemove);
}

void generateSudokuMuitoDificil(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    fillDiagonal(board);
    solveSudoku(board);
    
    int cellsToRemove = SIZE * SIZE / 3;
    removeNumbers(board, cellsToRemove);
    // Substituir números por letras
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                board[i][j] += 'a' - 1;
            }
        }
    }
    minutes = 25;
}

bool isBoardComplete(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

void printMenu() {
    printf("Menu:\n");
    printf("P - Pausar\n");
    printf("R - Reiniciar\n");
    printf("S - Sair\n");
}

void startGame(int board[SIZE][SIZE], int difficulty) {
    bool paused = false;
    if (difficulty == 4) {
        generateSudokuMuitoDificil(board);
    } else {
        generateSudoku(board, difficulty);
    }
    time_t startTime = time(NULL);
    time_t currentTime;
    int previousSecond = -1; // Para rastrear as mudanças de segundo

    while (!isBoardComplete(board) && !gameOver) {
        currentTime = time(NULL);
        int elapsedTime = (int)difftime(currentTime, startTime);
        int remainingTime = minutes * 60 - elapsedTime;

        if (remainingTime <= 0) {
            gameOver = true;
            break;
        }
        // Atualização dos segundos
        if (previousSecond != remainingTime % 60) {
            previousSecond = remainingTime % 60;
            seconds = remainingTime % 60;
        }

        minutes = remainingTime / 60;

        printBoard(board, difficulty);
        printMenu();

        if (paused) {
            printf("Jogo pausado. Digite 'R' para reiniciar ou 'S' para sair: ");
            char command;
            scanf(" %c", &command);
            if (command == 'R' || command == 'r') {
                startGame(board, difficulty);
                return;
            } else if (command == 'S' || command == 's') {
                gameOver = true;
                break;
            } else {
                paused = false;
                startTime = time(NULL) - elapsedTime;
                continue;
            }
        }

        int row, col;
        char input[10];
        int num;
        int result;

        if (difficulty == 4) {
            printf("Digite a linha (1-9), coluna (1-9) e letra (a-i): ");
        } else {
            printf("Digite a linha (1-9), coluna (1-9) e número (1-9): ");
        }

        scanf("%s", input);
        if (input[0] == 'P' || input[0] == 'p') {
            paused = true;
            continue;
        } else if (input[0] == 'R' || input[0] == 'r') {
            startGame(board, difficulty);
            return;
        } else if (input[0] == 'S' || input[0] == 's') {
            gameOver = true;
            break;
        }

        if (difficulty == 4) {
            result = sscanf(input, "%d %d %c", &row, &col, (char *)&num);
            num = tolower(num) - 'a' + 1;
            if (result != 3 || row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
                printf("Entrada invalida! Tente novamente.\n");
                sleep(1);
                continue;
            }
        } else {
            result = sscanf(input, "%d %d %d", &row, &col, &num);
            if (result != 3 || row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
                printf("Entrada invalida! Tente novamente.\n");
                sleep(1);
                continue;
            }
        }

        row--;
        col--;

        if (board[row][col] != 0) {
            printf("Essa posicao ja esta ocupada! Tente novamente.\n");
            sleep(1);
            continue;
        }

        if (difficulty == 4) {
            if (isValidMove(board, row, col, num, difficulty)) {
                board[row][col] = num + 'a' - 1; // Armazenar a letra na posição
            } else {
                printf("Movimento invalido! Tente novamente.\n");
                sleep(1);
            }
        } else {
            if (isValidMove(board, row, col, num, difficulty)) {
                board[row][col] = num;
            } else {
                printf("Movimento invalido! Tente novamente.\n");
                sleep(1);
            }
        }
    }

    if (gameOver) {
        printf("Tempo esgotado! Você perdeu o jogo.\n");
    } else {
        printf("Parabens! Você completou o Sudoku.\n");
        // Calcular e exibir pontuação
        int elapsedTime = (int)difftime(time(NULL), startTime);
        int score = calculateScore(difficulty, elapsedTime);
        printf("Pontuação: %d\n", score);
    }
}

int main() {
    srand(time(NULL));
    int board[SIZE][SIZE];
    int difficulty;
    int choice;

    do {
        printf("Escolha o nivel de dificuldade: \n");
        printf("1. Facil (30 minutos, metade do tabuleiro preenchido)\n");
        printf("2. Normal (25 minutos, um terço do tabuleiro preenchido)\n");
        printf("3. Dificil (20 minutos, um quarto do tabuleiro preenchido)\n");
        printf("4. Muito dificil (25 minutos, um terco do tabuleiro preenchido, numeros substituidos por letras)\n");
        printf("5. Sair do jogo\n");
        printf("Digite o numero correspondente ao nivel de dificuldade ou opcao: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
            case 2:
            case 3:
            case 4:
                difficulty = choice;
                startGame(board, difficulty);
                break;
            case 5:
                printf("Saindo do jogo...\n");
                return 0;
            default:
                printf("Opção invalida! Tente novamente.\n");
        }

        printf("Deseja jogar novamente? (1 - Sim, 0 - Nao): ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            gameOver = false;
        }

    } while (choice == 1);

    return 0;
}
