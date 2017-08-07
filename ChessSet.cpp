#include "ChessSet.h"
#include "Utils.h"
#include "GTAUtils.h"


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

void ChessSet::setSide(ChessSide::Side side)
{
	for (auto* piece : mPieces) {
		piece->setSide(side);
	}
}

ChessSetFactory::ChessSetFactory()
{
	FILENAME_CHESS_SETS = "Battlechess_sets.xml";
	mDefaultWhiteChessSet = getSimpleChessSet(ChessSide::WHITE);
	mDefaultBlackChessSet = getSimpleChessSet(ChessSide::BLACK);

	initialize();
}

std::vector<std::string> ChessSetFactory::getChessSetNames()
{
	std::vector<std::string> chessSetNames;
	for (auto* chessSet : mChessSets) {
		chessSetNames.push_back(chessSet->getName());
	}

	return chessSetNames;
}

ChessSet* ChessSetFactory::getChessSetByName(std::string name)
{
	return getSimpleChessSet(ChessSide::WHITE);
}

ChessSet* ChessSetFactory::getDefaultWhiteChessSet()
{
	mDefaultWhiteChessSet->setSide(ChessSide::WHITE);
	return mDefaultWhiteChessSet;
}

ChessSet* ChessSetFactory::getDefaultBlackChessSet()
{
	mDefaultBlackChessSet->setSide(ChessSide::BLACK);
	return mDefaultBlackChessSet;
}

void ChessSetFactory::initialize()
{
	tinyxml2::XMLDocument doc = new tinyxml2::XMLDocument();
	doc.LoadFile(FILENAME_CHESS_SETS.c_str());
	if (doc.Error()) {
		UIUtils::setStatusText("Chess set file " + FILENAME_CHESS_SETS + " could not be loaded. Error: " + doc.ErrorName());
		return;
	}

	tinyxml2::XMLElement* rootElement = doc.RootElement();

	for (tinyxml2::XMLElement* chessSetElement = rootElement->FirstChildElement("ChessSet");
	chessSetElement;
		chessSetElement = chessSetElement->NextSiblingElement())
	{
		std::string chessSetName = chessSetElement->Attribute("name");
		bool isDefaultWhite = chessSetElement->BoolAttribute("isDefaultWhite");
		bool isDefaultBlack = chessSetElement->BoolAttribute("isDefaultBlack");

		ChessPed chessPedPawn = getChessPedForElement(chessSetElement->FirstChildElement("Pawn"));
		ChessPed chessPedRook = getChessPedForElement(chessSetElement->FirstChildElement("Rook"));
		ChessPed chessPedKnight = getChessPedForElement(chessSetElement->FirstChildElement("Knight"));
		ChessPed chessPedBishop = getChessPedForElement(chessSetElement->FirstChildElement("Bishop"));
		ChessPed chessPedQueen = getChessPedForElement(chessSetElement->FirstChildElement("Queen"));
		ChessPed chessPedKing = getChessPedForElement(chessSetElement->FirstChildElement("King"));

		std::array<std::string, 3> pawnWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("Pawn"));
		std::array<std::string, 3> rookWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("Rook"));
		std::array<std::string, 3> knightWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("Knight"));
		std::array<std::string, 3> bishopWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("Bishop"));
		std::array<std::string, 3> queenWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("Queen"));
		std::array<std::string, 3> kingWeapons = getChessWeaponsForElement(chessSetElement->FirstChildElement("King"));

		std::array<ChessPiece*, 16>  pieces;
		int index = 0;
		for (int i = 0; i < 8; i++) {
			pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::PAWN, chessPedPawn, Vector3(), 1.0, pawnWeapons[0], pawnWeapons[1], pawnWeapons[2]);
		}
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::ROOK, chessPedRook, Vector3(), 1.0, rookWeapons[0], rookWeapons[1], rookWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::KNIGHT, chessPedKnight, Vector3(), 1.0, knightWeapons[0], knightWeapons[1], knightWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::BISHOP, chessPedBishop, Vector3(), 1.0, bishopWeapons[0], bishopWeapons[1], bishopWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::QUEEN, chessPedQueen, Vector3(), 1.0, queenWeapons[0], queenWeapons[1], queenWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::KING, chessPedKing, Vector3(), 1.0, kingWeapons[0], kingWeapons[1], kingWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::BISHOP, chessPedBishop, Vector3(), 1.0, bishopWeapons[0], bishopWeapons[1], bishopWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::KNIGHT, chessPedKnight, Vector3(), 1.0, knightWeapons[0], knightWeapons[1], knightWeapons[2]);
		pieces[index++] = new ChessPiece(ChessSide::WHITE, ChessPiece::ROOK, chessPedRook, Vector3(), 1.0, rookWeapons[0], rookWeapons[1], rookWeapons[2]);

		ChessSet* chessSet = new ChessSet(chessSetName, pieces);

		if (isDefaultWhite) {
			mDefaultWhiteChessSet = chessSet;
		}
		else if (isDefaultBlack) {
			mDefaultBlackChessSet = chessSet;
		}

		mChessSets.push_back(chessSet);
	}

}

ChessPed ChessSetFactory::getChessPedForElement(tinyxml2::XMLElement * chessPieceElement)
{

	std::array<int, 12> drawableVariations;
	std::array<int, 12> textureVariation; 
	std::array<int, 12> paletteVariation; 
	std::array<int, 3> propVariation; 
	std::array<int, 3> propTextureVariation;


	const char* strPedModelHash = chessPieceElement->Attribute("pedModelHash");
	DWORD pedModelHash = strtoul(strPedModelHash, NULL, 0);

	
	const char* constMovementStyle = chessPieceElement->Attribute("movementStyle");
	char* movementStyle = strdup(constMovementStyle);
	//set variant of ped
	for (int i = 0; i < 12; i++) {
		const char* strDrawableVar = chessPieceElement->Attribute(("drawableVariation" + std::to_string(i)).c_str());
		const char* strTextureVar = chessPieceElement->Attribute(("textureVariation" + std::to_string(i)).c_str());
		const char* strPaletteVar = chessPieceElement->Attribute(("paletteVariation" + std::to_string(i)).c_str());

		int drawableVar = std::stoi(strDrawableVar);
		int textureVar = std::stoi(strTextureVar);
		int paletteVar = std::stoi(strPaletteVar);
		drawableVariations[i] = drawableVar;
		textureVariation[i] = textureVar;
		paletteVariation[i] = paletteVar;
	}

	//get props
	for (int i = 0; i <= 2; i++) {
		const char* strPropVar = chessPieceElement->Attribute(("propVariation" + std::to_string(i)).c_str());
		const char* strPropTextureVar = chessPieceElement->Attribute(("propTextureVariation" + std::to_string(i)).c_str());

		int propVar = std::stoi(strPropVar);
		int propTextureVar = std::stoi(strPropTextureVar);
		propVariation[i] = propVar;
		propTextureVariation[i] = propTextureVar;
	}

	return ChessPed(pedModelHash, movementStyle, drawableVariations,textureVariation, paletteVariation, propVariation, propTextureVariation);
}

std::array<std::string, 3> ChessSetFactory::getChessWeaponsForElement(tinyxml2::XMLElement * chessPieceElement)
{
	std::array<std::string, 3> weapons;
	weapons[0] = chessPieceElement->Attribute("primaryWeapon");
	weapons[1] = chessPieceElement->Attribute("secondaryWeapon");
	weapons[2] = chessPieceElement->Attribute("meleeWeapon"); 
	return weapons;
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
	pieces[index++] = new ChessPiece(side, ChessPiece::KNIGHT, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::BISHOP, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::QUEEN, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::KING, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::BISHOP, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::KNIGHT, ChessPed(), Vector3(), 1.0);
	pieces[index++] = new ChessPiece(side, ChessPiece::ROOK, ChessPed(), Vector3(), 1.0);

	ChessSet* chessSet = new ChessSet("Simple Set", pieces);
	return chessSet;
}
