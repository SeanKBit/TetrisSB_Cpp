// Sean K. Bittner
// 12/12/2016
// Ver. 4.0
// Mod: 11/21/2017
//
// Console Tetris. 
//
// One minor bug when performing a "hard drop" that needs addressed- current program will allow a 4-char tall shape to pass into 
// shape or bucket below it if a hard drop is performed immediately before contacting below character. This is due to the hard drop
// being handled with a "+= 2" structure, and the movement checks not looking far enough ahead. 
// Proposed solution is to replace the "+= 2" increment with a temporary increase in "sleep" speed.
//
// Another bug found when rotating near side of bucket or shape. As shape rotates "into" bucket it disappears into sides.
//
// Mod11 version is updated based on peer review. And also some developer chosen updates. Update denoted with "MOD11 UPDATE"
// Removed uneccesary function, added level number and speed increase, added a Tetris! scoring check, added flashy titles

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Windows.h"
#include <ctime>
#include <thread>
#include <conio.h>
#include <fstream>
#include <iomanip>
#pragma comment(lib, "winmm.lib")


using namespace std;

// Globals and constants for 2-D bucket array
const int ROW = 25;
const int COL = 12;
const int SHAPEROW = 4;
const int SHAPECOL = 4;
char bucket[ROW][COL]; // Matrix (bucket) for gameplay
bool gameOver = false;
int input;
int score = 0;
int newScore = 0;
int speed = 550; // Initial speed of drops, used within sleep function
int level = 0;
string leader = "";
string recordScore;
string leaderScore;
time_t timer;
time_t timer2;
time_t timer3;
double seconds;
bool playing2 = false;
bool playing3 = false;


class TetrisShape
{
public:
	int shapeArray[SHAPEROW][SHAPECOL];
	int shapeTopLeftX;
	int shapeTopLeftY;
	TetrisShape(int type);
	TetrisShape();
	void setShape(TetrisShape& localTetrisShape);


private:
};

TetrisShape::TetrisShape(int type)
{
	shapeTopLeftX = 5; // Initiates starting coordinate for X
	shapeTopLeftY = 0; // Initiates starting coordinate for Y
	switch (type)
	{
	case 1: // Creates '|' type of shape down the 2nd column of shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = 'X'; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = 'X'; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = ' '; shapeArray[3][1] = 'X'; shapeArray[3][2] = ' '; shapeArray[3][3] = ' ';
		break;
	case 2: // Creates 'L' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = 'X'; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = ' '; shapeArray[3][1] = 'X'; shapeArray[3][2] = 'X'; shapeArray[3][3] = ' ';
		break;
	case 3: // Creates 'O' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = 'X'; shapeArray[2][3] = ' ';
		shapeArray[3][0] = ' '; shapeArray[3][1] = 'X'; shapeArray[3][2] = 'X'; shapeArray[3][3] = ' ';
		break;
	case 4: // Creates 'S' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = 'X'; shapeArray[2][3] = ' ';
		shapeArray[3][0] = 'X'; shapeArray[3][1] = 'X'; shapeArray[3][2] = ' '; shapeArray[3][3] = ' ';
		break;
	case 5: // Creates 'T' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = 'X'; shapeArray[2][1] = 'X'; shapeArray[2][2] = 'X'; shapeArray[2][3] = ' ';
		shapeArray[3][0] = ' '; shapeArray[3][1] = 'X'; shapeArray[3][2] = ' '; shapeArray[3][3] = ' ';
		break;
	case 6: // Creates FLAT '|' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = ' '; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = 'X'; shapeArray[3][1] = 'X'; shapeArray[3][2] = 'X'; shapeArray[3][3] = 'X';
		break;
	case 7: // Creates 'J' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = 'X'; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = 'X'; shapeArray[3][1] = 'X'; shapeArray[3][2] = ' '; shapeArray[3][3] = ' ';
		break;
	case 8: // Create 'Z' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = 'X'; shapeArray[2][1] = 'X'; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = ' '; shapeArray[3][1] = 'X'; shapeArray[3][2] = 'X'; shapeArray[3][3] = ' ';
		break;
	case 9: // Create upside down 'T' shape
		shapeArray[0][0] = ' '; shapeArray[0][1] = ' '; shapeArray[0][2] = ' '; shapeArray[0][3] = ' ';
		shapeArray[1][0] = ' '; shapeArray[1][1] = ' '; shapeArray[1][2] = ' '; shapeArray[1][3] = ' ';
		shapeArray[2][0] = ' '; shapeArray[2][1] = 'X'; shapeArray[2][2] = ' '; shapeArray[2][3] = ' ';
		shapeArray[3][0] = 'X'; shapeArray[3][1] = 'X'; shapeArray[3][2] = 'X'; shapeArray[3][3] = ' ';
		break;
	}
}

TetrisShape::TetrisShape()
{
	cout << "\nERROR: Shape type not initialized." << endl;
}

// Member function to create a duplicate shape that will be used to store in bucket array without moving
void TetrisShape::setShape(TetrisShape& localTetrisShape)
{
	for (int i = 0; i < SHAPEROW; i++)
	{
		for (int j = 0; j < SHAPECOL; j++)
		{
			shapeArray[i][j] = localTetrisShape.shapeArray[i][j];
		}
	}
}

// Function declarations
void displayTitle();
void initializeBucket(char(&bucket)[ROW][COL]);
void displayBucket(char(&bucket)[ROW][COL]);
void updateBucket(TetrisShape& localTetrisShape);
void clearTrail(TetrisShape& localTetrisShape);
void setCursorTo(int x, int y);
int getUserInput();
void moveShape(int LocalInput, TetrisShape& localTetrisShape, TetrisShape& nextTetrisShape);
int getRandom(int high, int low);
int calcScore(char(&bucket)[ROW][COL]);
void dropRow(char(&bucket)[ROW][COL], int rowNum);
string getHighScore();
string getLastDigits(string checkLeader);
void newHighScore(string localLeaderScore);

int main()
{
	srand(time(NULL)); // Seeds randomInt
	const int RANDMIN = 1;
	const int RANDMAX = 9;
	int shapeType;
	time(&timer);

	if (!PlaySound(TEXT("C:/TetrisSB/Osc_gangster.wav"), NULL, SND_ASYNC))
	{
		setCursorTo(29, 1); cout << "Sound file not found!"; Sleep(3000);
		setCursorTo(29, 1); cout << "                                     ";
	}
	Sleep(200);

	displayTitle();
	initializeBucket(bucket); // Calls to start building bucket
	displayBucket(bucket);

	// Start of game, loops while playing each level
	while (!gameOver)
	{
		shapeType = getRandom(RANDMAX, RANDMIN);
		TetrisShape localTetrisShape(shapeType);
		TetrisShape nextTetrisShape(shapeType);

		moveShape(input, localTetrisShape, nextTetrisShape);
	}

	system("pause");
	return 0;
}

// Title display. MOD11 UPDATE (more flashy)
void displayTitle()
{
	for (int i = 0; i < 5; i++)
	{
		setCursorTo(29, 1); cout << "                                     ";
		setCursorTo(29, 2); cout << "                                     ";
		setCursorTo(29, 3); cout << "                                     ";
		setCursorTo(29, 4); cout << "                                     ";
		Sleep(500);
		setCursorTo(29, 1); cout << "XXX    XXX    XXX    XXX    X    XXX";
		setCursorTo(29, 2); cout << " X     X       X     X X    X    X";
		setCursorTo(29, 3); cout << " X     XX      X     XX     X     X";
		setCursorTo(29, 4); cout << " X     XXX     X     X X    X    XXX";
		Sleep(500);
	}
}

// New function to initialize and display the bucket 
void initializeBucket(char(&bucket)[ROW][COL])
{
	for (int i = 0; i < ROW; i++) // Begin iteration through 2D array
	{
		for (int j = 0; j < COL; j++)
		{
			if (i == ROW - 1) // Check to see if last row, if so fills entire row with char.
			{
				for (int b = 0; b < COL; b++)
				{
					bucket[i][b] = '+';
				}
			}
			else if (j < 1 || j == COL - 1) // Check for 1st and last column to fill with char
			{
				bucket[i][j] = '+';
			}
			else // Fills remaining cells with empty char.
			{
				bucket[i][j] = ' ';
			}
		}
	}
}

// Used to display bucket, calls setCursorTo function
void displayBucket(char(&bucket)[ROW][COL])
{
	int posX, posY;
	posX = 40;
	posY = 6;

	for (int i = 0; i < ROW; i++)
	{
		setCursorTo(posX, posY);
		for (int j = 0; j < COL; j++)
		{
			cout << bucket[i][j];
		}
		posY++;
	}
}

// This function handles imposing of shape over the "bucket" array. 
void updateBucket(TetrisShape& thisLocalTetrisShape)
{
	for (int i = 0; i < SHAPEROW; i++)
	{
		for (int j = 0; j < SHAPECOL; j++)
		{
			if (thisLocalTetrisShape.shapeArray[i][j] == 'X' && bucket[thisLocalTetrisShape.shapeTopLeftY + i][thisLocalTetrisShape.shapeTopLeftX + j] != '+')
			{
				bucket[thisLocalTetrisShape.shapeTopLeftY + i][thisLocalTetrisShape.shapeTopLeftX + j] = thisLocalTetrisShape.shapeArray[i][j];
			}
		}
	}
}

void clearTrail(TetrisShape& localTetrisShape)
{
	for (int i = 0; i < SHAPEROW; i++) // These nested for loops are for clearing the "trail" of the shape as it moves
	{
		for (int j = 0; j < SHAPECOL; j++)
		{
			if (localTetrisShape.shapeArray[i][j] == 'X' && bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j] != '+')
			{
				bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j] = ' ';
			}
		}
	}
}

// Used to set cursor at appropriate position while displaying the bucket
void setCursorTo(int x, int y)
{
	HANDLE handle;
	COORD position;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(handle, position);
}

void moveShape(int localInput, TetrisShape& localTetrisShape, TetrisShape& nextTetrisShape)
{
	bool localStuck = false;
	score += calcScore(bucket);
	level++;
	setCursorTo(15, 6);
	cout << "SCORE: " << score * 10; // Outputs the current score at set location. Multiplied by 10 simply to make it more exciting
	setCursorTo(15, 7);
	cout << "LEVEL: " << level << endl; // MOD11 Update
	leader = getHighScore();
	setCursorTo(15, 9);
	cout << "HIGH SCORE: " << leader << endl;

	time(&timer2);
	time(&timer3);
	seconds = difftime(timer2, timer);
	setCursorTo(15, 12);

	if (!playing2 && seconds > 224) 
	{
		PlaySound(TEXT("C:/TetrisSB/180db.wav"), NULL, SND_ASYNC);
		playing2 = true;
	}
	if (!playing3 && seconds > 422)
	{
		PlaySound(TEXT("C:/TetrisSB/Untitled_1.wav"), NULL, SND_ASYNC);
		playing3 = true;
	}

	//MOD11 UPDATE
	if (level % 10 == 0 && level < 40) // Check to see if 10 additional levels have been completed, then increases speed a bit more drastically
	{
		speed -= 50;
	}
	else if (level % 10 == 0 && level >= 40) // If level is >= 50, speed increases slightly less drastically because the levels are increasing faster
	{
		speed -= 35;
	}

	while (!localStuck)
	{
		updateBucket(localTetrisShape); // Updates bucket with the current moving shape imposed into it
		displayBucket(bucket); // Displays bucket with now included shape
		Sleep(speed);
		clearTrail(localTetrisShape);

		// Loops for iterating through array to perform "stuck" checks on shape
		for (int y = 0; y < SHAPEROW; y++)
		{
			for (int x = 0; x < SHAPECOL; x++)
			{
				if (localTetrisShape.shapeArray[y][x] != ' ')
				{
					if (bucket[localTetrisShape.shapeTopLeftY + y + 1][localTetrisShape.shapeTopLeftX + x] == 'X') // Checks "ahead" to see if another shapes' remains are below
					{
						nextTetrisShape.setShape(localTetrisShape); // Creates duplicate shape
						nextTetrisShape.shapeTopLeftY = localTetrisShape.shapeTopLeftY; // Sets current x,y locations of shape to new shape
						nextTetrisShape.shapeTopLeftX = localTetrisShape.shapeTopLeftX;
						updateBucket(nextTetrisShape); // Imposes the new shape into bucket to "stack" within bucket (won't drop or move)
						displayBucket(bucket);
						localStuck = true;

						if (localTetrisShape.shapeTopLeftY == 0) // Ends game if shape backs-up to the top of bucket
						{
							nextTetrisShape.setShape(localTetrisShape); // Creates duplicate shape
							nextTetrisShape.shapeTopLeftY = localTetrisShape.shapeTopLeftY; // Sets current x,y locations of shape to new shape
							nextTetrisShape.shapeTopLeftX = localTetrisShape.shapeTopLeftX;
							updateBucket(nextTetrisShape); // Imposes the new shape into bucket to "stack" within bucket (won't drop or move)
							displayBucket(bucket);

							recordScore = getLastDigits(leader);
							newScore = score * 10;
							leaderScore = to_string(newScore);

							if (stoi(leaderScore) > stoi(recordScore)) // Checks for new high score and then calls that function
							{
								PlaySound(TEXT("C:/TetrisSB/HighScore.wav"), NULL, SND_ASYNC);
								for (int t = 0; t < 3; t++)
								{
									setCursorTo(60, 6); cout << "==============================" << endl; Sleep(300);
									setCursorTo(60, 8); cout << "!!!!!!! NEW HIGH SCORE !!!!!!!" << endl;  Sleep(300);
									setCursorTo(60, 10); cout << "==============================" << endl; Sleep(1000);

									setCursorTo(60, 6); cout << "                              " << endl; Sleep(400);
									setCursorTo(60, 8); cout << "                              " << endl;  Sleep(400);
									setCursorTo(60, 10); cout << "                              " << endl; Sleep(400);
								}
								newHighScore(leaderScore);
							}

							// Game Over! 
							PlaySound(TEXT("C:/TetrisSB/End.wav"), NULL, SND_ASYNC);
							setCursorTo(60, 6); cout << "==============================" << endl; Sleep(400);
							setCursorTo(60, 8); cout << "  XXX    X     X X   XXX   +" << endl;  Sleep(400);
							setCursorTo(60, 9); cout << "  X      X     X X   X     +" << endl;  Sleep(400);
							setCursorTo(60, 10); cout << "  XXX  X   X  X X X  XX    +" << endl;  Sleep(400);
							setCursorTo(60, 11); cout << "  XXX  X   X  X   X  XXX   +" << endl;  Sleep(400);
							setCursorTo(60, 12); cout << "                           +" << endl;  Sleep(400);
							setCursorTo(60, 13); cout << "  XXX   X X   XXX   XXX    +" << endl;  Sleep(400);
							setCursorTo(60, 14); cout << "  X X   X X   X     X X    +" << endl;  Sleep(400);
							setCursorTo(60, 15); cout << "  X X    X    XX    XX     +" << endl;  Sleep(400);
							setCursorTo(60, 16); cout << "  XXX    X    XXX   X X    @" << endl;  Sleep(400);
							setCursorTo(60, 18); cout << "==============================" << endl;

							localStuck = true;
							gameOver = true;
						}
					}
					else if (bucket[localTetrisShape.shapeTopLeftY + y + 1][localTetrisShape.shapeTopLeftX + x] == '+') // Checks to see if shape has reached bottom of bucket
					{
						nextTetrisShape.setShape(localTetrisShape);
						nextTetrisShape.shapeTopLeftY = localTetrisShape.shapeTopLeftY;
						nextTetrisShape.shapeTopLeftX = localTetrisShape.shapeTopLeftX;
						updateBucket(nextTetrisShape);
						displayBucket(bucket);
						localStuck = true;
					}
				}
			}
		}

		bool dropCheck = false;
		char tempCellVal;
		int checkCount2 = 0;
		input = getUserInput();
		if (!localStuck)
		{
			switch (input) {
			case 1: // Case 1 rotates shape. BUG here! Shape will get "stuck" in wall if it rotates into wall
				tempCellVal = localTetrisShape.shapeArray[0][0]; localTetrisShape.shapeArray[0][0] = localTetrisShape.shapeArray[0][3];
				localTetrisShape.shapeArray[0][3] = localTetrisShape.shapeArray[3][3]; localTetrisShape.shapeArray[3][3] = localTetrisShape.shapeArray[3][0];
				localTetrisShape.shapeArray[3][0] = tempCellVal;

				tempCellVal = localTetrisShape.shapeArray[0][1]; localTetrisShape.shapeArray[0][1] = localTetrisShape.shapeArray[1][3];
				localTetrisShape.shapeArray[1][3] = localTetrisShape.shapeArray[3][2]; localTetrisShape.shapeArray[3][2] = localTetrisShape.shapeArray[2][0];
				localTetrisShape.shapeArray[2][0] = tempCellVal;

				tempCellVal = localTetrisShape.shapeArray[0][2]; localTetrisShape.shapeArray[0][2] = localTetrisShape.shapeArray[2][3];
				localTetrisShape.shapeArray[2][3] = localTetrisShape.shapeArray[3][1]; localTetrisShape.shapeArray[3][1] = localTetrisShape.shapeArray[1][0];
				localTetrisShape.shapeArray[1][0] = tempCellVal;

				tempCellVal = localTetrisShape.shapeArray[1][1]; localTetrisShape.shapeArray[1][1] = localTetrisShape.shapeArray[1][2];
				localTetrisShape.shapeArray[1][2] = localTetrisShape.shapeArray[2][2]; localTetrisShape.shapeArray[2][2] = localTetrisShape.shapeArray[2][1];
				localTetrisShape.shapeArray[2][1] = tempCellVal;
				input = 0;
				break;

			case 2: // Case 2 moves shape down faster, and checks to make sure it's safe to do so. "Hard Drop" BUG! (fixed sort of)
				for (int y = 0; y < SHAPEROW; y++)
				{
					for (int x = 0; x < SHAPECOL; x++)
					{
						if (localTetrisShape.shapeArray[y][x] != ' ')
						{
							if (bucket[localTetrisShape.shapeTopLeftY + y + 4][localTetrisShape.shapeTopLeftX + x] == 'X' ||
								bucket[localTetrisShape.shapeTopLeftY + y + 4][localTetrisShape.shapeTopLeftX + x] == '+' ||
								bucket[localTetrisShape.shapeTopLeftY + y + 5][localTetrisShape.shapeTopLeftX + x] == 'X' ||
								bucket[localTetrisShape.shapeTopLeftY + y + 5][localTetrisShape.shapeTopLeftX + x] == '+' ||
								bucket[localTetrisShape.shapeTopLeftY + y + 6][localTetrisShape.shapeTopLeftX + x] == 'X' ||
								bucket[localTetrisShape.shapeTopLeftY + y + 6][localTetrisShape.shapeTopLeftX + x] == '+') // These are looking ahead for other characters. Prevents running into chars.
							{
								dropCheck = true;
								break;
							}
						}
					}
				}
				if (!dropCheck)
				{
					localTetrisShape.shapeTopLeftY += 2;
				}
				dropCheck = false;
				input = 0;
				break;


			case 3: // Case 3 moves shape left, and checks to make sure it is safe to do so
				for (int i = 0; i < SHAPEROW; i++)
				{
					for (int j = 0; j < SHAPECOL; j++)
					{
						if (localTetrisShape.shapeArray[i][j] != ' ')
						{
							if (bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j - 1] != '+' &&
								bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j - 1] != 'X')
							{
								checkCount2++;
							}
						}
					}
				}
				if (checkCount2 == 4)
				{
					localTetrisShape.shapeTopLeftX--;
				}
				input = 0;
				checkCount2 = 0;
				break;

			case 4: // Case 4 moves shape right, and checks to make sure it is safe to do so
				for (int i = 0; i < SHAPEROW; i++)
				{
					for (int j = 3; j >= 0; j--)
					{
						if (localTetrisShape.shapeArray[i][j] != ' ')
						{
							if (bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j + 1] != '+' &&
								bucket[localTetrisShape.shapeTopLeftY + i][localTetrisShape.shapeTopLeftX + j + 1] != 'X')
							{
								checkCount2++;
							}
						}
					}
				}
				if (checkCount2 == 4)
				{
					localTetrisShape.shapeTopLeftX++;
				}
				input = 0;
				checkCount2 = 0;
				break;

			default:
				localTetrisShape.shapeTopLeftY++; // Increments the 'Y' location of the top left corner of shape to move the shape with each pass
				break;
			}
		}
	}
}

int getUserInput()
{

	int localInput = 0;
	enum
	{
		UP_ARROW = 72, DOWN_ARROW = 80, LEFT_ARROW = 75, RIGHT_ARROW = 77
	};
	if (_kbhit())
	{
		localInput = _getch();
	}

	if (localInput == UP_ARROW)
	{
		return 1;
	}
	if (localInput == DOWN_ARROW)
	{
		return 2;
	}
	if (localInput == LEFT_ARROW)
	{
		return 3;
	}
	if (localInput == RIGHT_ARROW)
	{
		return 4;
	}

}

// Random integer generator
int getRandom(int high, int low)
{
	int rando;
	rando = (rand() % high) + low;

	return rando;
}

// Checks if row is filled and scores accordingly, then clears row
int calcScore(char(&bucket)[ROW][COL])
{
	int tempScore = 0;
	int fillRowCounter;
	int tetrisCheck = 0; // Used as a counter to see if 4 rows were cleared in one scoring (Tetris!) MOD11 UPDATE

						 // For loops check to see if row is filled with 10 X's
	for (int i = 0; i < ROW; i++)
	{
		fillRowCounter = 0; // fillRowCounter resets at each row
		for (int j = 0; j < COL; j++)
		{
			if (bucket[i][j] == 'X')
			{
				fillRowCounter++; // Increments counter if 'X' is found
				if (fillRowCounter == 10) // If 10 X's are found, adds to score 
				{
					tempScore++;
					tetrisCheck++; //MOD11 UPDATE
					//PlaySound(TEXT("C:/TetrisSB/Score.wav"), NULL, SND_ASYNC);
					for (int k = 10; k > 0; k--) // Works from the end of current row and icrements backwards while clearing X's
					{
						bucket[i][k] = ' ';
					}
					dropRow(bucket, i); // Passes the current row number to the dropRow function
				}
			}
		}
	}
	// Checks to see if a Tetris has been scored, and if so, add 10 bonus points. MOD11 UPDATE
	if (tetrisCheck >= 4)
	{
		for (int i = 0; i < 5; i++) // Loop for flashy Tetris
		{
			setCursorTo(13, 9);
			cout << "                 ";Sleep(400);
			setCursorTo(13, 9);
			cout << "**TETRIS BONUS!** +10"; Sleep(400);
		}
		setCursorTo(13, 9);
		cout << "                      "; // Used to clear "Tetris Bonus" for next round
		tempScore++;
	}
	return tempScore;
}

// dropRow called from within calcScrore
// Function to drop all above shapes down one everytime a row is cleared. Uses the current row number from the calcScore
// function to only iterate until that row is reached.
void dropRow(char(&bucket)[ROW][COL], int rowNum)
{
	for (rowNum - 1; rowNum > 0; rowNum--) // Loop iterates "backwards" from the row that was just cleared up to the top of bucket
	{
		for (int j = 0; j < COL; j++)
		{
			if (bucket[rowNum][j] == 'X')
			{
				bucket[rowNum + 1][j] = 'X'; // This "drops" each X char down one position
				bucket[rowNum][j] = ' ';
				displayBucket(bucket);
			}
		}
	}
}

string getHighScore()
{
	ifstream recordFile;
	string line;
	string tempLeader;
	recordFile.open("C:/TetrisSB/TetrisSB_Records.txt");

	if (recordFile.is_open())
	{
		while (getline(recordFile, line))
		{
			tempLeader = line;
		}
		recordFile.close();
	}
	else
	{
		tempLeader = "0";
	}

	return tempLeader;
}

string getLastDigits(string checkLeader)
{
	string tempLeaderScore;
	int i = 0;

	while (checkLeader[i] != ' ')
	{
		i++;
	}
	i++;
	while (i != checkLeader.length())
	{
		tempLeaderScore += checkLeader[i];
		i++;
	}

	return tempLeaderScore;
}

void newHighScore(string localLeaderScore)
{
	string initials;
	bool lengthCheck = false;

	while (!lengthCheck)
	{
		setCursorTo(60, 10); cout << "ENTER YOUR INITIALS: _ _ _" << endl;
		setCursorTo(81, 10); cin >> initials;
		if (initials.length() <= 3)
		{
			lengthCheck = true;
		}
		else
		{
			setCursorTo(60, 12); cout << "3 DIGITS ONLY!" << endl; Sleep(5000);
			setCursorTo(60, 10); cout << "                             " << endl;
			lengthCheck = false;
		}
	}

	ofstream recordFile;
	recordFile.open("C:/TetrisSB/TetrisSB_Records.txt", ios::trunc);
	recordFile << initials << " " << localLeaderScore;
	recordFile.close();
}





