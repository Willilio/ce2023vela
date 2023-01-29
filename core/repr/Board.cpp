//
// Created by wcirw on 1/17/2023.
//

// Includes
#include <iostream>
#include <cmath>
#include "../util/str.h"
#include "Board.h"
#include "MoveGeneration.h"


// Define LoadFEN function (description in header)
// This is totally crying out for optimization !!!!!
void Board::LoadFEN(char *fenString) {

    // Split the fen string - should have six parts if done correctly
    auto **fen_parts = util::str::split(fenString);

    // Let's start creating the board by initializing the piece piece_bitboards with the fen piece info
    char *piece_setup = fen_parts[0];
    int char_pos = 0, board_pos = 0;
    // We loop through the characters of the piece setup string
    while (piece_setup[char_pos] != '\0') {
        int converted_board_pos = 56 - (board_pos / 8) * 8 + board_pos % 8;
        U64 bitboard_pos = (U64) pow(2, converted_board_pos);
        if (piece_setup[char_pos] != '/') {
            // We assume here that the fen has been correctly passed to the function
            int diff_zero = piece_setup[char_pos] - '0';
            if (diff_zero <= 1 || diff_zero > 8) {
                *piece_bitboards[FEN_piece_type[piece_setup[char_pos]]] |= bitboard_pos;
                board_pos++;
            } else {
                board_pos += diff_zero;
            }
        }
        char_pos++;
    }

    // Create the color piece_bitboards
    white_pieces = white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_kings;
    black_pieces = black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_kings;

    // Free the memory from the fen_parts string
    delete[] fen_parts;

}

// From fen is a board factory function with fen strings
Board *Board::FromFEN(char *fenString) {
    auto *newBoard = new Board();
    newBoard->LoadFEN(fenString);
    return newBoard;
}

void Board::PrintBoard() {
    const char *line_break = "\t+---+---+---+---+---+---+---+---+\n\t| ";
    for (int i=0; i<64; i++) {
        int converted_board_pos = 56 - (i / 8) * 8 + i % 8;
        U64 bitboard_pos = pow(2, converted_board_pos);
        if (i % 8 == 0) {
            std::cout << '\n' << line_break;
        }
        int piece_at_square = Board::PieceAt(bitboard_pos);
        if (piece_at_square != -1) {
            std::cout << piece_shorthand[piece_at_square];
        } else {
            std::cout << " ";
        }
        std::cout << " | ";
        if (i % 8 == 7) {
            std::cout << (71 - i) / 8;
        }
    }
    std::cout << '\n' << "\t+---+---+---+---+---+---+---+---+\n\t  a   b   c   d   e   f   g   h\n" << std::endl;
}

void Board::PrintMove (struct s_Move *move) {
    std::cout << "Print Move is under construction *building noises* lol" << std::endl;
}

void Board::PrintMoveSimple(struct s_Move *move) {
    auto origin_index = (short) log2((double) move->origin);
    auto dest_index = (short) log2((double) move->dest);
    bool is_promotion = (move->flags&PROMOTION_MASK) > 0;
    std::cout << file_names[origin_index%8] << rank_names[origin_index>>3];
    std::cout << file_names[dest_index%8] << rank_names[dest_index>>3];
    if (is_promotion) {
        std::cout << promotion_shorthand[(move->flags&PROMOTION_MASK)];
    }
    std::cout << std::endl;
}

void Board::PrintMoveHistory() {
    std::cout << "Printing move history..." << std::endl;
    for (auto it = move_history.cbegin(); it != move_history.cend(); ++it) {
        Board::PrintMoveSimple(*it);
    }
    std::cout << std::endl;
}

int Board::PieceAt(U64 bitboard_pos) {
    for (int bitboard_no=0; bitboard_no<NUM_BITBOARDS; bitboard_no++) {
        // This condition skips the generic white and black pieces piece_bitboards
        if (bitboard_no % 8 == 0) { continue; }
        U64 *bitboard = piece_bitboards[bitboard_no];
        if (bitboard != nullptr) {
            if ((*bitboard & bitboard_pos) == bitboard_pos) {
                return bitboard_no;
            }
        }
    }
    return -1;
}

void Board::EmptySquare(U64 square_bitboard) {
    U64 opp_bitboard = ~square_bitboard;
    for (auto bitboard : piece_bitboards) {
        if (bitboard != nullptr) {
            *bitboard &= opp_bitboard;
        }
    }
}

void Board::SetPiece(U64 square_bitboard, int piece_to_set) {

}

void Board::MakeMove(struct s_Move *move) {

    // set the piece at the new square to be the piece that was at the origin square
    // modify the move flags accordingly to let the move be correctly undone
    int piece_type = PieceAt(move->origin);
    int color_type = (piece_type / 8) * 8; // 0 for white, 8 for black
    U64 *pieceBitboard = piece_bitboards[piece_type];
    U64 *colorBitboard = piece_bitboards[color_type];
    *pieceBitboard |= move->dest;
    *colorBitboard |= move->dest;

    // replace the piece at the origin square with nothing
    EmptySquare(move->origin);

    // add the move to the move history stack
    move_history.push_back(move);
}

void Board::UnmakeLastMove() {

    // get the last move from the stack
    struct s_Move *last_move = move_history.back();
    move_history.pop_back();
}
