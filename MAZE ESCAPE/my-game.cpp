// Game Name: Maze Escape
// Written by: Josh Thomas
// Date: December 6, 2022
// 
// Instructions:
// 
// -You (BLUE) are being chased by a splitting monster (BLACK)
// -The monster's radioactive trail (RED) will kill you
// -You must run through a maze to find the goal (YELLOW)
// -Reaching the goal will save you from the monster by
// transporting you to a new maze with another monster
// -Complete all 7 mazes to escape from the monster entirely
// -W = Move UP | A = Move LEFT | S = Move DOWN | D = Move RIGHT
// -Pressing [Q] or [ESC] will end the level and your progress

#include "graphics.h"
#include <string>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;
//s
const int MAX_X = 51;
const int MAX_Y = 51;
void drawBox(int x, int y, int color, int drawNumber = -1);
int minSizeNonZero(int, int, int, int);
enum MazeContent { EMPTY, WALL, EXIT };
//e
const int BACKGROUND_COLOR = BLUE;
const int TEXT_COLOR = LIGHTGRAY;

random_device myEngine;
uniform_real_distribution<double> randomReal(0.0, 1.0);

void welcomeScreen();

int main()
{
	static int WALL_COLOR = WHITE;
	static int EMPTY_COLOR = GREEN;
	static int EXIT_COLOR = YELLOW;
	static int PLAYER_COLOR = RED;
	static int FILL_COLOR = MAGENTA;
	static int PATH_COLOR = BLACK;
	static int MONSTER_COLOR = BLACK;
	ifstream inputFile;

	int numColumns, numRows;
	int x, y;
	char nextChar;
	char keyPressed;
	//
	const int EXIT_NUMBER = -99;

	const int QUEUE_SIZE = 500;

	struct MazeLocation {
		int x;
		int y;
	};

	MazeLocation player = { 1, 1 };
	MazeLocation playerPrev = { 1, 1 };
	MazeLocation monster = { 20, 20 };
	MazeLocation monsterPrev = { 20, 20 };
	int playerX = 1;
	int playerY = 1;
	int playerPrevX = 1;
	int playerPrevY = 1;
	int monsterX = 20;
	int monsterY = 20;
	int monsterPrevX = 20;
	int monsterPrevY = 20;
	int level = 1;
	//int monster = 1;


	bool doBreadthAuto = false;

	//
	int foundExitX = 0;
	int foundExitY = 0;
	bool showSolution = false;

	// A queue of positions, using parallel arrays
	


	// The big array that holds the maze
	int maze[MAX_X][MAX_Y];
	int currentX = 0, currentY = 0;
	bool somethingMoved = true;
	//char keyPressed;
	initwindow(930, 800, (char*)"MAZE ESCAPE", 10, 10);

	welcomeScreen();
	bool playAgain = true;
	bool keepGoing = true;
	//
	// -------- Main "Play Again" loop
	//
	do {
		cleardevice();
		int queueX[QUEUE_SIZE];
		int queueY[QUEUE_SIZE];
		int queueFront = -1;
		int queueBack = 0;
		int prevStep = 1;
		int currentStep = 1;
		int startingStep = 1;
	
		//
		if (level == 1) {
			inputFile.open("maze1.txt");
		}
		else if (level == 2) {
			inputFile.open("maze2.txt");
		}
		else if (level == 3) {
			inputFile.open("maze3.txt");
		}
		else if (level == 4) {
			inputFile.open("maze4.txt");
		}
		else if (level == 5) {
			inputFile.open("maze5.txt");
		}
		else if (level == 6) {
			inputFile.open("maze6.txt");
		}
		else if (level == 7) {
			inputFile.open("maze7.txt");
		}
		else inputFile.open("maze8.txt");
		

		inputFile >> numColumns >> numRows;

		// Read the maze, store values in array
		//    0 = empty space
		//   -2 = wall
		//  -99 = Exit
		// Then and draw the maze
		for (y = 0; y < numRows; y++)
		{
			for (x = 0; x < numColumns; x++)
			{
				inputFile >> nextChar;
				if (nextChar == 'X' || nextChar == 'x') {
					maze[x][y] = -2;
					drawBox(x, y, WALL_COLOR);
				}
				else if (nextChar == 'E' || nextChar == 'e')
				{
					maze[x][y] = -3;
					drawBox(x, y, EXIT_COLOR);
				}
				else if (nextChar == 'P' || nextChar == 'p') {
					player.x = x;
					player.y = y;
					playerPrev.x = x;
					playerPrev.y = y;
					maze[x][y] = PLAYER_COLOR;
					drawBox(x, y, PLAYER_COLOR);
				}
				else if (nextChar == 'M' || nextChar == 'm') {
					monster.x = x;
					monster.y = y;
					monsterPrev.x = x;
					monsterPrev.y = y;
					maze[x][y] = MONSTER_COLOR;
					drawBox(x, y, MONSTER_COLOR);
				}
				else {
					maze[x][y] = 0;
					drawBox(x, y, EMPTY_COLOR);
				}
			}
		}
		// Close the file.
		inputFile.close();

		outtextxy(20, 670, (char*)"GO GO GO!"); 
		                                       
		// 
		// -------- Main "Keep Going" loop
		// 

		do {  
			if (kbhit()) {
				keyPressed = getch();
				MazeLocation attempt;
				// Q or ESC pressed
				if (keyPressed == 'q' || keyPressed == 'Q' || keyPressed == 0x1b) {
					keepGoing = false;
				}

				if (keyPressed == 'W' || keyPressed == 'w' || keyPressed == 'A' || keyPressed == 'a'
					|| keyPressed == 'S' || keyPressed == 's' || keyPressed == 'D' || keyPressed == 'd') {
					// Attempt to move player
					attempt = player; // Set attempt to current location

					if (keyPressed == 'A' || keyPressed == 'a') {
						attempt.x--; // Left
					}
					if (keyPressed == 'D' || keyPressed == 'd') {
						attempt.x++; // Right
					}
					if (keyPressed == 'W' || keyPressed == 'w') {
						attempt.y--; // Up
					}
					if (keyPressed == 'S' || keyPressed == 's') {
						attempt.y++; // Down
					}

					// If that location is empty, go there
					if (maze[attempt.x][attempt.y] != -2) {
						// Remember current position
						playerPrev.x = player.x;
						playerPrev.y = player.y;

						player = attempt;
						somethingMoved = true;
					}
					if (somethingMoved) {
						drawBox(playerPrev.x, playerPrev.y, EMPTY_COLOR);
						drawBox(player.x, player.y, PLAYER_COLOR);
					}
				}
				for (int i = 0; i < 5; i++) {
					delay(5);
					if (queueFront == -1) {
						// If we're just starting, start at monster x, y
						queueFront = 0;
						queueBack = 0;
						currentX = monster.x;
						currentY = monster.y;
						maze[currentX][currentY] = currentStep;
						drawBox(currentX, currentY, MONSTER_COLOR, currentStep);
					}
					else {
						// Otherwise, use the front of the queue
						currentX = queueX[queueFront];
						currentY = queueY[queueFront];
						startingStep = maze[currentX][currentY];
						currentStep = maze[currentX][currentY];
						queueFront = (queueFront + 1) % QUEUE_SIZE;
						drawBox(currentX, currentY, FILL_COLOR, startingStep);
						//cout << "From Front: " << currentX << ", " << currentY << ", queue size = " << queueBack - queueFront << endl;
					}

					// Check all directions, and put them into the queue
					if (maze[currentX + 1][currentY] < 1) {
						
						 if (maze[currentX + 1][currentY] != -2) { //
							queueX[queueBack] = currentX + 1;
							queueY[queueBack] = currentY;
							queueBack = (queueBack + 1) % QUEUE_SIZE;
							maze[currentX][currentY] = prevStep;
							maze[currentX + 1][currentY] = currentStep + 1;
							drawBox(currentX + 1, currentY, PATH_COLOR, currentStep + 1);
						}
					}
					if (maze[currentX - 1][currentY] < 1) {
						
					 if (maze[currentX - 1][currentY] != -2) {  //
							queueX[queueBack] = currentX - 1;
							queueY[queueBack] = currentY;
							queueBack = (queueBack + 1) % QUEUE_SIZE;
							maze[currentX][currentY] = prevStep;
							maze[currentX - 1][currentY] = currentStep + 1;
							drawBox(currentX - 1, currentY, PATH_COLOR, currentStep + 1);
						}
					}
					if (maze[currentX][currentY + 1] < 1) {
				
						 if (maze[currentX][currentY + 1] != -2) {  //
							queueX[queueBack] = currentX;
							queueY[queueBack] = currentY + 1;
							queueBack = (queueBack + 1) % QUEUE_SIZE;
							maze[currentX][currentY] = prevStep;
							maze[currentX][currentY + 1] = currentStep + 1;
							drawBox(currentX, currentY + 1, PATH_COLOR, currentStep + 1);
						}
					}
					if (maze[currentX][currentY - 1] < 1) {
						
						 if (maze[currentX][currentY - 1] != -2) {  //
							queueX[queueBack] = currentX;
							queueY[queueBack] = currentY - 1;
							queueBack = (queueBack + 1) % QUEUE_SIZE;
							maze[currentX][currentY] = prevStep;
							maze[currentX][currentY - 1] = currentStep + 1;
							drawBox(currentX, currentY - 1, PATH_COLOR, currentStep + 1);
						}
					}
				}

			} // end while(keepGoing)



		} while ((maze[player.x][player.y] != -3) && (maze[player.x][player.y] != prevStep) && keepGoing == true);// Game has ended

		if (maze[player.x][player.y] == -3 && level == 7) {
			cleardevice();
			settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
			outtextxy(10, 350, (char*)"CONGRATULATIONS!!!");
			settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
			outtextxy(10, 400, (char*)"YOU ESCAPED THE MONSTER");
			settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
			outtextxy(10, 450, (char*)"Game Created By: Josh Thomas");
			delay(5000);
			level = 1;
			welcomeScreen();
		}

		else if (maze[player.x][player.y] == -3) {
			outtextxy(10, 670, (char*)"YOU MADE IT! NEXT LEVEL...");
		    delay(3000);
			level++;
		}

		else {
			outtextxy(10, 10, (char*)"GAME OVER, Play Again? (Y/N)");
			delay(1000);
			keyPressed = getch();
			if (keyPressed == 'y' || keyPressed == 'Y') {
				level = 1;
				keepGoing = true;
				welcomeScreen();
			}
			else {
				playAgain = false;
			}
		}
	} while (playAgain == true);  // Main "Play Again" loop

	return 0; }	 // end main()


void welcomeScreen() {
	
	setbkcolor(BACKGROUND_COLOR);
	cleardevice();

	setcolor(TEXT_COLOR);
	settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
	
	outtextxy(10, 10, (char*)"It's DO or DIE, in MAZE ESCAPE!");
	outtextxy(10, 100, (char*)"RULES:");
	settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
	outtextxy(10, 150, (char*)"-You (RED) are being chased by a splitting monster (BLACK)");
	outtextxy(10, 200, (char*)"-The monster's radioactive trail (PURPLE) will kill you");
	outtextxy(10, 250, (char*)"-You must run through a maze to find the goal (YELLOW)");
	outtextxy(10, 300, (char*)"-Reaching the goal will save you from the monster by");
	outtextxy(10, 350, (char*)"transporting you to a new maze with another monster");
	outtextxy(10, 400, (char*)"-Complete all 7 mazes to escape from the monster entirely");
	settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
	outtextxy(10, 500, (char*)"CONTROLS:");
	settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
	outtextxy(10, 550, (char*)"W = Move UP | A = Move LEFT | S = Move DOWN | D = Move RIGHT");
	outtextxy(10, 600, (char*)"Pressing [Q] or [ESC] will end the level and your progress");
	settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
	outtextxy(10, 700, (char*) "Press any key to begin");

	getch();
	cleardevice();
}
void drawBox(int mazeX, int mazeY, int color, int drawNumber) {

	int screenX;
	int screenY;

	const int BOX_SIZE = 22;

	screenX = mazeX * BOX_SIZE + 20;
	screenY = mazeY * BOX_SIZE + 20;

	setfillstyle(SOLID_FILL, color);

	//Draw box, outline, and text number
	bar(screenX, screenY, screenX + BOX_SIZE, screenY + BOX_SIZE);

	return;
}