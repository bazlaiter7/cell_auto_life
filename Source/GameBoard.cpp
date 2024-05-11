#include "GameBoard.h"
#include "string"
#include <iostream>
#include <audiere/audiere.h>


int GameBoard::WrappingModulo(int pNum, int pMod)
{
	int answer = pNum % pMod;
	if (answer < 0)
		answer += pMod;
	return answer;
}

int GameBoard::SurroundingCells(int pCol, int pRow)
{
	int enabledSurroundingCells = 0;

	//Top of cell
	enabledSurroundingCells += mBoard[WrappingModulo(pCol - 1, mBoardColumns)][WrappingModulo(pRow - 1,  mBoardRows)];
	enabledSurroundingCells += mBoard[WrappingModulo(pCol    , mBoardColumns)][WrappingModulo(pRow - 1, mBoardRows)];
	enabledSurroundingCells += mBoard[WrappingModulo(pCol + 1, mBoardColumns)][WrappingModulo(pRow - 1, mBoardRows)];

	//Bottom of cell
	enabledSurroundingCells += mBoard[WrappingModulo(pCol - 1, mBoardColumns)][WrappingModulo(pRow + 1, mBoardRows)];
	enabledSurroundingCells += mBoard[WrappingModulo(pCol, mBoardColumns)][WrappingModulo(pRow + 1, mBoardRows)];
	enabledSurroundingCells += mBoard[WrappingModulo(pCol + 1, mBoardColumns)][WrappingModulo(pRow + 1, mBoardRows)];

	//Left and right of cell
	enabledSurroundingCells += mBoard[WrappingModulo(pCol - 1, mBoardColumns)][WrappingModulo(pRow, mBoardRows)];
	enabledSurroundingCells += mBoard[WrappingModulo(pCol + 1, mBoardColumns)][WrappingModulo(pRow, mBoardRows)];

	return enabledSurroundingCells;
}

//Allocate memory to the board
GameBoard::GameBoard(int pScreenWidth, int pScreenHeight, int pCellSize)
{
	//Set the member variables to the parameters
	mScreenWidth = pScreenWidth;
	mScreenHeight = pScreenHeight;
	mCellSize = pCellSize;

	//Calculate the number of cells wide and tall the board is
	mBoardColumns = mScreenWidth / mCellSize;
	mBoardRows = mScreenHeight / mCellSize;
	
	//Allocate memory to the board
	mBoard = new bool*[mBoardColumns];
	for (int col = 0; col < mBoardColumns; col++)
	{
		mBoard[col] = new bool[mBoardRows];

		//Set all the bools in this row to false by default
		for (int row = 0; row < mBoardRows; row++) 
		{
			mBoard[col][row] = false;
		}
	}

	//Allocate memory to the temporary board
	mBoard2 = new bool*[mBoardColumns];
	for (int col = 0; col < mBoardColumns; col++)
	{
		mBoard2[col] = new bool[mBoardRows];

		//Set all the bools in this row to false by default
		for (int row = 0; row < mBoardRows; row++)
		{
			mBoard2[col][row] = false;
		}
	}

	//Set the viewport
	glViewport(0, 0, mScreenWidth, mScreenHeight);
}

void GameBoard::CalculateNextFrame(std::string* console_ptr, int* population_ptr)
{	
	//Iterate through all the cells on the board
	for (int col = 0; col < mBoardColumns; col++)
	{
		for (int row = 0; row < mBoardRows; row++)
		{
			int surroundingCells = SurroundingCells(col, row);

			if (mBoard[col][row]) 
			{
				
				if (surroundingCells < 2 || surroundingCells > 3) {
					mBoard2[col][row] = false;
					*console_ptr += "Cell die at - " + std::to_string(col) + ", " + std::to_string(row) + "\n";
					if (*population_ptr > 0) {
						*population_ptr -= 1;
					}
				}

				else {
					mBoard2[col][row] = true;
				}
					
			}

			if (!mBoard[col][row])
			{
				if (surroundingCells == 3) {
					mBoard2[col][row] = true;
					*console_ptr += "Cell created at - " + std::to_string(col) + ", " + std::to_string(row) + "\n";
					*population_ptr += 1;
				}
					
				else {
					mBoard2[col][row] = false;
					
				}
					
			}
		}
	}

	//Switch the temporary board and the main board
	bool** tmp = mBoard;
	mBoard = mBoard2;
	mBoard2 = tmp;
}

void GameBoard::ChangedWindowSize(int pNewWidth, int pNewHeight)
{
	mScreenWidth = pNewWidth;
	mScreenHeight = pNewHeight;

	//Calculate the number of cells wide and tall the board is
	mBoardColumns = mScreenWidth / mCellSize;
	mBoardRows = mScreenHeight / mCellSize;

	//Set the viewport
	glViewport(0, 0, mScreenWidth, mScreenHeight);
}

void GameBoard::SetBoardPosition(int pCol, int pRow, bool pToggle, std::string* console_ptr, int* population_ptr, AudioDevicePtr device)
{
	if (pCol < mBoardColumns && pRow < mBoardRows && pCol >= 0 && pRow >= 0) { //If this index is within the right range
		if (mBoard[pCol][pRow] == true && pToggle == true) {
			return;
		}
		else if (mBoard[pCol][pRow] == true && pToggle == false) {
			mBoard[pCol][pRow] = pToggle;
			*population_ptr -= 1;
			*console_ptr += "Cell deleted at - " + std::to_string(pCol) + ", " + std::to_string(pRow) + "\n";
		}
		else if (mBoard[pCol][pRow] == false && pToggle == true) {
			OutputStreamPtr sound = OpenSound(device, "./birth_sound.wav", false);
			
			sound->play();
			mBoard[pCol][pRow] = pToggle;
			*console_ptr += "Cell created at - " + std::to_string(pCol) + ", " + std::to_string(pRow) + "\n";
			*population_ptr += 1;
			
		}
		else if (mBoard[pCol][pRow] == false && pToggle == false) {
			return;
		}
	}
}

void GameBoard::DrawGrid() 
{
	//Initialize openGl for drawing lines
	glLoadIdentity();
	glOrtho(0, mBoardColumns, 0, mBoardRows, 0, 1);
	glLineWidth(0.1); //Set the width of the line to 1
	glColor3f(0.8f, 0.8f, 0.8f); //Set the color of the line to grey
	glBegin(GL_LINES);

	//Draw the vertical lines
	for (int col = 0; col <= mBoardColumns; col++) 
	{
		float xPos = ((float)col / (float)mBoardColumns) * 2 - 1;
		glVertex2f(col, 0);
		glVertex2f(col, mScreenHeight);
	}

	//Draw the horizontal lines
	for (int row = 0; row <= mBoardRows; row++)
	{
		float yPos = ((float)row / (float)mBoardRows) * 2 - 1;
		glVertex2f(0, row);
		glVertex2f(mScreenWidth, row);
	}
	glEnd();
}

void GameBoard::DrawBoard(std::string* console_ptr, int* population_ptr)
{
	//Initialize openGl for drawing quads
	glLoadIdentity();
	glOrtho(0, mBoardColumns, 0, mBoardRows, 0, 1);
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_QUADS);
	
	//Iterate through all the cells on the board
	for (int col = 0; col < mBoardColumns; col++) 
	{
		for (int row = 0; row < mBoardRows; row++) 
		{
			if (mBoard[col][row]) //If this cell is enabled
			{
				
				glVertex2f(col, row);
				glVertex2f(col + 1, row);
				glVertex2f(col + 1, row + 1);
				glVertex2f(col, row + 1);
			}
		}
	}
	glEnd();
}

void GameBoard::DrawMouseHover(double pMouseX, double pMouseY)
{
	//Find what index on the board the mouse position corresponds to
	int xIndex, yIndex;
	xIndex = (int)(pMouseX / (double)mScreenWidth * mBoardColumns);
	yIndex = mBoardRows - (int)(pMouseY / (double)mScreenHeight * mBoardRows) - 1; //Y is inverted

	//Initialize openGl for drawing lines
	glLoadIdentity();
	glOrtho(0, mBoardColumns, 0, mBoardRows, 0, 1);
	glLineWidth(2); //Set the width of the line to 1
	glColor3f(0.2f, 0.2f, 0.2f); //Set the color of the line to grey
	glBegin(GL_LINES);

	//Top of box
	glVertex2f(xIndex, yIndex);
	glVertex2f(xIndex + 1, yIndex);

	//Bottom of box
	glVertex2f(xIndex + 1, yIndex + 1);
	glVertex2f(xIndex, yIndex + 1);

	//Left of box
	glVertex2f(xIndex, yIndex + 1);
	glVertex2f(xIndex, yIndex);

	//Right of box
	glVertex2f(xIndex + 1, yIndex + 1);
	glVertex2f(xIndex + 1, yIndex);

	glEnd();
}

void GameBoard::MouseClick(double pMouseX, double pMouseY, bool pEnable, int* population_ptr, std::string* console_ptr, AudioDevicePtr device)
{
	//Find what index on the board the mouse position corresponds to
	int xIndex, yIndex;
	xIndex = (int)(pMouseX / (double)mScreenWidth * mBoardColumns);
	yIndex = mBoardRows - (int)(pMouseY / (double)mScreenHeight * mBoardRows) - 1; //Y is inverted

	//Set that cell to be enabled/disabled
	SetBoardPosition(xIndex, yIndex, pEnable, console_ptr, population_ptr, device);
	
}

void GameBoard::ResetBoard(std::string* console_ptr, int* population_ptr)
{
	*console_ptr = "";
	*population_ptr = 0;
	//Iterate through all the cells on the board
	for (int col = 0; col < mBoardColumns; col++)
	{
		for (int row = 0; row < mBoardRows; row++)
		{
			mBoard[col][row] = false;
			mBoard2[col][row] = false;
		}
	}
}

void GameBoard::LoadPreconfiguration(int pConfig, std::string* console_ptr, int* population_ptr, AudioDevicePtr device)
{
	//Clear the board before we load the new config
	ResetBoard(console_ptr, population_ptr);
	OutputStreamPtr sound = OpenSound(device, "./birth_sound.wav", false);

	sound->play();
	//Calculate the center cell index
	int centerX, centerY;
	centerX = mBoardColumns / 2;
	centerY = mBoardRows / 2;
	
	if(pConfig == 1) //Glider
	{
		SetBoardPosition(centerX - 1, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX    , centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX    , centerY + 1, true, console_ptr, population_ptr, device);
	}
	else if (pConfig == 2) //Horizontal Glider
	{
		SetBoardPosition(centerX - 2, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 2, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 2, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX    , centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 2, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 2, centerY + 2, true, console_ptr, population_ptr, device);
	}
	else if (pConfig == 3) //Big Horizonal Glider
	{
		SetBoardPosition(centerX - 5, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY + 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY + 4, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY + 5, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 3, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 2, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY + 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY + 5, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX - 5, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY - 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY - 4, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY - 5, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 3, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 2, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY - 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY - 5, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX + 1, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 2, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 2, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 2, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 3, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 3, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 4, centerY    , true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 5, centerY    , true, console_ptr, population_ptr, device);
	}
	else if (pConfig == 4) //Glider gun
	{
		SetBoardPosition(centerX - 16, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 16, centerY, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 15, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 15, centerY, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX - 6, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 6, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 6, centerY, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY - 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 5, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 3, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 4, centerY - 4, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 3, centerY - 4, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX - 2, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY - 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX - 1, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX, centerY, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 1, centerY - 1, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX + 4, centerY, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 4, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 4, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 5, centerY, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 5, centerY + 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 5, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 6, centerY + 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 6, centerY - 1, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX + 8, centerY - 1, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 8, centerY - 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 8, centerY + 3, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 8, centerY + 4, true, console_ptr, population_ptr, device);

		SetBoardPosition(centerX + 18, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 18, centerY + 1, true, console_ptr, population_ptr, device)
			;
		SetBoardPosition(centerX + 19, centerY + 2, true, console_ptr, population_ptr, device);
		SetBoardPosition(centerX + 19, centerY + 1, true, console_ptr, population_ptr, device);

	}
	else if (pConfig == 5) //Oscillators
	{
		//Oscillator 1
		{
			SetBoardPosition(centerX - 22, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 22, centerY - 3, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 22, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 22, centerY - 1, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 22, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 22, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY - 2, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 21, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY + 1, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 22, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 20, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 19, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 18, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 17, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 22, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 21, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 20, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 19, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 18, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 17, centerY + 4, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 19, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 18, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 17, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 16, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY - 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 19, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 18, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 17, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 16, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY - 3, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX - 15, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY + 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 15, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY + 4, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY + 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX - 14, centerY - 1, true, console_ptr, population_ptr, device);
		}

		//Oscillator 2
		{
			SetBoardPosition(centerX + 1, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 1, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 1, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 1, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 1, centerY - 3, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX + 3, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 3, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 3, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 3, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 3, centerY - 3, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX + 2, centerY - 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 2, centerY + 1, true, console_ptr, population_ptr, device);
		}

		//Oscillator 3
		{
			SetBoardPosition(centerX + 18, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 18, centerY + 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 18, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 19, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 19, centerY + 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 19, centerY + 3, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 20, centerY + 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 20, centerY + 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 20, centerY + 3, true, console_ptr, population_ptr, device);

			SetBoardPosition(centerX + 21, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 21, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 21, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 22, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 22, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 22, centerY, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 23, centerY - 2, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 23, centerY - 1, true, console_ptr, population_ptr, device);
			SetBoardPosition(centerX + 23, centerY, true, console_ptr, population_ptr, device);
		}
	}
}
