#include "ChessBoard.h"

#include "..\..\inc\natives.h"
#include "Utils.h"


ChessBoard::ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY)
{
	Logger::logDebug("ChessBoard::ChessBoard");
	mBaseLocation = baseLocation;
	mSquareDeltaX = squareDeltaX;
	mSquareDeltaY = squareDeltaY;
	initializeSquares();
}

bool ChessBoard::hasSideChessPieceAt(ChessSide::Side side, int rank, int file)
{
	Logger::assert(rank >= 1 && rank <= 8 && file >= 1 && file <= 1, "ChessBoard::hasSideChessPieceAt rank or file is not in [1,8]");

	ChessPiece* piece = getChessPieceAt(rank, file);
	if (piece != nullptr && piece->getSide() == side) {
		return true;
	}
	else {
		return false;
	}

}

ChessPiece* ChessBoard::getChessPieceAt(int rank, int file)
{
	Logger::assert(rank >= 1 && rank <= 8 && file >= 1 && file <= 8, "ChessBoard::getChessPieceAt rank or file is not in [1,8]");

	ChessBoardSquare* square = getSquareAt(rank, file);
	return square->getPiece();
}

void ChessBoard::setWhiteChessSet(ChessSet* whiteSet)
{
	Logger::logDebug("ChessBoard::setWhiteChessSet");
	mWhiteSet = whiteSet;

	std::array<ChessPiece*, 16> pieces = mWhiteSet->getPieces();

	//assign ChessPieces to ChessBoardSqaures
	//ChessSet is ordered
	int pieceIndex = 0;
	for (int rank = 2; rank >= 1; rank--) {
		for (int file = 1; file <= 8; file++) {
			ChessBoardSquare* square = getSquareAt(rank, file);

			if (square != nullptr) {
				Logger::logDebug("ChessBoard::setWhiteChessSet Setting piece with index "+ std::to_string(pieceIndex));
				square->setPiece(pieces[pieceIndex]);
				pieceIndex++;
			}
		}
	}

}

void ChessBoard::setBlackChessSet(ChessSet* blackSet)
{
	Logger::logDebug("ChessBoard::setBlackChessSet");
	mBlackSet = blackSet;

	std::array<ChessPiece*, 16> pieces = mBlackSet->getPieces();

	//assign ChessPieces to ChessBoardSqaures
	//ChessSet is ordered
	int pieceIndex = 0;
	for (int rank = 7; rank <= 8; rank++) {
		for (int file = 1; file <= 8; file++) {
			ChessBoardSquare* square = getSquareAt(rank, file);

			if (square != nullptr) {
				Logger::logDebug("ChessBoard::setBlackChessSet Setting piece with index " + std::to_string(pieceIndex));
				square->setPiece(pieces[pieceIndex]);
				pieceIndex++;
			}
		}
	}
}

void ChessBoard::spawnChessPieces()
{
	Logger::logInfo("ChessBoard::spawnChessPieces");
	//for (auto* square : mSquares) {
	for (int i = 0; i < mSquares.size(); i++) {
		ChessBoardSquare* square = mSquares[i];
		if (!square->isEmpty()) {
			ChessPiece* piece = square->getPiece();
			
			Logger::logInfo("Spawning piece Side:" + std::to_string(piece->getSide()) + " Type:"+ std::to_string(piece->getPieceType()));
			piece->spawnPed();
		}
		else {
			Logger::logInfo("Square is empty Rank" + std::to_string(square->getSquareRank()) + " File:" + std::to_string(square->getSquareFile()));
		}
	}
}

void ChessBoard::drawOnTick()
{
	for (auto* square : mSquares) {
		square->drawOnTick();
	}
}

/*
Create square in the following pattern
Double-digit values are indexs in array
A  B  C  D  E  F  G  H
File value                Rank value
1  2  3  4  5  6  7  8
00 01 02 03 04 05 06 07    1
08 09 10 11 12 13 14 15    2
16 17 18 19 20 21 22 23    3
24 25 26 27 28 29 30 31    4
32 33 34 35 36 37 38 39    5
40 41 42 43 44 45 46 47    6
48 49 50 51 52 53 54 55    7
56 57 58 59 60 61 62 63    8
*/
void ChessBoard::initializeSquares()
{
	Logger::logDebug("ChessBoard::initializeSquares");
	int index = 0;
	float headingWhite = 0.0;
	float headingBlack = 180.0;

	for (int rank = 1; rank <= 8; rank++) {
		bool isPromotion = false;
		if (rank == 1 || rank == 8) {
			isPromotion = true;
		}

		for (int file = 1; file <= 8; file++) {
			//A1 is Dark. Alternating afterwards
			ChessBoardSquare::Color color = ChessBoardSquare::DARK;
			if (rank + file % 2 == 1) {
				color = ChessBoardSquare::LIGHT;
			}

			Vector3 location;
			location.x = mBaseLocation.x + mSquareDeltaX * (file-1) ;
			location.y = mBaseLocation.y + mSquareDeltaY * (rank-1);
			location.z = mBaseLocation.z;

			mSquares[index] = new ChessBoardSquare(file, rank, isPromotion, color, location, headingWhite, headingBlack);
			index++;
		}
	}

}

ChessBoardSquare* ChessBoard::getSquareAt(int rank, int file)
{
	Logger::logDebug("Rank:" + std::to_string(rank) + " File:" + std::to_string(file));
	Logger::assert(rank >= 1 && rank <= 8 && file >= 1 && file <= 8, "ChessBoard::getSquareAt rank or file is not in [1,8]");
	
	int index = (rank - 1) * 8 + (file - 1);
	Logger::logDebug("Index:" + std::to_string(index) + " mSquares.size:" + std::to_string(mSquares.size()));
	return mSquares[index];
}

std::vector<ChessBoardSquare*> ChessBoard::possibleMoves(ChessSide::Side side, const ChessBoardSquare * squareFrom)
{
	//TODO: Implement
	return std::vector<ChessBoardSquare*>();
}
