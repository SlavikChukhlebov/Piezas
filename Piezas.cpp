#include "Piezas.h"
#include <vector>
#include <iostream>
/** CLASS Piezas
 * Class for representing a Piezas vertical board, which is roughly based
 * on the game "Connect Four" where pieces are placed in a column and 
 * fall to the bottom of the column, or on top of other pieces already in
 * that column. For an illustration of the board, see:
 *  https://en.wikipedia.org/wiki/Connect_Four
 *
 * Board coordinates [row,col] should match with:
 * [2,0][2,1][2,2][2,3]
 * [1,0][1,1][1,2][1,3]
 * [0,0][0,1][0,2][0,3]
 * So that a piece dropped in column 2 should take [0,2] and the next one
 * dropped in column 2 should take [1,2].
**/


/**
 * Constructor sets an empty board (default 3 rows, 4 columns) and 
 * specifies it is X's turn first
**/
Piezas::Piezas()
{
	//std::cout << "Constructing" << std::endl;
	Piece emptySpace = Blank;
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		board.push_back( std::vector<Piece>() );
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			board[row].push_back(emptySpace);
			//std::cout << (char)board[row][col];
		}
		//std::cout << std::endl;
	}
	turn = X;
}

/**
 * Resets each board location to the Blank Piece value, with a board of the
 * same size as previously specified
**/
void Piezas::reset()
{
	//std::cout << "Reseting" << std::endl;
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			board[row][col] = Blank;
			//std::cout << (char)board[row][col] << " ";
		}
		//std::cout << std::endl;
	}
}

/**
 * Places a piece of the current turn on the board, returns what
 * piece is placed, and toggles which Piece's turn it is. dropPiece does 
 * NOT allow to place a piece in a location where a column is full.
 * In that case, placePiece returns Piece Blank value 
 * Out of bounds coordinates return the Piece Invalid value
 * Trying to drop a piece where it cannot be placed loses the player's turn
**/ 
Piece Piezas::dropPiece(int column)
{	
	//std::cout << "Column " << column << std::endl;
	if(column >= BOARD_COLS || column < 0)
	{
		//std::cout << "Column " << column << " is out of range." << std::endl;
		return Invalid;
	}
	else if(board[BOARD_ROWS - 1][column] != Blank)
	{
		//std::cout << "Column " << column << " is full." << std::endl;
		return Blank;
	}
	
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		//std::cout << (char)board[row][column] << std::endl;
		if(board[row][column] == Blank)
		{
			//std::cout << "New value added" << std::endl;
			board[row][column] = turn;
			if(turn == X)
			{
				turn = O;
			}
			else
			{
				turn = X;
			}
			//std::cout << "[" << row << "][" << column << "] " << (char)board[row][column] << std::endl;
			//std::cout << "Piezas: " << (char)board[row][column] << std::endl;
			return board[row][column];
		}
	}
	
	//std::cout << "Piezas: B" << std::endl;
    return Blank;
}

/**
 * Returns what piece is at the provided coordinates, or Blank if there
 * are no pieces there, or Invalid if the coordinates are out of bounds
**/
Piece Piezas::pieceAt(int row, int column)
{
	if(row < 0 || row >= BOARD_ROWS
	|| column < 0 || column >= BOARD_COLS)
	{
		return Invalid;
	}
	
	//std::cout << "R: " << (char)board[row][column] << std::endl;
    return board[row][column];
}

/**
 * Returns which Piece has won, if there is a winner, Invalid if the game
 * is not over, or Blank if the board is filled and no one has won ("tie").
 * For a game to be over, all locations on the board must be filled with X's 
 * and O's (i.e. no remaining Blank spaces). The winner is which player has
 * the most adjacent pieces in a single line. Lines can go either vertically
 * or horizontally. If both X's and O's have the same max number of pieces in a
 * line, it is a tie.
**/
Piece Piezas::gameState()
{
	//Check if game is over
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			if(board[row][col] != X && board[row][col] != O)
			{
				//std::cout << "Invalid score, game is not over!" << std::endl;
				return Invalid;
			}
		}
	}
	
	int xHighScore = 0;
	int oHighScore = 0;
	int xScore = 0;
	int oScore = 0;
	Piece p = Blank;
	
	//For rows
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		xScore = 0;
		oScore = 0;
		p = X;
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			//std::cout << "[" << row << "][" << col << "]" << std::endl;
			//std::cout << (char)p << "->" << (char)board[row][col] << std::endl;
			if(p == board[row][col] && p == X)
			{
				xScore++;
			}
			else if(p == board[row][col] && p == O)
			{
				oScore++;
			}
			else if(p != board[row][col] && p == X)
			{
				p = O;
				xScore = 0;
				oScore++;
			}
			else if(p != board[row][col] && p == O)
			{
				p = X;
				oScore = 0;
				xScore++;
			}
			
			if(xScore > xHighScore)
			{
				xHighScore = xScore;
			}
			if(oScore > oHighScore)
			{
				oHighScore = oScore;
			}
		}
		//std::cout << "xHighScore: " << xHighScore << std::endl;
		//std::cout << "oHighScore: " << oHighScore << std::endl;
	}
	
	//For cols
	for(size_t col = 0; col < BOARD_COLS; col++)
	{
		xScore = 0;
		oScore = 0;
		p = X;
		for(size_t row = 0; row < BOARD_ROWS; row++)
		{
			//std::cout << "[" << row << "][" << col << "]" << std::endl;
			//std::cout << (char)p << "->" << (char)board[row][col] << std::endl;
			if(p == board[row][col] && p == X)
			{
				xScore++;
			}
			else if(p == board[row][col] && p == O)
			{
				oScore++;
			}
			else if(p != board[row][col] && p == X)
			{
				p = O;
				xScore = 0;
				oScore++;
			}
			else if(p != board[row][col] && p == O)
			{
				p = X;
				oScore = 0;
				xScore++;
			}
			
			if(xScore > xHighScore)
			{
				xHighScore = xScore;
			}
			if(oScore > oHighScore)
			{
				oHighScore = oScore;
			}
		}
		//std::cout << "xHighScore: " << xHighScore << std::endl;
		//std::cout << "oHighScore: " << oHighScore << std::endl;
	}
	
	if(xHighScore > oHighScore)
	{
		return X;
	}
	else if(xHighScore < oHighScore)
	{
		return O;
	}
	
    return Blank;
}
