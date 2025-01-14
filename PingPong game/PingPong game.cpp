#include <raylib.h>
#include <iostream>

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player1_score = 0;
int player2_score = 0;

struct Ball {
    float x, y;
    int speed_x, speed_y;
    int radius;
};

struct Paddle {
    float x, y;
    float width, height;
    int speed;
    int up_key, down_key;
};

void InitializeBall(Ball& b, int screen_width, int screen_height) {
    b.x = screen_width / 2;
    b.y = screen_height / 2;
    b.speed_x = 7;
    b.speed_y = 7;
    b.radius = 20;
}

void InitializePaddle(Paddle& p, float x, float y, float width, float height, int speed, int up_key, int down_key) {
    p.x = x;
    p.y = y;
    p.width = width;
    p.height = height;
    p.speed = speed;
    p.up_key = up_key;
    p.down_key = down_key;
}

void ResetBall(Ball& b, int screen_width, int screen_height) {
    b.x = screen_width / 2;
    b.y = screen_height / 2;

    int speed_choices[2] = { -1, 1 };
    b.speed_x *= speed_choices[GetRandomValue(0, 1)];
    b.speed_y *= speed_choices[GetRandomValue(0, 1)];
}

void UpdateBall(Ball& b, int screen_width, int screen_height) {
    b.x += b.speed_x;
    b.y += b.speed_y;

    if (b.y + b.radius >= screen_height || b.y - b.radius <= 0) {
        b.speed_y *= -1;
    }

    if (b.x + b.radius >= screen_width) {
        player1_score++;
        ResetBall(b, screen_width, screen_height);
    }

    if (b.x - b.radius <= 0) {
        player2_score++;
        ResetBall(b, screen_width, screen_height);
    }
}

void UpdatePaddle(Paddle& p) {
    if (IsKeyDown(p.up_key)) {
        p.y -= p.speed;
    }
    if (IsKeyDown(p.down_key)) {
        p.y += p.speed;
    }

    if (p.y <= 0) {
        p.y = 0;
    }
    if (p.y + p.height >= GetScreenHeight()) {
        p.y = GetScreenHeight() - p.height;
    }
}

void DrawBall(const Ball& b) {
    DrawCircle(b.x, b.y, b.radius, Yellow);
}

void DrawPaddle(const Paddle& p) {
    DrawRectangleRounded(Rectangle{ p.x, p.y, p.width, p.height }, 0.8, 0, WHITE);
}

int main() {
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game!");
    SetTargetFPS(60);

    Ball ball;
    InitializeBall(ball, screen_width, screen_height);

    Paddle player1, player2;
    InitializePaddle(player1, screen_width - 35, screen_height / 2 - 60, 25, 120, 6, KEY_UP, KEY_DOWN);
    InitializePaddle(player2, 10, screen_height / 2 - 60, 25, 120, 6, KEY_W, KEY_S);

    while (!WindowShouldClose()) {
        // Update game logic
        UpdateBall(ball, screen_width, screen_height);
        UpdatePaddle(player1);
        UpdatePaddle(player2);

        // Check for collisions
        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player1.x, player1.y, player1.width, player1.height })) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player2.x, player2.y, player2.width, player2.height })) {
            ball.speed_x *= -1;
        }

        // Draw game
        BeginDrawing();
        ClearBackground(Dark_Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        DrawBall(ball);
        DrawPaddle(player1);
        DrawPaddle(player2);

        DrawText(TextFormat("%i", player2_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player1_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
