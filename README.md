# Console-Based-Chess
This project is a fully functional console-based Chess game written in C++. It supports two-player game (White vs Black) with most standard chess rules implemented
It includes:
including:

Valid move checking for all pieces
Check and checkmate detection
Pawn promotion
En Passant (advanced pawn rule)
Turn-based gameplay

The game runs entirely in the terminal and uses simple text-based board visualization.
THE WORKING OF GAME:
The game starts with the standard chess setup.

Players take turns entering moves in algebraic format:

e2 e4
First input = source square
Second input = destination square
To exit the game, We type quit
Piece Movement Logic

1.Each chess piece has its own movement validation function:

Piece	Function Used
Pawn	isValidPawnMove()
Rook	isValidRookMove()
Bishop	isValidBishopMove()
Knight	isValidKnightMove()
Queen	isValidQueenMove()
King	isValidKingMove()

These functions ensure:

Correct movement rules
No illegal jumps (except knight)
No capturing same-color pieces

2.  Pawn Special Rules
3.  Move Validation System
4.  
✔ Normal Movement
1 step forward
2 steps from starting position

✔ Diagonal Capture
Captures opponent pieces diagonally

✔ En Passant (Advanced Rule)
Triggered when an opponent pawn moves 2 squares forward
Must be captured immediately on next move
Implemented using global tracking variables:
lastPawnMoveStartR
lastPawnMoveEndR
lastPawnMoveEndC

✔ Promotion
When pawn reaches last rank
Player chooses: Queen, Rook, Bishop, or Knight
Default = Queen
4.  Move Validation System

The main validation function:
isValidMove()
It checks:

Piece exists
Correct player's turn
Destination validity
Piece movement rules
King safety (very important)
4.  King Safety (Anti-Self-Check Logic)

Before allowing any move:

The move is simulated temporarily
System checks if own king becomes attacked
If yes → move is rejected

This prevents illegal moves like moving pinned pieces.

5.  Check Detection

Function:
isKingInCheck()
Finds king position
Checks if any opponent piece can attack that square
6.  Checkmate Detection

Function:
isCheckmate()
Logic:
King must already be in check
Try all possible moves
If no move removes check → CHECKMATE

7.  Board Representation
8×8 array:
char board[8][8]
Piece Encoding
Piece	White	Black
Pawn	P	p
Rook	R	r
Knight	N	n
Bishop	B	b
Queen	Q	q
King	K	k
Code Structure
Main Components
initializeBoard() → sets up starting position
printBoard() → displays board
isValidMove() → central move validation
makeMove() → executes move + handles special rules
isKingInCheck() → check detection
isCheckmate() → checkmate detection
