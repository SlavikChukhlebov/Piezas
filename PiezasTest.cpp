/**
 * Unit Tests for Piezas
**/

#include <gtest/gtest.h>
#include "Piezas.h"
 
class PiezasTest : public ::testing::Test
{
	protected:
		PiezasTest(){} //constructor runs before each test
		virtual ~PiezasTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor) 
};

TEST(PiezasTest, sanityCheck)
{
	ASSERT_TRUE(true);
}

TEST(PiezasTest, constructor)
{
	Piezas testObj;
}

TEST(PiezasTest, reset)
{
	Piezas testObj;
	testObj.reset();
	
	//Check all initialized to Blank
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			ASSERT_TRUE(testObj.pieceAt(row, col) == Blank);
		}
	}
}

TEST(PiezasTest, dropPieceBoundaries)
{
	//Check boundaries
	Piezas testObj;
	ASSERT_TRUE(testObj.dropPiece(-1) == Invalid);
	ASSERT_TRUE(testObj.dropPiece(BOARD_COLS) == Invalid);
	ASSERT_FALSE(testObj.dropPiece(0) == Invalid);
	ASSERT_FALSE(testObj.dropPiece(BOARD_COLS - 1) == Invalid);
	ASSERT_FALSE(testObj.dropPiece(BOARD_COLS / 2) == Invalid);
}
	
TEST(PiezasTest, dropPieceContents)	
{
	Piezas testObj;
	Piece turn = X;
	
	//Check turns are alternating and Pieces are stored correctly
	for(size_t col = 0; col < BOARD_COLS; col++)
	{
		for(size_t row = 0; row < BOARD_ROWS; row++)
		{
			//Check return value
			ASSERT_TRUE(testObj.dropPiece(col) == turn);
			//Check real value
			ASSERT_TRUE(testObj.pieceAt(row, col) == turn);
			if(turn == X)
			{
				turn = O;
			}
			else
			{
				turn = X;
			}
		}
	}
	
	//Now all columns are full
	//Check that all full columns return Blank
	for(size_t col = 0; col < BOARD_COLS; col++)
	{
		ASSERT_TRUE(testObj.dropPiece(col) == Blank);
	}
}

TEST(PiezasTest, pieceAtBoundaries)
{
	Piezas testObj;
	
	ASSERT_TRUE(testObj.pieceAt(-1, 0) == Invalid);
	ASSERT_TRUE(testObj.pieceAt(BOARD_ROWS, 0) == Invalid);
	ASSERT_TRUE(testObj.pieceAt(0, -1) == Invalid);
	ASSERT_TRUE(testObj.pieceAt(0, BOARD_COLS) == Invalid);
	ASSERT_TRUE(testObj.pieceAt(-1, -1) == Invalid);
	ASSERT_TRUE(testObj.pieceAt(BOARD_ROWS, BOARD_COLS) == Invalid);
	
	ASSERT_FALSE(testObj.pieceAt(0, 0) == Invalid);
	ASSERT_FALSE(testObj.pieceAt(BOARD_ROWS - 1, 0) == Invalid);
	ASSERT_FALSE(testObj.pieceAt(0, BOARD_COLS - 1) == Invalid);
	ASSERT_FALSE(testObj.pieceAt(BOARD_ROWS - 1, BOARD_COLS - 1) == Invalid);
}

TEST(PiezasTest, pieceAtContents)
{
	Piezas testObj;
	ASSERT_TRUE(testObj.pieceAt(0, 0) == Blank);
	ASSERT_TRUE(testObj.pieceAt(0, 1) == Blank);
	
	bool x = true;
	for(size_t i = 0; i < BOARD_ROWS; i++)
	{
		testObj.dropPiece(0);
		if(x)
		{
			ASSERT_TRUE(testObj.pieceAt(i, 0) == X);
		}
		else
		{
			ASSERT_TRUE(testObj.pieceAt(i, 0) == O);
		}
		x = !x;
		ASSERT_TRUE(testObj.pieceAt(i, 1) == Blank);
	}
}

TEST(PiezasTest, gameStateInvalid)
{
	Piezas testObj;
	ASSERT_TRUE(testObj.gameState() == Invalid);
	
	for(size_t row = 0; row < BOARD_ROWS - 1; row++)
	{
		for(size_t col = 0; col < BOARD_COLS - 1; col++)
		{
			testObj.dropPiece(col);
		}
	}
	ASSERT_TRUE(testObj.gameState() == Invalid);
}

TEST(PiezasTest, gameStateTie)
{
	Piezas testObj;
	int xScore = 0;
	int yScore = 0;
	bool x = true;
	
	for(size_t row = 0; row < BOARD_ROWS; row++)
	{
		for(size_t col = 0; col < BOARD_COLS; col++)
		{
			testObj.dropPiece(col);
			if(x)
			{
				xScore++;
			}
			else
			{
				yScore++;
			}
			x = !x;
		}
	}
	ASSERT_TRUE(xScore == yScore);
	ASSERT_TRUE(testObj.gameState() == Blank);
}

TEST(PiezasTest, gameWin)
{	
	Piezas testObj;
	bool x = true;
	int maxMoves = BOARD_COLS * BOARD_ROWS;
	int moves = 0;
	int flip = 1;
	
	while(moves < maxMoves)
	{
		if(x)
		{
			for(size_t row = 0; row < BOARD_ROWS; row++)
			{
				if(flip > 0)
				{
					for(size_t col = 0; col < BOARD_COLS; col++)
					{
						if(testObj.dropPiece(col) == X)
						{
							moves++;
							std::cout << "#" << moves << " [" << col << "] X" << std::endl;
							col = BOARD_COLS;
							row = BOARD_ROWS;
							x = !x;
							flip *= -1;
							break;
						}
					}
				}
				else
				{
					for(int col = BOARD_COLS - 1; col >= 0; col--)
					{
						if(testObj.dropPiece(col) == X)
						{
							moves++;
							std::cout << "#" << moves << " [" << col << "] X" << std::endl;
							col = BOARD_COLS;
							row = BOARD_ROWS;
							x = !x;
							flip *= -1;
							break;
						}
					}
				}
			}
		}
		else
		{
			for(size_t col = 0; col < BOARD_COLS; col++)
			{
				for(size_t row = 0; row < BOARD_ROWS; row++)
				{
					if(testObj.dropPiece(col) == O)
					{
						moves++;
						std::cout << "#" << moves << " [" << col << "] O" << std::endl;
						col = BOARD_COLS;
						row = BOARD_ROWS;
						x = !x;
						break;
					}
				}
			}
		}
	}
	
	if(BOARD_COLS > BOARD_ROWS)
	{
		ASSERT_TRUE(testObj.gameState() == X);
	}
	else if(BOARD_ROWS < BOARD_COLS)
	{
		ASSERT_TRUE(testObj.gameState() == O);
	}
}
