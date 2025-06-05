#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 20
#define GRID_WIDTH 40
#define GRID_HEIGHT 30
#define SCREEN_WIDTH (CELL_SIZE * GRID_WIDTH)
#define SCREEN_HEIGHT (CELL_SIZE * GRID_HEIGHT)

typedef struct {
    int x;
    int y;
} Vector2i;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game - Raylib");
    InitAudioDevice();

    // Charger musique (ajoute un fichier "background.mp3" dans le même dossier)
    Music music = LoadMusicStream("background.mp3");
    PlayMusicStream(music);

    SetTargetFPS(10);
    srand(time(NULL));

    // Initialisation du serpent
    Vector2i snake[GRID_WIDTH * GRID_HEIGHT] = {0};
    int snakeLength = 1;
    snake[0].x = GRID_WIDTH / 2;
    snake[0].y = GRID_HEIGHT / 2;

    Vector2i fruit;
    fruit.x = rand() % GRID_WIDTH;
    fruit.y = rand() % GRID_HEIGHT;

    int dirX = 0;
    int dirY = 0;

    int gameOver = 0;
    int score = 0;
    bool paused = false;

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        // Gestion pause avec P
        if (IsKeyPressed(KEY_P)) paused = !paused;
        if (paused) {
            // Pause musique si jeu en pause
            PauseMusicStream(music);
        } else {
            ResumeMusicStream(music);
        }

        if (!paused && !gameOver) {
            // Gestion des entrées
            if (IsKeyPressed(KEY_UP) && dirY != 1) {
                dirX = 0; dirY = -1;
            }
            else if (IsKeyPressed(KEY_DOWN) && dirY != -1) {
                dirX = 0; dirY = 1;
            }
            else if (IsKeyPressed(KEY_LEFT) && dirX != 1) {
                dirX = -1; dirY = 0;
            }
            else if (IsKeyPressed(KEY_RIGHT) && dirX != -1) {
                dirX = 1; dirY = 0;
            }

            // Déplacer le serpent si direction non nulle
            if (dirX != 0 || dirY != 0) {
                // Déplacer le corps
                for (int i = snakeLength - 1; i > 0; i--) {
                    snake[i] = snake[i - 1];
                }
                // Déplacer la tête
                snake[0].x += dirX;
                snake[0].y += dirY;

                // Wrap-around : passage d’un côté à l’autre
                if (snake[0].x < 0) snake[0].x = GRID_WIDTH - 1;
                else if (snake[0].x >= GRID_WIDTH) snake[0].x = 0;
                if (snake[0].y < 0) snake[0].y = GRID_HEIGHT - 1;
                else if (snake[0].y >= GRID_HEIGHT) snake[0].y = 0;

                // Collision corps
                for (int i = 1; i < snakeLength; i++) {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                        gameOver = 1;
                    }
                }

                // Manger fruit
                if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                    score++;
                    snakeLength++;
                    // Générer nouveau fruit
                    do {
                        fruit.x = rand() % GRID_WIDTH;
                        fruit.y = rand() % GRID_HEIGHT;
                        // Vérifier que fruit n'apparaisse pas sur le serpent
                        bool onSnake = false;
                        for (int i = 0; i < snakeLength; i++) {
                            if (fruit.x == snake[i].x && fruit.y == snake[i].y) {
                                onSnake = true;
                                break;
                            }
                        }
                        if (!onSnake) break;
                    } while (1);
                }
            }
        }

        // Dessin
        BeginDrawing();

        // Fond bleu ciel #87CEEB (RGB 135,206,235)
        ClearBackground((Color){135, 206, 235, 255});

        if (gameOver) {
            DrawText("GAME OVER!", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 40, 40, RED);
            DrawText(TextFormat("Score final: %d", score), SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 10, 30, WHITE);
            DrawText("Appuyez sur R pour rejouer", SCREEN_WIDTH/2 - 110, SCREEN_HEIGHT/2 + 60, 20, WHITE);

            if (IsKeyPressed(KEY_R)) {
                // Reset jeu
                snakeLength = 1;
                snake[0].x = GRID_WIDTH / 2;
                snake[0].y = GRID_HEIGHT / 2;
                dirX = 0;
                dirY = 0;
                score = 0;
                gameOver = 0;
                fruit.x = rand() % GRID_WIDTH;
                fruit.y = rand() % GRID_HEIGHT;
            }
        } else {
            // Dessiner serpent
            for (int i = 0; i < snakeLength; i++) {
                DrawRectangle(snake[i].x * CELL_SIZE, snake[i].y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
            }

            // Dessiner fruit
            DrawRectangle(fruit.x * CELL_SIZE, fruit.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);

            // Afficher score et pause
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
            if (paused) {
                DrawText("PAUSE (P pour reprendre)", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2, 30, DARKGRAY);
            }
        }

        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
