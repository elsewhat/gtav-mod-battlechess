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
	case ChessPiece::PAWN: {
		std::shared_ptr<SyncedAnimation> syncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Drunken fist fight");
		if (syncedAnimation != nullptr) {
			return std::make_shared <ChessBattleSyncedAnimation>(ChessBattleSyncedAnimation(syncedAnimation,true,true, Vector3()));
		}
		else {
			Logger::logDebug("Drunken fist fight synced anim does not exist");
			return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
		}
	}
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	default:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon());
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForRook(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon("FIRING_PATTERN_FULL_AUTO"));
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
	case ChessPiece::KNIGHT: {
		std::shared_ptr<SyncedAnimation> syncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Guitar beatdown");
		if (syncedAnimation != nullptr) {
			Vector3 offset;
			offset.x = -1.5;
			offset.y = -0.5;
			offset.z = 0.0;
			return std::make_shared <ChessBattleSyncedAnimation>(ChessBattleSyncedAnimation(syncedAnimation,false,true, offset));
		}
		else {
			Logger::logDebug("Guitar beatdown synced anim does not exist");
			return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon());
		}
	}
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
