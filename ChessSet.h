#pragma once

#include "..\..\inc\types.h"
class ChessSide;
#include "ChessBoard.h"
class ChessPiece;
#include "ChessPiece.h"
#include "ChessSet.h"
#include <vector>
#include <array>

class ChessSet {
public:
	ChessSet();
	ChessSet(std::string name, std::array<ChessPiece*, 16> pieces);
	std::string getName();
	std::array<ChessPiece*, 16>  getPieces();

protected:
	std::string mName;
	std::array<ChessPiece*, 16> mPieces;
};

class ChessSetFactory{
public:
	ChessSetFactory();
	
	std::vector<std::string> getChessSetNames();
	ChessSet getChessSetByName(std::string name);
	ChessSet getDefaultWhiteChessSet();
	ChessSet getDefaultBlackChessSet();

protected:
	std::vector<std::string> mChessSetNames;
	std::vector<ChessSet> mChessSets;

	void initialize();
	ChessSet getSimpleChessSet(ChessSide::Side side);

};