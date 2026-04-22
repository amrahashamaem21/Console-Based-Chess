# Console-Based-Chess
This project is a fully functional console-based Chess game written in C++. It supports two-player game (White vs Black) with most standard chess rules implemented
Features

All standard piece movements (pawn, rook, knight, bishop, queen, king)
En Passant — valid for exactly one turn after an opponent's double pawn advance
Pawn Promotion — prompts for piece choice on reaching the back rank (Q/R/B/N), defaults to Queen
Check detection with full pin validation — moves that leave your own king in check are rejected
Checkmate detection — searches all legal moves and declares the winner

Not implemented: Castling and stalemate detection.

How to Play
Enter moves as two space-separated algebraic coordinates — source square then destination square.
e2 e4       # Move pawn forward
g1 f3       # Develop a knight
quit        # Exit the game

The board is printed before every turn. White pieces are uppercase (P R N B Q K), black pieces are lowercase (p r n b q k), and empty squares are shown as .

Piece Reference
Piece	White	Black
Pawn	P	p
Rook	R	r
Knight	N	n
Bishop	B	b
Queen	Q	q
King	K	k

Piece Movement Logic

Each chess piece has its own movement validation function:

Piece	Function Used
Pawn	isValidPawnMove()
Rook	isValidRookMove()
Bishop	isValidBishopMove()
Knight	isValidKnightMove()
Queen	isValidQueenMove()
King	isValidKingMove()

Pawn Special Rules
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

Move Validation System
The main validation function:
isValidMove()
It checks:

Piece exists
Correct player's turn
Destination validity
Piece movement rules
King safety (very important)

Check Detection
Function:
isKingInCheck()
Finds king position
Checks if any opponent piece can attack that square

Checkmate Detection
Function:
isCheckmate()
Logic:
King must already be in check
Try all possible moves
If no move removes check → CHECKMATE
