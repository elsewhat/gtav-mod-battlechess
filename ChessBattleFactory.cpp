#include "ChessBattleFactory.h"

ChessBattleFactory::ChessBattleFactory()
{
	initialize();
}

std::shared_ptr<ChessBattle> ChessBattleFactory::findBattle(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getAttacker()->getPieceType())
	{
	case ChessPiece::PAWN:
		return getChessBattleForPawn(chessMove, chessBoard);
	case ChessPiece::ROOK:
		return getChessBattleForRook(chessMove, chessBoard);
	case ChessPiece::KNIGHT:
		return getChessBattleForKnight(chessMove, chessBoard);
	case ChessPiece::BISHOP:
		return getChessBattleForBishop(chessMove, chessBoard);
	case ChessPiece::QUEEN:
		return getChessBattleForQueen(chessMove, chessBoard);
	case ChessPiece::KING:
		return getChessBattleForKing(chessMove, chessBoard);
	default:
		return getChessBattleForPawn(chessMove, chessBoard);
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForPawn(ChessMove chessMove, ChessBoard * chessBoard)
{
	
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForRook(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForKnight(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForBishop(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForQueen(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForKing(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
	}
}

void ChessBattleFactory::initialize()
{

}
