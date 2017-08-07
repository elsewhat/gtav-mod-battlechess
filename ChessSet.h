#pragma once

#include "inc\types.h"
class ChessSide;
#include "ChessSide.h"
#include "ChessBoard.h"
class ChessPiece;
#include "ChessPiece.h"
#include "ChessSet.h"
#include "tinyxml2.h"
#include <vector>
#include <array>

class ChessSet {
public:
	ChessSet();
	ChessSet(std::string name, std::array<ChessPiece*, 16> pieces);
	std::string getName();
	std::array<ChessPiece*, 16>  getPieces();
	void setSide(ChessSide::Side side);

protected:
	std::string mName;
	std::array<ChessPiece*, 16> mPieces;
};

class ChessSetFactory{
public:
	ChessSetFactory();
	
	std::vector<std::string> getChessSetNames();
	ChessSet* getChessSetByName(std::string name);
	ChessSet* getDefaultWhiteChessSet();
	ChessSet* getDefaultBlackChessSet();

protected:
	std::vector<ChessSet*> mChessSets;
	std::string FILENAME_CHESS_SETS;
	ChessSet* mDefaultWhiteChessSet;
	ChessSet* mDefaultBlackChessSet;

	void initialize();
	ChessPed getChessPedForElement(tinyxml2::XMLElement* chessSetElement);
	std::array<std::string, 3> getChessWeaponsForElement(tinyxml2::XMLElement* chessPieceElement);
	ChessSet* getSimpleChessSet(ChessSide::Side side);

};