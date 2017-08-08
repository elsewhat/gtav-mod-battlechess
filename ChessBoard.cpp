#include "ChessBoard.h"

#include "inc\natives.h"
#include "Utils.h"


ChessBoard::ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY)
{
	Logger::logDebug("ChessBoard::ChessBoard");
	mBaseLocation = baseLocation;
	mSquareDeltaX = squareDeltaX;
	mSquareDeltaY = squareDeltaY;
	initializeSquares();
	mSideToMove = ChessSide::WHITE;

	mChessBattleFactory = new ChessBattleFactory();

	mWhiteRelationshipGroupHash = GAMEPLAY::GET_HASH_KEY("BATTLECHESS_WHITE");
	mBlackRelationshipGroupHash = GAMEPLAY::GET_HASH_KEY("BATTLECHESS_BLACK");

	PED::REMOVE_RELATIONSHIP_GROUP(mWhiteRelationshipGroupHash);
	PED::REMOVE_RELATIONSHIP_GROUP(mBlackRelationshipGroupHash);

	PED::ADD_RELATIONSHIP_GROUP("BATTLECHESS_WHITE", &(mWhiteRelationshipGroupHash));
	PED::ADD_RELATIONSHIP_GROUP("BATTLECHESS_BLACK", &(mBlackRelationshipGroupHash));

	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, mWhiteRelationshipGroupHash, mBlackRelationshipGroupHash);
	PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, mBlackRelationshipGroupHash, mWhiteRelationshipGroupHash);
}

ChessSide::Side ChessBoard::sideToMove()
{
	return mSideToMove;
}

void ChessBoard::makeMove(ChessMove chessMove)
{
	if (chessMove.isNull()) {
		Logger::logError("ChessBoard::makeMove chessMove is unexpectedly null!");
		return;
	}
	ChessBoardSquare* squareFrom = chessMove.getSquareFrom();
	ChessBoardSquare* squareTo = chessMove.getSquareTo();

	Logger::assert(!squareFrom->isEmpty(), "ChessBoard::makeMove SquareFrom has no ChessPiece");

	if (chessMove.isCapture() && chessMove.isPromotion()) {
		Logger::logInfo("ChessBoard::makeMove TODO handle move that is both capture and promotion");
	}
	else if (chessMove.isPromotion()) {
		Logger::logInfo("ChessBoard::makeMove TODO handle move that is promotion");
	}
	else if (chessMove.isEnpassentCapture()) {
		Logger::logInfo("ChessBoard::makeMove TODO handle move that is enpassent capture");
	}
	else if (chessMove.isCastling()) {
		Logger::logInfo("ChessBoard::makeMove TODO handle move that is castling");
	}
	else if (chessMove.isCapture()) {
		ChessPiece* attacker = chessMove.getAttacker();
		ChessPiece* defender = chessMove.getDefender();

		squareFrom->removePiece();
		squareTo->removePiece();
		squareTo->setPiece(attacker);

		//Logger::logInfo("ChessBoard::makeMove TODO handle move that is capture");
	}
	else {
		ChessPiece* chessPiece = chessMove.getAttacker();
		squareFrom->removePiece();
		squareTo->setPiece(chessPiece);

	}

	if (chessMove.isEnpassentEnabling()) {
		Logger::logInfo("ChessBoard::makeMove TODO set enpassent on square");
	}
	//TODO: Switch sides
	mSideToMove = ChessSide::oppositeSide(mSideToMove);
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
				//Logger::logDebug("ChessBoard::setWhiteChessSet Setting piece with index "+ std::to_string(pieceIndex));
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
				//Logger::logDebug("ChessBoard::setBlackChessSet Setting piece with index " + std::to_string(pieceIndex));
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
			
			if (piece->getSide() == ChessSide::WHITE) {
				piece->spawnPed(mWhiteRelationshipGroupHash);
			}
			else {
				piece->spawnPed(mBlackRelationshipGroupHash);
			}
		}
		else {
			//Logger::logDebug("Square is empty Rank" + std::to_string(square->getSquareRank()) + " File:" + std::to_string(square->getSquareFile()));
		}
	}
}

void ChessBoard::freezeAllExcept(std::vector<ChessPiece*> chessPieces)
{
	for (auto* square : mSquares) {
		if (!square->isEmpty()) {
			square->getPiece()->setPedFreezed(true);
		}
	}
	for (auto* piece : chessPieces) {
		piece->setPedFreezed(false);
	}
}

ChessBattleFactory * ChessBoard::getChessBattleFactory()
{
	return mChessBattleFactory;
}

void ChessBoard::setSyncedAnimationFactory(SyncedAnimationFactory * syncedAnimationFactory)
{
	mSyncedAnimationFactory = syncedAnimationFactory;
}

SyncedAnimationFactory * ChessBoard::getSyncedAnimationFactory()
{
	return mSyncedAnimationFactory;
}

void ChessBoard::setAnimationFactory(AnimationFactory * animationFactory)
{
	mAnimationFactory = animationFactory;
}

AnimationFactory * ChessBoard::getAnimationFactory()
{
	return mAnimationFactory;
}

void ChessBoard::drawOnTick()
{
	for (auto* square : mSquares) {
		square->drawOnTick();
	}

	/*
	for (int rank = 1; rank <= 8; rank++) {
		ChessBoardSquare* squareStart = getSquareAt(rank, 1);
		ChessBoardSquare* squareEnd = getSquareAt(rank, 8);

		GRAPHICS::DRAW_LINE(squareStart->getLocation().x - 1.0, squareStart->getLocation().y - 1.0, squareStart->getLocation().z, squareEnd->getLocation().x - 1.0, squareEnd->getLocation().y - 1.0, squareEnd->getLocation().z, 255, 255, 255, 255);
		if (rank == 8) {
			GRAPHICS::DRAW_LINE(squareStart->getLocation().x + 1.0, squareStart->getLocation().y + 1.0, squareStart->getLocation().z, squareEnd->getLocation().x + 1.0, squareEnd->getLocation().y + 1.0, squareEnd->getLocation().z, 255, 255, 255, 255);
		}
	}

	for (int file = 1; file <= 8; file++) {
		ChessBoardSquare* squareStart = getSquareAt(1, file);
		ChessBoardSquare* squareEnd = getSquareAt(8, file);

		GRAPHICS::DRAW_LINE(squareStart->getLocation().x - 1.0, squareStart->getLocation().y - 1.0, squareStart->getLocation().z, squareEnd->getLocation().x - 1.0, squareEnd->getLocation().y - 1.0, squareEnd->getLocation().z, 255, 255, 255, 255);
		if (file == 8) {
			GRAPHICS::DRAW_LINE(squareStart->getLocation().x + 1.0, squareStart->getLocation().y + 1.0, squareStart->getLocation().z, squareEnd->getLocation().x + 1.0, squareEnd->getLocation().y + 1.0, squareEnd->getLocation().z, 255, 255, 255, 255);
		}
	}*/
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
		bool isWhitePawnLine = false;
		bool isBlackPawnLine = false;
		if (rank == 1 || rank == 8) {
			isPromotion = true;
		}
		if (rank == 2) {
			isWhitePawnLine = true;
		}
		if (rank == 7) {
			isBlackPawnLine = true;
		}

		for (int file = 1; file <= 8; file++) {
			//A1 is Dark. Alternating afterwards
			ChessBoardSquare::Color color = ChessBoardSquare::DARK;
			if ((rank + file) % 2 == 1) {
				color = ChessBoardSquare::LIGHT;
			}

			Vector3 location;
			location.x = mBaseLocation.x + mSquareDeltaX * (file-1) ;
			location.y = mBaseLocation.y + mSquareDeltaY * (rank-1);
			location.z = mBaseLocation.z;

			mSquares[index] = new ChessBoardSquare(file, rank, isPromotion, isWhitePawnLine, isBlackPawnLine, color, location, headingWhite, headingBlack);
			index++;
		}
	}

}

ChessBoardSquare* ChessBoard::getSquareAt(int rank, int file)
{
	Logger::assert(rank >= 1 && rank <= 8 && file >= 1 && file <= 8, "ChessBoard::getSquareAt rank or file is not in [1,8]");
	
	int index = (rank - 1) * 8 + (file - 1);
	return mSquares[index];
}

bool ChessBoard::isValidRankAndFile(int rank, int file)
{
	if (rank >= 1 && rank <= 8 && file >= 1 && file <= 8) {
		return true;
	}
	else {
		return false;
	}
}

ChessBoardSquare * ChessBoard::getSquareInFrontOf(ChessBoardSquare * square, ChessSide::Side side)
{
	if (square->getSquareRank() == 8 || square->getSquareRank() == 1) {
		Logger::logError("ChessBoard::getSquareInFrontOf is already on the edge");
		return square;
	}
	if (side == ChessSide::WHITE) {
		return getSquareAt(square->getSquareRank() - 1, square->getSquareFile());
	}
	else {
		return getSquareAt(square->getSquareRank() + 1, square->getSquareFile());
	}

}

std::vector<ChessMove> ChessBoard::possibleMoves(ChessSide::Side side, ChessBoardSquare * squareFrom)
{
	std::vector<ChessMove> possibleMoves;
	
	//Inspired by cutechess
	//https://github.com/cutechess/cutechess/blob/master/projects/lib/src/board/westernboard.cpp

	if (!squareFrom->isEmpty()) {
		int rankFrom = squareFrom->getSquareRank();
		int fileFrom = squareFrom->getSquareFile();

		ChessPiece* pieceFrom = squareFrom->getPiece();
		ChessPiece::Type type = pieceFrom->getPieceType();

		Logger::logInfo("ChessBoard::possibleMoves for  rankFrom" + std::to_string(rankFrom) + " fileFrom" + std::to_string(fileFrom) + " Type:" + std::to_string(type));

		if (type == ChessPiece::PAWN) {
			possibleMoves = generatePawnMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}
		else if (type == ChessPiece::KNIGHT) {
			possibleMoves = generateHoppingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}
		else if (type == ChessPiece::BISHOP) {
			possibleMoves = generateSlidingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}
		else if (type == ChessPiece::ROOK) {
			possibleMoves = generateSlidingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
			possibleMoves = generateCastlingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}
		else if (type == ChessPiece::QUEEN) {
			possibleMoves = generateSlidingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}
		else if (type == ChessPiece::KING) {
			possibleMoves = generateKingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
			possibleMoves = generateCastlingMoves(possibleMoves, pieceFrom->getSide(), squareFrom, pieceFrom);
		}


	}

	Logger::logInfo("ChessBoard::possibleMoves returns " + std::to_string(possibleMoves.size()) + " moves");
	return possibleMoves;
}


std::vector<ChessMove> ChessBoard::generatePawnMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare * squareFrom, ChessPiece* piece)
{
	Logger::logInfo("ChessBoard::generatePawnMoves");
	//verify with https://github.com/cutechess/cutechess/blob/master/projects/lib/src/board/westernboard.cpp#L1145
	//however, we do it slightly different
	int pawnStep = 1;
	if (side == ChessSide::BLACK) {
		pawnStep = -1;
	}

	int rank = squareFrom->getSquareRank();
	int file = squareFrom->getSquareFile();

	ChessBoardSquare* squareForward = NULL;
	if (isValidRankAndFile(rank + (pawnStep), file)) {
		squareForward = getSquareAt(rank + (pawnStep), file);
		if (squareForward->isEmpty()) {
			possibleMoves.push_back(ChessMove(squareFrom, squareForward, false, squareForward->isPromotion()));
		}
	}

	//Check capture to the sides sides. Left first, then right
	for (int sideFile = file - 1; sideFile <= file + 1; sideFile += 2) {
		if (isValidRankAndFile(rank + (pawnStep), sideFile)) {
			ChessBoardSquare* squareForwardSide = getSquareAt(rank + (pawnStep), sideFile);
			
			if (!squareForwardSide->isEmpty()) {
				ChessPiece* potentialCapturePiece = squareForwardSide->getPiece();
				if (potentialCapturePiece->getSide() == ChessSide::oppositeSide(side)) {
					possibleMoves.push_back(ChessMove(squareFrom, squareForwardSide,true));
				}
			}
			//enpassent
			if (squareForwardSide->isEmpty() && squareForwardSide->isEnpassentSquare(side)) {
				possibleMoves.push_back(ChessMove(squareFrom, squareForwardSide, true,false,true));
			}
		}
	}

	//Double move
	if (squareFrom->isPawnLine(side)) {
		if (isValidRankAndFile(rank + (pawnStep * 2), file)) {	
			ChessBoardSquare * squareDoubleMove = getSquareAt(rank + (pawnStep * 2), file);
			if (squareForward->isEmpty() && squareDoubleMove->isEmpty()) {
				possibleMoves.push_back(ChessMove(squareFrom, squareDoubleMove, false));
				//when executed, this move should set the enpassent of the first square
			}
		}
	}
	return possibleMoves;
}


std::vector<ChessMove> ChessBoard::generateSlidingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare * squareFrom, ChessPiece* piece)
{

	std::vector<int> rankOffsets;
	std::vector<int> fileOffsets;
	bool doMoveSingleSquare = false;

	ChessPiece::Type type = piece->getPieceType();
	if (type == ChessPiece::BISHOP) {
		rankOffsets = { 1,1,-1,-1 };
		fileOffsets = { 1,-1,1,-1 };
	}
	else if (type == ChessPiece::QUEEN) {
		rankOffsets = { 1,1,-1,-1, 1,-1, 0, 0};
		fileOffsets = { 1,-1,1,-1, 0, 0, 1, -1};
	}
	else if (type == ChessPiece::ROOK) {
		rankOffsets = { 1,-1, 0, 0 };
		fileOffsets = { 0, 0, 1, -1 };
	}

	for (int i = 0; i < rankOffsets.size(); i++) {
		int targetRank = squareFrom->getSquareRank()+rankOffsets[i];
		int targetFile = squareFrom->getSquareFile()+fileOffsets[i];

		while (isValidRankAndFile(targetRank, targetFile)) {
			ChessBoardSquare* targetSquare = getSquareAt(targetRank, targetFile);
			if (targetSquare->isEmpty()) {
				possibleMoves.push_back(ChessMove(squareFrom, targetSquare, false));
			}
			else {
				ChessPiece* targetPiece = targetSquare->getPiece();
				if (targetPiece->getSide() != side) {
					possibleMoves.push_back(ChessMove(squareFrom, targetSquare, true));
				}
				//jump out of while loop
				break;
			}
			targetRank += rankOffsets[i];
			targetFile += fileOffsets[i];
		}
	}
	return possibleMoves;
}


std::vector<ChessMove> ChessBoard::generateHoppingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare * squareFrom, ChessPiece* piece)
{
	std::array<int, 8>  rankOffsets = { 1 ,2 ,1 ,2 ,-1,-2,-1,-2 };
	std::array<int, 8> fileOffsets = { 2 ,1 ,-2,-1,-2,-1, 2, 1};

	for (int i = 0; i < rankOffsets.size(); i++) {
		int targetRank = squareFrom->getSquareRank() + rankOffsets[i];
		int targetFile = squareFrom->getSquareFile() + fileOffsets[i];

		if (isValidRankAndFile(targetRank, targetFile)) {
			ChessBoardSquare* targetSquare = getSquareAt(targetRank, targetFile);
			if (targetSquare->isEmpty()) {
				possibleMoves.push_back(ChessMove(squareFrom, targetSquare, false));
			}
			else {
				ChessPiece* targetPiece = targetSquare->getPiece();
				if (targetPiece->getSide() != side) {
					possibleMoves.push_back(ChessMove(squareFrom, targetSquare, true));
				}
			}
		}
	}
	return possibleMoves;
}

std::vector<ChessMove> ChessBoard::generateCastlingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare * squareFrom, ChessPiece* piece)
{
	return possibleMoves;
}


std::vector<ChessMove> ChessBoard::generateKingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare * squareFrom, ChessPiece* piece)
{
	std::array<int,8> rankOffsets= { 1,1,-1,-1, 1,-1, 0, 0 };;
	std::array<int,8> fileOffsets= { 1,-1,1,-1, 0, 0, 1, -1 };;

	//TODO For each move, verify if king is in check

	for (int i = 0; i < rankOffsets.size(); i++) {
		int targetRank = squareFrom->getSquareRank() + rankOffsets[i];
		int targetFile = squareFrom->getSquareFile() + fileOffsets[i];

		if (isValidRankAndFile(targetRank, targetFile)) {
			ChessBoardSquare* targetSquare = getSquareAt(targetRank, targetFile);
			if (targetSquare->isEmpty()) {
				possibleMoves.push_back(ChessMove(squareFrom, targetSquare, false));
			}
			else {
				ChessPiece* targetPiece = targetSquare->getPiece();
				if (targetPiece->getSide() != side) {
					possibleMoves.push_back(ChessMove(squareFrom, targetSquare, true));
				}
			}
		}
	}
	return possibleMoves;
}
