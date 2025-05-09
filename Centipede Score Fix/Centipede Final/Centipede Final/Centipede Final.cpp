// Centipede Final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
using namespace std;

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

using namespace std;

// Color colorname = { Red, Green, Blue, Alpha };
// ================= Closer to   0 = Black
// ================= Closer to 255 = White
Color backgroundColor = { 255, 255, 255, 255 };
Color snakeColor = { 0, 255, 0, 255 };
Color foodColor = { 255, 0, 0, 255 };


// used for resolution and playspace, change this if needed but note that this grid is important
int gridSize = 25;
int gridCount = 20;
int screen = 1;
//int lives = 3;
int score = 0;

// this is used to make sure the player isn't speed blitzing and is moving at a controlled speed
double lastUpdateTime = 0;

bool timedMovementTrigger(double interval) {

	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;

	}
	return false;
}


bool DequeElement(Vector2 element, deque<Vector2> deque) {
	for (unsigned int i = 0; i < deque.size(); i++) {
		if (Vector2Equals(deque[i], element)) {
			return true;
		}
	}
	return false;
}

// player class, this holds the information about the snake
class playerSnake {
public:
	deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
	Vector2 direction = { 1, 0 };

	bool incrementBody = false;

	void Draw() {
		for (unsigned int i = 0; i < body.size(); i++) {
			int x = body[i].x;
			int y = body[i].y;

			DrawRectangle(x * gridSize, y * gridSize, gridSize, gridSize, snakeColor);
		}
	}

	void Update() {

		body.push_front(Vector2Add(body[0], direction));

		if (incrementBody == true) {

			incrementBody = false;
		}
		else {
			body.pop_back();
		}
	}

	void Reset() {
		body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
		direction = { 1, 0 };
	}
};

// food class, this holds the information about the fruit
class Food {
public:
	Vector2 position;
	const char* filename = "Apple.png";
	
	Texture2D apple = LoadTexture(filename);

	Food(deque<Vector2> snakeBody) {
		position = GenRandomPos(snakeBody);
	}


	void Draw() {
		
		//Vector2 applePosition = (position.x * gridSize, position.y * gridSize);
		DrawRectangle(position.x * gridSize, position.y * gridSize, gridSize, gridSize, foodColor);

		//Rectangle frameRec = { position.x*gridSize,position.y*gridSize,gridSize,gridSize };

		//DrawTextureRec(apple, frameRec, position, WHITE);
	}

	Vector2 RanCell() {

		float x = GetRandomValue(0, gridCount - 1);
		float y = GetRandomValue(0, gridCount - 1);
		return Vector2{ x, y };
	}


	Vector2 GenRandomPos(deque<Vector2> snakeBody) {

		Vector2 postion = RanCell();

		while (DequeElement(position, snakeBody)) {

			position = RanCell();
		}
		return position;
	}

};


class Game {
public:
	//const char* filename = "Apple.png";
	//Texture2D apple = LoadTexture(filename);
	playerSnake Player = playerSnake();
	Food Fruit = Food(Player.body);
	bool isGameRunning = false;

	void Draw() {
		char buffer[5];
		Fruit.Draw();
		Player.Draw();
		DrawText("Score:",70, 370, 20, GREEN);
		DrawText(_itoa(score, buffer, 10), 70, 400, 20, GREEN);

		//DrawText("Lives:", 350, 370, 20, GREEN);
		//DrawText(_itoa(lives, buffer, 10), 350, 400, 20, GREEN);
	}

	void Update() {

		if (isGameRunning) {
			Player.Update();
			HasCollidedWithFood();
			HasCollidedWithWall();
			HasCollidedWithBody();
		}
	}

	void HasCollidedWithFood() {
		if (Vector2Equals(Player.body[0], Fruit.position)) {
			cout << "Collision";
			Fruit.position = Fruit.GenRandomPos(Player.body);
			Player.incrementBody = true;
			score += 10;
		}
	}

	void HasCollidedWithWall() {
		if (Player.body[0].x == gridCount || Player.body[0].x == -1) {
			GameOver();
		}
		if (Player.body[0].y == gridCount || Player.body[0].y == -1) {
			GameOver();
		}
	}

	void HasCollidedWithBody() {
		deque<Vector2> headlessBody = Player.body;
		headlessBody.pop_front();

		if (DequeElement(Player.body[0], headlessBody)) {
			GameOver();
		}

	}

	void GameOver() {

		Player.Reset();
		
		Fruit.position = Fruit.GenRandomPos(Player.body);
		isGameRunning = false;
		//lives -= 1;
		screen = 3;
		//if (lives <= 0) {
		//	screen = 3;
		//}

	}

};

// main game function
int main() {
	bool startGame = false;
	
	bool endGame = false;

	cout << "Starting game" << endl;
	InitWindow(gridSize * gridCount, gridSize * gridCount, "Centipede!");
	// force 60 fps render, game speed is tied to frame speed
	SetTargetFPS(60);

	Game centipedeGame = Game();

	while (WindowShouldClose() == false) {
		
		BeginDrawing();
		
		
		
		
		
		// Game state switch
			//cout << screen;
			switch (screen) {
			case 1:
				//Start screen, displays start button
				ClearBackground(backgroundColor);
				DrawText("Centipede!", 120, 120, 40, GREEN);
				//Start Button
				if (GuiButton(Rectangle{ 180, 400, 120, 30 }, "Start Game")) {
					centipedeGame.isGameRunning = true;
					cout << "button pressed";
					screen = 2;
					cout << screen;
				}
			break;

			case 2:
				// movement
				//cout << "Game started";
				centipedeGame.isGameRunning = true;
				if (timedMovementTrigger(0.2)) {
					centipedeGame.Update();
				}

				if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && centipedeGame.Player.direction.y != 1) {
					centipedeGame.Player.direction = { 0, -1 };

				}

				if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && centipedeGame.Player.direction.y != -1) {
					centipedeGame.Player.direction = { 0, 1 };

				}

				if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && centipedeGame.Player.direction.x != 1) {
					centipedeGame.Player.direction = { -1, 0 };

				}

				if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && centipedeGame.Player.direction.x != -1) {
					centipedeGame.Player.direction = { 1, 0 };

				}
				
				
				if (centipedeGame.isGameRunning == true) {
					if (GuiButton(Rectangle{ 180, 400, 120, 30 }, "Quit")) {
						screen = 3;
					}

				}



				ClearBackground(backgroundColor);

				// draw the frame
				centipedeGame.Draw();

			break;

			case 3:
				//game over screen, displays GAME OVER, plaer score and gives option to play again or exit
				ClearBackground(backgroundColor);
				DrawText("Game Over", 120, 120, 40, GREEN);
				char buffer[5];
				DrawText("Your score is:", 120, 170, 20, GREEN);
				DrawText(_itoa(score, buffer, 10), 120, 200, 20, GREEN);

				//Exit Button
				if (GuiButton(Rectangle{ 180, 370, 120, 30 }, "Exit")) {
					exit(0);
				}
				//PLay again button
				if (GuiButton(Rectangle{ 180, 400, 120, 30 }, "Play Again")) {
					score = 0;
					screen = 2;
				}
			break;
			
			}
		
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}