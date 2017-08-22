#include "ChessBattleFactory.h"
#include <stdlib.h>
#include "GTAUtils.h"
#include "Animation.h"

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
		if (MathUtils::percentageChance(65)) {
			return std::make_shared <ChessBattleHandToHandWeapon>(ChessBattleHandToHandWeapon(chessMove, chessBoard,WeaponUtils::getRandomHandToHandWeaponName(), WeaponUtils::getRandomHandToHandWeaponName(), false, 150));
		}
		else {
			return std::make_shared <ChessBattleHeadbutt>(ChessBattleHeadbutt(chessMove, chessBoard));
		}
	}
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleThrowGrenade>(ChessBattleThrowGrenade(chessMove, chessBoard, "WEAPON_GRENADE"));
	case ChessPiece::KNIGHT: {
		std::shared_ptr<SyncedAnimation> firstSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Knife struggle (tackle)");
		std::shared_ptr<SyncedAnimation> secondSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Knife struggle (loop)");
		if (firstSyncedAnimation != nullptr && secondSyncedAnimation != nullptr) {
			Vector3 offset;
			offset.x = -1.5;
			offset.y = -0.5;
			offset.z = 0.0;
			return std::make_shared <ChessBattleSyncedAnimationChained>(ChessBattleSyncedAnimationChained(chessMove, chessBoard, firstSyncedAnimation, secondSyncedAnimation, false, true, offset, true, "WEAPON_KNIFE"));
		}
		else {
			Logger::logDebug("Knife struggle (tackle) synced anim does not exist");
			return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon(chessMove, chessBoard));
		}
	}
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleJerryCan>(ChessBattleJerryCan(chessMove, chessBoard));
		//return std::make_shared <ChessBattleStealthKill>(ChessBattleStealthKill());
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleDeathByCop>(ChessBattleDeathByCop(chessMove, chessBoard));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon(chessMove, chessBoard));
	default:
		return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon(chessMove, chessBoard));
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForRook(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard, "FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleNeedBiggerGun>(ChessBattleNeedBiggerGun(chessMove, chessBoard, "WEAPON_MINIGUN", "FIRING_PATTERN_FULL_AUTO", "WEAPON_RPG", "FIRING_PATTERN_FULL_AUTO", 7000));
	case ChessPiece::KNIGHT:{
		std::shared_ptr<SyncedAnimation> syncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Guitar beatdown");
		if (syncedAnimation != nullptr) {
			Vector3 offset;
			offset.x = -1.5;
			offset.y = -0.5;
			offset.z = 0.0;
			return std::make_shared <ChessBattleSyncedAnimation>(ChessBattleSyncedAnimation(chessMove, chessBoard, syncedAnimation, false, true, offset));
		}
		else {
			Logger::logDebug("Guitar beatdown synced anim does not exist");
			return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
		}
	}
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard, "FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleTank>(ChessBattleTank(chessMove, chessBoard));
		//return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard, "FIRING_PATTERN_FULL_AUTO"));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard, "FIRING_PATTERN_FULL_AUTO"));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard, "FIRING_PATTERN_FULL_AUTO"));
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForKnight(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleHatchetFront>(ChessBattleHatchetFront(chessMove, chessBoard));
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::KNIGHT: {
			std::shared_ptr<SyncedAnimation> syncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Drunken fist fight");
			if (syncedAnimation != nullptr) {
				return std::make_shared <ChessBattleSyncedAnimation>(ChessBattleSyncedAnimation(chessMove, chessBoard,syncedAnimation, true, true, Vector3()));
			}
			else {
				Logger::logDebug("Drunken fist fight synced anim does not exist");
				return std::make_shared <ChessBattleFireSecondaryWeapon>(ChessBattleFireSecondaryWeapon(chessMove, chessBoard));
			}
		}

	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleShootOut>(ChessBattleShootOut(chessMove, chessBoard));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleSlap>(ChessBattleSlap(chessMove, chessBoard));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForBishop(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleShoryuken>(ChessBattleShoryuken(chessMove, chessBoard));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForQueen(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleTurnIntoAnimal>(ChessBattleTurnIntoAnimal(chessMove, chessBoard,"a_c_hen", chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(2925),3000));
	case ChessPiece::ROOK:
		return std::make_shared <ChessBattleAttackOfTheClones>(ChessBattleAttackOfTheClones(chessMove, chessBoard));
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleAttackedByAnimals>(ChessBattleAttackedByAnimals(chessMove, chessBoard, "a_c_mtlion",2));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	}
}

std::shared_ptr<ChessBattle> ChessBattleFactory::getChessBattleForKing(ChessMove chessMove, ChessBoard * chessBoard)
{
	switch (chessMove.getDefender()->getPieceType())
	{
	case ChessPiece::PAWN:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::ROOK://WEAPON_MOLOTOV
		return std::make_shared <ChessBattleFiringSquad>(ChessBattleFiringSquad(chessMove, chessBoard, "WEAPON_GRENADE", true, chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(2925)));
	case ChessPiece::KNIGHT:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::BISHOP:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::QUEEN:
		return std::make_shared <ChessBattleFiringSquad>(ChessBattleFiringSquad(chessMove, chessBoard, "WEAPON_SNOWBALL", false, chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(2925)));
		//return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	case ChessPiece::KING:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	default:
		return std::make_shared <ChessBattleFirePrimaryWeapon>(ChessBattleFirePrimaryWeapon(chessMove, chessBoard));
	}
}

void ChessBattleFactory::initialize()
{

}
