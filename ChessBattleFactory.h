#pragma once

#include "inc\types.h"
class ChessBattle;
#include "ChessBattle.h"
class ChessBoard;
#include "ChessBoard.h"
#include "ChessMove.h"
class ChessMove;
#include "ChessMove.h"
#include <vector>
#include <memory>

class ChessBattleFactory {
public:
	ChessBattleFactory();

	std::shared_ptr<ChessBattle> findBattle(ChessMove chessMove, ChessBoard* chessBoard);

protected:
	std::shared_ptr<ChessBattle> getChessBattleForPawn(ChessMove chessMove, ChessBoard* chessBoard);
	std::shared_ptr<ChessBattle> getChessBattleForRook(ChessMove chessMove, ChessBoard* chessBoard);
	std::shared_ptr<ChessBattle> getChessBattleForKnight(ChessMove chessMove, ChessBoard* chessBoard);
	std::shared_ptr<ChessBattle> getChessBattleForBishop(ChessMove chessMove, ChessBoard* chessBoard);
	std::shared_ptr<ChessBattle> getChessBattleForQueen(ChessMove chessMove, ChessBoard* chessBoard);
	std::shared_ptr<ChessBattle> getChessBattleForKing(ChessMove chessMove, ChessBoard* chessBoard);

	void initialize();
};