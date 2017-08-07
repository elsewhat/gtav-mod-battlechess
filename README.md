# gtav-mod-battlechess
BattleChess for GTA V by elsewhat

Source available at: https://github.com/elsewhat/gtav-mod-battlechess

Demo video: TBD

Installation: Requires Scripthook V. Copy BattleChess.asi, Battlechess_anims.txt, Battlechess_sets.xml and Battlechess_synchedanims.xml into game directory

## Developer setup
1. Get GTA V (steam version preferred)
2. Install openIV http://openiv.com/
3. Install Scripthook V http://www.dev-c.com/gtav/scripthookv/
(SDK should be not be required as key files are in git repo)
4. Create empty file ScriptHookV.dev in GTA V game directory
C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto V
This allows you to press CTRL+R to unload the mod, deploy from Visual Studio and then CTRL+R to reload it again
5. Install Visual Studio (I'm using Visual Studio 2015 Community edition)
6. Clone git repo
7. Open BattleChess.sln in Visual Studio
8. Build project. This should copy Battlechess.asi to your game directory 
C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto V
9. Launch GTA V through OpenIV
10. Once in single player mode, press F7 to start Battlechess


## Chess code overview
Initialize
1. Create ChessBoard
- Initialize ChessBoardSquares 
2. ChessSetFactory 
- Read xml file
- Prepare getDefaultWhiteChessSet and getDefaultBlackChessSet
3. ChessBoard.SetWhiteChessSet and setBlackChessSet
4. Place ChessPieces to initial ChessBoardSquare
5. Spawn peds through ChessBoard->spawnChessPieces

Move
1. Select ChessBoardSquare
2. Generate possible ChessMoves
3. Select move
4. Execute ChessBoard->MakeMove
- Updates ChessBoardSquare and ChessPiece
5. Execute ChessBattle

```
Board overview
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
```
