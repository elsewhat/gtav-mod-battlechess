#include "ChessSet.h"
#include "Utils.h"

ChessSet::ChessSet()
{
}

ChessSet::ChessSet(std::string name, std::array<ChessPiece*, 16> pieces)
{
	mName = name;
	mPieces = pieces;
}

std::string ChessSet::getName()
{
	return mName;
}

std::array<ChessPiece*, 16> ChessSet::getPieces()
{
	return mPieces;
}

ChessSetFactory::ChessSetFactory()
{
	initialize();
}

std::vector<std::string> ChessSetFactory::getChessSetNames()
{
	return mChessSetNames;
}

ChessSet* ChessSetFactory::getChessSetByName(std::string name)
{
	return getSimpleChessSet(ChessSide::WHITE);
}

ChessSet* ChessSetFactory::getDefaultWhiteChessSet()
{
	return getSimpleChessSet(ChessSide::WHITE);
}

ChessSet* ChessSetFactory::getDefaultBlackChessSet()
{
	return getSimpleChessSet(ChessSide::BLACK);
}

void ChessSetFactory::initialize()
{
	//TODO: Load from xml file
}

ChessSet* ChessSetFactory::getSimpleChessSet(ChessSide::Side side)
{
	Logger::logDebug("ChessSetFactory::getSimpleChessSet");
	std::array<ChessPiece*, 16>  pieces;
	int index = 0;
	for (int i = 0; i < 8; i++) {
		pieces[index++] = new ChessPiece(side, ChessPiece::PAWN, ChessPed(), Vector3(), 1.0);;
	}
	pieces[index++] = new ChessPiece(side, ChessPiece::ROOK, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::BISHOP, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::KNIGHT, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::QUEEN, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::KING, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::KNIGHT, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::BISHOP, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::ROOK, ChessPed(), Vector3(), 1.0);

	ChessSet* chessSet = new ChessSet("Simple Set", pieces);
	return chessSet;
}
