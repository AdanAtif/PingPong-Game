#include <raylib.h>
#include <iostream>

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;

struct Ball {
    float x, y;
    int speed_x, speed_y;
    int radius;
};

struct Paddle {
    float x, y;
    float width, height;
    int speed;
};

Ball ball;
Paddle player;
Paddle cpu;

void DrawBall(Ball& b) {
    DrawCircle(b.x, b.y, b.radius, Yellow);
}


void ResetBall(Ball& b) {
    b.x = GetScreenWidth() / 2;
    b.y = GetScreenHeight() / 2;

    int speed_choices[2] = { -1, 1 };
    b.speed_x *= speed_choices[GetRandomValue(0, 1)];
    b.speed_y *= speed_choices[GetRandomValue(0, 1)];
}

void UpdateBall(Ball& b) {
    b.x += b.speed_x;
    b.y += b.speed_y;

    if (b.y + b.radius >= GetScreenHeight() || b.y - b.radius <= 0) {
        b.speed_y *= -1;
    }

    if (b.x + b.radius >= GetScreenWidth()) {
        cpu_score++;
        ResetBall(b);
    }

    if (b.x - b.radius <= 0) {
        player_score++;
        ResetBall(b);
    }
}


void DrawPaddle(Paddle& p) {
    DrawRectangleRounded(Rectangle{ p.x, p.y, p.width, p.height }, 0.8, 0, WHITE);
}

void UpdatePlayerPaddle(Paddle& p) {
    if (IsKeyDown(KEY_UP)) {
        p.y -= p.speed;
    }
    if (IsKeyDown(KEY_DOWN)) {
        p.y += p.speed;
    }

    if (p.y <= 0) {
        p.y = 0;
    }
    if (p.y + p.height >= GetScreenHeight()) {
        p.y = GetScreenHeight() - p.height;
    }
}

void UpdateCpuPaddle(Paddle& cpu, float ball_y) {
    if (cpu.y + cpu.height / 2 > ball_y) {
        cpu.y -= cpu.speed;
    }
    if (cpu.y + cpu.height / 2 <= ball_y) {
        cpu.y += cpu.speed;
    }

    if (cpu.y <= 0) {
        cpu.y = 0;
    }
    if (cpu.y + cpu.height >= GetScreenHeight()) {
        cpu.y = GetScreenHeight() - cpu.height;
    }
}

int main() {
    std::cout << "Starting the game" << std::endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (!WindowShouldClose()) {
        BeginDrawing();

        // Updating
        UpdateBall(ball);
        UpdatePlayerPaddle(player);
        UpdateCpuPaddle(cpu, ball.y);

        // Checking for collisions
        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height })) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        DrawBall(ball);
        DrawPaddle(cpu);
        DrawPaddle(player);

        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
