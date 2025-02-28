#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else

#endif

#define MAX_SNAKE_LENGTH 100
#define MAX_OBSTACLES 20
#define FILE_NAME "highscore.txt"
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int width = 40, height = 40, gameOver, score, highScore, snakeX[MAX_SNAKE_LENGTH], snakeY[MAX_SNAKE_LENGTH], snakeLength, foodX, foodY, direction, obstacleX[MAX_OBSTACLES], obstacleY[MAX_OBSTACLES], numObstacles, level, mode;

#define PASSWORD "sazid0519"

void readHighScore() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void writeHighScore() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

void initializeGame() {
    gameOver = 0;
    score = 0;
    snakeLength = 1;
    snakeX[0] = width / 2;
    snakeY[0] = height / 2;
    direction = RIGHT;
    level = 1;
    numObstacles = 3;
    foodX = rand() % (width - 2) + 1;
    foodY = rand() % (height - 2) + 1;
    for (int i = 0; i < numObstacles; i++) {
        obstacleX[i] = rand() % (width - 2) + 1;
        obstacleY[i] = rand() % (height - 2) + 1;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else

#endif
}

void drawBoard() {
    clearScreen();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                printf("#");
            } else if (i == foodY && j == foodX) {
                printf("F");
            } else {
                int isSnake = 0;
                for (int k = 0; k < snakeLength; k++) {
                    if (snakeX[k] == j && snakeY[k] == i) {
                        printf("O");
                        isSnake = 1;
                        break;
                    }
                }
                if (!isSnake) {
                    int isObstacle = 0;
                    for (int k = 0; k < numObstacles; k++) {
                        if (obstacleX[k] == j && obstacleY[k] == i) {
                            printf("X");
                            isObstacle = 1;
                            break;
                        }
                    }
                    if (!isObstacle) printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d  High Score: %d  Level: %d\n", score, highScore, level);
}

int kbhit(void) {
#ifdef _WIN32
    return _kbhit();
#else

#endif
}

void input() {
    if (kbhit()) {
        char ch = getchar();
        switch (ch) {
            case 'w': case 'W': if (direction != DOWN) direction = UP; break;
            case 's': case 'S': if (direction != UP) direction = DOWN; break;
            case 'a': case 'A': if (direction != RIGHT) direction = LEFT; break;
            case 'd': case 'D': if (direction != LEFT) direction = RIGHT; break;
            case 'q': case 'Q': gameOver = 1; break;
#ifdef _WIN32
            case 72: if (direction != DOWN) direction = UP; break;
            case 80: if (direction != UP) direction = DOWN; break;
            case 75: if (direction != RIGHT) direction = LEFT; break;
            case 77: if (direction != LEFT) direction = RIGHT; break;
#else

#endif
        }
    }
}

void generateObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        obstacleX[i] = rand() % (width - 2) + 1;
        obstacleY[i] = rand() % (height - 2) + 1;
    }
}

void logic() {
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    switch (direction) {
        case UP: snakeY[0]--; break;
        case DOWN: snakeY[0]++; break;
        case LEFT: snakeX[0]--; break;
        case RIGHT: snakeX[0]++; break;
    }

    if (mode != 2) {
        if (snakeX[0] <= 0 || snakeX[0] >= width - 1 || snakeY[0] <= 0 || snakeY[0] >= height - 1) gameOver = 1;
    } else { // Noob mode
        if (snakeX[0] <= 0) snakeX[0] = width - 2;
        if (snakeX[0] >= width - 1) snakeX[0] = 1;
        if (snakeY[0] <= 0) snakeY[0] = height - 2;
        if (snakeY[0] >= height - 1) snakeY[0] = 1;
    }

    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) gameOver = 1;
    }

    for (int i = 0; i < numObstacles; i++) {
        if (snakeX[0] == obstacleX[i] && snakeY[0] == obstacleY[i]) gameOver = 1;
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score += 10;
        if (score > highScore) {
            highScore = score;
            writeHighScore();
        }

        snakeLength++;
        foodX = rand() % (width - 2) + 1;
        foodY = rand() % (height - 2) + 1;

        if (score % 50 == 0) {
            level++;
            numObstacles++;
            if (numObstacles > MAX_OBSTACLES) numObstacles = MAX_OBSTACLES;
            generateObstacles();
        }
    }
}

void setGameParameters() {
    printf("Enter grid width (min 10, default 40): ");
    scanf("%d", &width);
    printf("Enter grid height (min 10, default 40): ");
    scanf("%d", &height);

    if (width < 10) width = 40;
    if (height < 10) height = 40;

    printf("Select game mode (0: Normal, 1: Extreme, 2: Noob): ");
    scanf("%d", &mode);
    if (mode < 0 || mode > 2) mode = 0;
}

int checkPassword() {
    char inputPassword[50];

    printf("Enter the password to play the game: ");
    scanf("%s", inputPassword);

    if (strcmp(inputPassword, PASSWORD) == 0) {
        return 1;
    } else {
        printf("Incorrect password. Exiting...\n");
        return 0;
    }
}

int main() {
    srand(time(0));
    readHighScore();

    if (!checkPassword()) {
        return 0;
    }

    setGameParameters();
    initializeGame();

    while (!gameOver) {
        drawBoard();
        input();
        logic();

        int speed;
        if (mode == 2) {
            speed = 2000;
        } else {
            speed = 200 - (level * 20);
            if (speed < 50) speed = 50;
        }

#ifdef _WIN32
        Sleep(speed);
#else

#endif
    }

    printf("Game Over! Final Score: %d\n", score);
    printf("Press any key to exit...\n");
    getchar();
    return 0;
}