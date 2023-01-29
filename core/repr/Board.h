/**
 * This Board class will store a series of piece-centric piece_bitboards to be used for move generation, attack & defend maps,
 * and other move generation tasks
 * */

#ifndef CE2023VELA_BOARD_H
#define CE2023VELA_BOARD_H

// Inclusions
#include <cstdint>
#include <cstring>
#include <bitset>
#include <stack>
#include <map>

// A recommendation from https://www.chessprogramming.org/Bitboards
typedef uint64_t U64; // supported by MSC 13.00+ and C99 (uint64_t guarantees 64 bits)
#define C64(constantU64) constantU64##ULL

#include "MoveGeneration.h" // waited until after typedef U64

// Constants
static char CLASSIC_FEN[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// An enum of squares with their names in Little Endian Rank File (LERF)
// Thanks again to https://www.chessprogramming.org/Square_Mapping_Considerations
enum [[maybe_unused]] e_Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

// An enum of piece types to match to piece_bitboards
enum e_Piece_Types {
    white_pawn, black_pawn, white_knight, black_knight, white_bishop, black_bishop,
    white_rook, black_rook, white_queen, black_queen, white_king, black_king,
};

// An index of file mappings and rank mappings
const char file_names[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
const char rank_names[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

// For outputting moves in simple form ([from_square][to_square][promotion_type])
const char promotion_shorthand[] = {
        '?', 'n', 'b', '?',
        'r', '?', '?', '?',
        'q', '?', '?', '?',
        '?', '?', '?', '?',
        };

// Piece and color types
const int NOTHING = 0b0000;
const int WHITE = 0b0000;
const int BLACK = 0b1000;
const int PAWN = 0b0001;
const int KNIGHT = 0b0010;
const int BISHOP = 0b0011;
const int ROOK = 0b0100;
const int QUEEN = 0b0101;
const int KING = 0b0110;

/**
 * Board class should contain a list of piece_bitboards
 * All piece_bitboards will use arithmetic order with bit index 0 as the LSB
 * All piece_bitboards will be mapped using Little Endian Rank File mapping (LERF)
 * The principal objective of the piece_bitboards is to store piece locations
 * */
class Board {
private:

    // Piece piece_bitboards by color
    U64 white_pieces = 0;
    U64 black_pieces = 0;

    // White pieces by type
    U64 white_pawns = 0;
    U64 white_knights = 0;
    U64 white_bishops = 0;
    U64 white_rooks = 0;
    U64 white_queens = 0;
    U64 white_kings = 0;

    // Black pieces by type
    U64 black_pawns = 0;
    U64 black_knights = 0;
    U64 black_bishops = 0;
    U64 black_rooks = 0;
    U64 black_queens = 0;
    U64 black_kings = 0;

    // Move history on the board
    std::deque<struct s_Move*> move_history = std::deque<struct s_Move*>();

    // Piece types to bitboard
    static const int NUM_BITBOARDS = 16;
    U64 *piece_bitboards[NUM_BITBOARDS] = {
            &white_pieces,
            &white_pawns,
            &white_knights,
            &white_bishops,
            &white_rooks,
            &white_queens,
            &white_kings,
            nullptr,
            &black_pieces,
            &black_pawns,
            &black_knights,
            &black_bishops,
            &black_rooks,
            &black_queens,
            &black_kings,
            nullptr,
    };

    // Fen piece notation to the piece integer they represent
    // std::maps are effectively lists of std::pairs which have a first and second field
    std::map<char, int> FEN_piece_type {
            {'P', WHITE | PAWN}, {'p', BLACK | PAWN},
            {'N', WHITE | KNIGHT}, {'n', BLACK | KNIGHT},
            {'B', WHITE | BISHOP}, {'b', BLACK | BISHOP},
            {'R', WHITE | ROOK}, {'r', BLACK | ROOK},
            {'Q', WHITE | QUEEN}, {'q', BLACK | QUEEN},
            {'K', WHITE | KING}, {'k', BLACK | KING},
    };

    // Integer piece representations to the fen shorthand (for generating FEN and printing boards etc.)
    std::map<int, char> piece_shorthand {
            {WHITE | PAWN, 'P'}, {BLACK | PAWN, 'p'},
            {WHITE | KNIGHT, 'N'}, {BLACK | KNIGHT, 'n'},
            {WHITE | BISHOP, 'B'}, {BLACK | BISHOP, 'b'},
            {WHITE | ROOK, 'R'}, {BLACK | ROOK, 'r'},
            {WHITE | QUEEN, 'Q'}, {BLACK | QUEEN, 'q'},
            {WHITE | KING, 'K'}, {BLACK | KING, 'k'},
    };

public:

    /**
     * PrintBoard loops over the squares of the board to print a character that corresponds to the piece that is on that
     * square. This is the correct output for the command "d" in Universal Chess Interface (UCI)
     */
    void PrintBoard();

    /**
     * PrintMoveSimple uses the current board as context to output the passed move in modern algebraic notation
     * @param move is a pointer to the move that is to be contextualized and printed
     * */
    static void PrintMoveSimple(struct s_Move *move);

    /**
     * PrintMove uses the current board as context to output the passed move in modern algebraic notation
     * @param move is a pointer to the move that is to be contextualized and printed
     * */
    void PrintMove(struct s_Move *move);

    /**
     * LoadFEN looks at a Forsyth Edwards Notation (FEN) string to construct the piece piece_bitboards
     * @cite https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * @param fenString the FEN string to use
     * */
    void LoadFEN(char *fenString);

    /**
     * LoadFEN looks at a Forsyth Edwards Notation (FEN) string to build a board and fill in values for it from the
     * information gathered
     * @cite https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     * @param fenString the FEN string to use
     * */
    static Board *FromFEN(char *fenString);

    /**
     * PieceAt looks at all of the bitboards to determine which one has the piece at the square. It returns a reference
     * to the piece int that was found (-1 if nothing could be found)
     */
    int PieceAt(U64 bitboard_pos);

    /**
     * Empty square ensures that no bitboard registers a piece on a certain square, deleting all references to that
     * piece if it happens to exist
     * @param square_bitboard with a one in every space which is to be set
     */
    void EmptySquare(U64 square_bitboard);

    /**
     * SetPiece adjusts all piece_bitboards in order to put a piece on a certain square
     * @param square_bitboard with a one in every space which is to be set
     * @param piece_to_set is the number of the piece that is to be put there instead
     */
    void SetPiece(U64 square_bitboard, int piece_to_set);

    /**
     * MakeMove is the function that takes a move struct and manipulates the piece_bitboards and move history stack
     * @param move is the move to be played
     */
    void MakeMove(struct s_Move *move);

    /**
     * UnmakeLastMove is the function that unmakes the move that was last played. It takes no parameters because it can
     * only unmake the last move
     * TODO: Create a function that can undo any move that
     */
    void UnmakeLastMove();

    /**
     * PrintMoveHistory reads out the moves from the stack that contains the previously played moves. It reads them in
     * the order they were played and doesn't read any variations that were pushed at any point.
     */
    void PrintMoveHistory();
};

#endif //CE2023VELA_BOARD_H
