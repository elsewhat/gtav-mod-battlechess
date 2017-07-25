# gtav-mod-battlechess
BattleChess for GTA V by elsewhat

Source available at: https://github.com/elsewhat/gtav-mod-battlewchess

Demo video: TBD

Installation: Requires Scripthook V. Copy BattleChess.asi into game directory



## Chess code overview
Initialize
1. Create ChessBoard
- Initialize ChessBoardSquares 
2. ChessSetFactory getDefaultWhiteChessSet and getDefaultBlackChessSet
3. ChessBoard.SetWhiteChessSet and setBlackChessSet
4. Place ChessPieces to initial ChessBoardSquare
5. Spawn peds


Move
1. Select square
2. Get ChessPiece
3. Generate possible moves
4. Select move
5. Execute ChessBattle
6. Update ChessPieces and ChessSquares
