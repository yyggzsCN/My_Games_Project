#include "raylib.h"
#include <iostream>
#include <deque>
#include "raymath.h"

using namespace std;

Color green = {173, 204, 96, 255};

const int cell_size = 30;
const int cell_count = 25;
double LastUpdateTime = 0;
const int offset = 75;

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
    for (unsigned int i = 0; i < deque.size(); ++i) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - LastUpdateTime >= interval) {
        LastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    // Use a loop to draw the snake

    
    void Draw() {
        for (unsigned int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cell_size,offset + y * cell_size, (float)cell_size, (float)cell_size};
            DrawRectangleRounded(segment, 0.5, 6, DARKGREEN);
        }
    }

    bool addSegment = false;
    void Upadate() {
        if (addSegment == true) {
            addSegment = false;
        } else {
            body.pop_back();
        }
        body.push_front(Vector2Add(body[0],direction));
        
    }

    void Reset() {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};


// Create a Food
class Food {

public:
    Vector2 position;
    Texture2D texture;
    // Constructor
    Food(deque<Vector2> snakebody) {
        Image image = LoadImage("Graphic/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakebody);
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, offset + position.x * cell_size, offset + position.y * cell_size, WHITE);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cell_count - 1);
        float y = GetRandomValue(0, cell_count - 1);
        return Vector2{x, y};
    }
    
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {

        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game  {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }
    ~Game(){
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        if (running) {
            snake.Upadate();
            CheckCollisionWithfood();
            CheckCcollisionWithEdge();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithfood() {
        if (Vector2Equals(snake.body[0],food.position)) {
            cout << "Eating food" << endl;
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCcollisionWithEdge() {
        if (snake.body[0].x == cell_count || snake.body[0].x == -1) {
            GameOver();
        }
        if (snake.body[0].y == cell_count || snake.body[0].y == -1) {
            GameOver();
        }

    }

    void CheckCollisionWithTail() {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)) {
            GameOver();
        }
    }

    void GameOver() { 
        cout << "Game Over !" << endl;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

int main() {
    cout << "Starting the game...." << endl;
    InitWindow(2 * offset + cell_size * cell_count,2 * offset + cell_size * cell_count, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        if (eventTriggered(0.2)) {
            game.Update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        } else if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        } else if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        } else if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // Drawing
        ClearBackground(green); 
        DrawRectangleLinesEx(Rectangle{(float)offset - 5,
        (float)offset - 5, (float)cell_size*cell_count + 10, (float)cell_size* cell_count + 10}, 5, DARKGREEN);
        DrawText("Retro Snake", offset - 5, 20, 40, DARKGREEN);
        DrawText(TextFormat("%i", game.score),  offset - 5, offset + cell_size*cell_count + 10, 49, DARKGREEN);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
