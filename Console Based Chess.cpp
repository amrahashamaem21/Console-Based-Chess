#include <iostream>
#include <cmath>
#include <string>
using namespace std;
const int SIZE = 8;
char board[SIZE][SIZE];
bool whiteTurn = true;
bool gameOver = false;
int lastPawnMoveStartR = -1;
int lastPawnMoveEndR = -1;
int lastPawnMoveEndC = -1;
void initializeBoard();
void printBoard();
bool isValidMove(int sx, int sy, int dx, int dy, bool ignoreCheckRule = false);
bool isValidPawnMove(int sx, int sy, int dx, int dy);
bool isValidBishopMove(int sx, int sy, int dx, int dy);
bool isValidRookMove(int sx, int sy, int dx, int dy);
bool isValidKnightMove(int sx, int sy, int dx, int dy);
bool isValidQueenMove(int sx, int sy, int dx, int dy);
bool isValidKingMove(int sx, int sy, int dx, int dy);
bool isPathClear(int sx, int sy, int dx, int dy);
void makeMove(int sx, int sy, int dx, int dy);
void findKing(bool isWhiteKing, int& kingR, int& kingC);
bool isSquareAttacked(int targetR, int targetC, bool byWhite);
bool isKingInCheck(bool isWhiteKing);
bool isCheckmate(bool isWhiteKing);
bool algebraicToCoords(const string& alg, int& row, int& col);
string getPieceName(char piece);
bool isInsideBoard(int r, int c) {
    return r >= 0 && r < SIZE && c >= 0 && c < SIZE;
}
int main() {
    initializeBoard();
    while (!gameOver) {
        printBoard();
        cout << "\n" << (whiteTurn ? "White" : "Black") << "'s turn:  ";
        string from, to;
        if (!(cin >> from)) break;
        if (from == "quit") break;
        if (!(cin >> to)) break;

        int fromR, fromC, toR, toC;
        if (!algebraicToCoords(from, fromR, fromC) || !algebraicToCoords(to, toR, toC)) {
            cout << "Invalid input format. Use e.g. e2 e4.\n";
            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); }
            continue;
        }
        if (!isInsideBoard(fromR, fromC) || !isInsideBoard(toR, toC)) {
            cout << "Coordinates out of range.\n";
            continue;
        }
        if (isValidMove(fromR, fromC, toR, toC, false)) {
            makeMove(fromR, fromC, toR, toC);
            if (gameOver) break;
            whiteTurn = !whiteTurn;
            cout << "Move executed successfully.\n";
        }
        else {
            cout << "Invalid move. Please try again.\n";
        }
    }

    cout << "Game ended.\n";
    return 0;
}
bool algebraicToCoords(const string& alg, int& row, int& col) {
    if (alg.length() != 2) return false;
    char fileChar = alg[0];
    if (fileChar >= 'A' && fileChar <= 'H') {
        fileChar = fileChar - ('A' - 'a');
    }
    if (fileChar < 'a' || fileChar > 'h') return false;
    col = fileChar - 'a';
    char rankChar = alg[1];
    if (rankChar < '1' || rankChar > '8') return false;
    row = SIZE - (rankChar - '0');
    return true;
}
string getPieceName(char piece) {
    bool isWhite = (piece >= 'A' && piece <= 'Z');
    char type = isWhite ? piece + 'a' - 'A' : piece;
    string name = isWhite ? "White " : "Black ";
    switch (type) {
    case 'p': return name + "Pawn";
    case 'r': return name + "Rook";
    case 'n': return name + "Knight";
    case 'b': return name + "Bishop";
    case 'q': return name + "Queen";
    case 'k': return name + "King";
    default: return "Unknown Piece";
    }
}
void initializeBoard() {
    for (int i = 2; i < SIZE - 2; ++i)
        for (int j = 0; j < SIZE; ++j) board[i][j] = ' ';

    board[0][0] = 'r'; board[0][1] = 'n'; board[0][2] = 'b'; board[0][3] = 'q'; board[0][4] = 'k'; board[0][5] = 'b'; board[0][6] = 'n'; board[0][7] = 'r';
    for (int j = 0; j < SIZE; j++) board[1][j] = 'p';

    for (int j = 0; j < SIZE; j++) board[6][j] = 'P';
    board[7][0] = 'R'; board[7][1] = 'N'; board[7][2] = 'B'; board[7][3] = 'Q'; board[7][4] = 'K'; board[7][5] = 'B'; board[7][6] = 'N'; board[7][7] = 'R';
}
void printBoard() {
    cout << "\n    a b c d e f g h\n";
    cout << "  -------------------\n";
    for (int i = 0; i < SIZE; ++i) {
        cout << SIZE - i << " |";
        for (int j = 0; j < SIZE; ++j)
            cout << " " << (board[i][j] == ' ' ? '.' : board[i][j]);
        cout << " |\n";
    }
    cout << "  -------------------\n";
}

bool isPathClear(int sr, int sc, int tr, int tc) {
    int dr = (tr > sr) ? 1 : (tr < sr ? -1 : 0);
    int dc = (tc > sc) ? 1 : (tc < sc ? -1 : 0);
    int r = sr + dr, c = sc + dc;
    while (r != tr || c != tc) {
        if (board[r][c] != ' ') return false;
        r += dr; c += dc;
    }
    return true;
}
bool isValidPawnMove(int sx, int sy, int dx, int dy) {
    char piece = board[sx][sy];
    bool white = (piece >= 'A' && piece <= 'Z');
    int dir = white ? -1 : 1;
    int start = white ? 6 : 1;

    int dR = dx - sx;
    int dC = dy - sy;
    if (dC == 0) {
        if (board[dx][dy] != ' ') return false; 
        if (dR == dir) return true;
        if (sx == start && dR == 2 * dir && board[sx + dir][sy] == ' ')
            return true;

        return false;
    }
    if (abs(dC) == 1 && dR == dir) {
        char target = board[dx][dy];
        if (target != ' ') {
            bool targetWhite = (target >= 'A' && target <= 'Z');
            return white != targetWhite;
        }
        if (target == ' ') {
            if (dx == lastPawnMoveEndR && dy == lastPawnMoveEndC) {
                char victim = board[dx - dir][dy];
                if (victim != ' ' && (victim == (white ? 'p' : 'P')) &&
                    abs(lastPawnMoveEndR - lastPawnMoveStartR) == 2) {
                    return true;
                }
            }
        }
        return false;
    }

    return false;
}
bool isValidBishopMove(int sx, int sy, int dx, int dy) {
    char piece = board[sx][sy], dest = board[dx][dy];
    bool isWhite = (piece >= 'A' && piece <= 'Z'), destIsWhite = (dest >= 'A' && dest <= 'Z');
    if (abs(dx - sx) != abs(dy - sy) || abs(dx - sx) == 0) return false;
    if (!isPathClear(sx, sy, dx, dy)) return false;
    if (dest != ' ' && isWhite == destIsWhite) return false;
    return true;
}
bool isValidRookMove(int sx, int sy, int dx, int dy) {
    char piece = board[sx][sy], dest = board[dx][dy];
    bool isWhite = (piece >= 'A' && piece <= 'Z'), destIsWhite = (dest >= 'A' && dest <= 'Z');
    if (sx != dx && sy != dy) return false;
    if (!isPathClear(sx, sy, dx, dy)) return false;
    if (dest != ' ' && isWhite == destIsWhite) return false;
    return true;
}
bool isValidKnightMove(int sx, int sy, int dx, int dy) {
    int dr = abs(dx - sx), dc = abs(dy - sy);
    if (!((dr == 1 && dc == 2) || (dr == 2 && dc == 1))) return false;
    char piece = board[sx][sy], dest = board[dx][dy];
    bool isWhite = (piece >= 'A' && piece <= 'Z'), destIsWhite = (dest >= 'A' && dest <= 'Z');
    if (dest != ' ' && isWhite == destIsWhite) return false;
    return true;
}
bool isValidQueenMove(int sx, int sy, int dx, int dy) {
    int dr = abs(dx - sx), dc = abs(dy - sy);
    if (dr == dc && dr > 0) return isValidBishopMove(sx, sy, dx, dy);
    if (sx == dx || sy == dy) return isValidRookMove(sx, sy, dx, dy);
    return false;
}
bool isValidKingMove(int sx, int sy, int dx, int dy) {
    int dr = abs(dx - sx), dc = abs(dy - sy);
    if (dr > 1 || dc > 1) return false;
    char piece = board[sx][sy], dest = board[dx][dy];
    bool isWhite = (piece >= 'A' && piece <= 'Z'), destIsWhite = (dest >= 'A' && dest <= 'Z');
    if (dest != ' ' && isWhite == destIsWhite) return false;
    return true;
}
void findKing(bool isWhiteKing, int& kr, int& kc) {
    char king = isWhiteKing ? 'K' : 'k';
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) if (board[r][c] == king) { kr = r; kc = c; return; }
    kr = -1; kc = -1;
}
bool canPieceAttackSquare(int sr, int sc, int tr, int tc) {
    char p = board[sr][sc]; if (p == ' ') return false;
    int dr = tr - sr, dc = tc - sc;
    char type = (p >= 'A' && p <= 'Z') ? p + 'a' - 'A' : p;
    if (type == 'n') return (abs(dr) == 1 && abs(dc) == 2) || (abs(dr) == 2 && abs(dc) == 1);
    if (type == 'k') return abs(dr) <= 1 && abs(dc) <= 1;
    if (type == 'p') return ((p == 'P' && dr == -1 && abs(dc) == 1) || (p == 'p' && dr == 1 && abs(dc) == 1));
    if ((type == 'b' || type == 'q') && abs(dr) == abs(dc)) return isPathClear(sr, sc, tr, tc);
    if ((type == 'r' || type == 'q') && (sr == tr || sc == tc)) return isPathClear(sr, sc, tr, tc);
    return false;
}
bool isSquareAttacked(int tr, int tc, bool byWhite) {
    for (int r = 0; r < 8; r++) for (int c = 0; c < 8; c++) {
        char piece = board[r][c]; if (piece == ' ') continue;
        bool pieceIsWhite = (piece >= 'A' && piece <= 'Z');
        if (pieceIsWhite == byWhite && canPieceAttackSquare(r, c, tr, tc)) return true;
    }
    return false;
}
bool isKingInCheck(bool isWhiteKing) {
    int kr, kc; findKing(isWhiteKing, kr, kc);
    if (kr == -1) return false;
    return isSquareAttacked(kr, kc, !isWhiteKing);
}
bool isCheckmate(bool isWhiteKing) {
    if (!isKingInCheck(isWhiteKing)) return false;
    bool originalWhiteTurn = whiteTurn;
    whiteTurn = isWhiteKing;
    bool foundLegalMove = false;
    for (int sr = 0; sr < 8; sr++) {
        for (int sc = 0; sc < 8; sc++) {
            char piece = board[sr][sc];
            if (piece == ' ') continue;
            bool pieceIsWhite = (piece >= 'A' && piece <= 'Z');
            if (pieceIsWhite != isWhiteKing) continue; 

            for (int dr = 0; dr < 8; dr++) {
                for (int dc = 0; dc < 8; dc++) {
                    if (isValidMove(sr, sc, dr, dc, false)) {
                        foundLegalMove = true;
                        goto restore_turn;
                    }
                }
            }
        }
    }

restore_turn:;
    whiteTurn = originalWhiteTurn;
    return !foundLegalMove;
}
bool isValidMove(int sx, int sy, int dx, int dy, bool ignoreCheckRule) {
    char piece = board[sx][sy], target = board[dx][dy];
    if (piece == ' ') { return false; }
    bool isWhite = (piece >= 'A' && piece <= 'Z');
    if (!ignoreCheckRule && whiteTurn != isWhite) {
        return false;
    }
    if (sx == dx && sy == dy) { return false; }
    if (target != ' ' && ((target >= 'A' && target <= 'Z') == isWhite)) { return false; }
    char type = isWhite ? piece + 'a' - 'A' : piece;
    bool ok = false;
    if (type == 'p') ok = isValidPawnMove(sx, sy, dx, dy);
    else if (type == 'b') ok = isValidBishopMove(sx, sy, dx, dy);
    else if (type == 'r') ok = isValidRookMove(sx, sy, dx, dy);
    else if (type == 'n') ok = isValidKnightMove(sx, sy, dx, dy);
    else if (type == 'q') ok = isValidQueenMove(sx, sy, dx, dy);
    else if (type == 'k') ok = isValidKingMove(sx, sy, dx, dy);
    if (!ok) { return false; }
    if (!ignoreCheckRule) {
        char backupSrc = board[sx][sy], backupDst = board[dx][dy];
        bool isEnPassant = false;
        char backupEPVictim = ' ';
        int victimR = -1;
        if (type == 'p' && target == ' ' && abs(sy - dy) == 1 && abs(sx - dx) == 1) {
            if (dx == lastPawnMoveEndR && dy == lastPawnMoveEndC) {
                isEnPassant = true;
                int dir = isWhite ? -1 : 1;
                victimR = dx - dir;
            }
        }
        if (isEnPassant) {
            backupEPVictim = board[victimR][dy]; 
            board[victimR][dy] = ' ';          
        }
        board[dx][dy] = board[sx][sy];
        board[sx][sy] = ' ';
        bool leavesCheck = isKingInCheck(isWhite);
        board[sx][sy] = backupSrc;
        board[dx][dy] = backupDst;
        if (isEnPassant) {
            board[victimR][dy] = backupEPVictim; 
        }
        if (leavesCheck) {
            return false;
        }
    }

    return true;
}
void makeMove(int sx, int sy, int dx, int dy)
{
    char movingPiece = board[sx][sy];
    char capturedPiece = board[dx][dy];
    char type = (movingPiece >= 'A' && movingPiece <= 'Z') ? movingPiece + 'a' - 'A' : movingPiece;
    bool isWhite = (movingPiece >= 'A' && movingPiece <= 'Z');
    lastPawnMoveStartR = -1;
    lastPawnMoveEndR = -1;
    lastPawnMoveEndC = -1;
    bool isEnPassant = false;
    if (type == 'p' && capturedPiece == ' ' && abs(sy - dy) == 1 && abs(sx - dx) == 1) {
        isEnPassant = true;
    }
    board[dx][dy] = movingPiece;
    board[sx][sy] = ' ';
    if (capturedPiece != ' ') {
        cout << "Captured " << getPieceName(capturedPiece) << " at "
            << char('a' + dy) << (8 - dx) << "!\n";
    }

    if (isEnPassant) {
        int victimR = isWhite ? dx + 1 : dx - 1;
        capturedPiece = board[victimR][dy];
        board[victimR][dy] = ' ';
        cout << "Captured " << getPieceName(capturedPiece) << " at "
            << char('a' + dy) << (8 - victimR) << " via En Passant!\n";
    }
    if (type == 'p' && (dx == 0 || dx == 7)) {
        char newPiece = 'Q'; 
        string choice;
        cout << "Pawn promotion! Choose (Q/R/B/N): ";
        cin.clear();
        cin.ignore(10000, '\n');
        getline(cin, choice);

        if (!choice.empty()) {
            char c = toupper(choice[0]);
            if (c == 'Q' || c == 'R' || c == 'B' || c == 'N') {
                newPiece = c;
            }
        }
        board[dx][dy] = isWhite ? newPiece : tolower(newPiece);
        cout << getPieceName(movingPiece) << " promoted to " << getPieceName(board[dx][dy]) << "!\n";
    }
    if (type == 'p' && abs(sx - dx) == 2) {
        lastPawnMoveStartR = sx;
        lastPawnMoveEndR = dx;
        lastPawnMoveEndC = dy;
    }
    bool opponentIsWhite = !isWhite;  
    if (isKingInCheck(opponentIsWhite)) {
        if (isCheckmate(opponentIsWhite)) {
            cout << "Checkmate! " << (isWhite ? "White" : "Black") << " wins!\n";
            gameOver = true;
        }
        else {
            cout << (opponentIsWhite ? "White" : "Black") << " is in check!\n";
        }
    }
}