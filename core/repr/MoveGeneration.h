//
// Created by wcirw on 1/23/2023.
//

#ifndef CE2023VELA_MOVEGENERATION_H
#define CE2023VELA_MOVEGENERATION_H

// Includes
#include <cmath>
#include "Board.h"

// Constants for the move flags
const U64 PROMOTION_MASK = 0b0000000000000000000000000000000000000000000000000000000000001111;
const U64 PROMOTION_KNIGHT = 0b0000000000000000000000000000000000000000000000000000000000000001;
const U64 PROMOTION_BISHOP = 0b0000000000000000000000000000000000000000000000000000000000000010;
const U64 PROMOTION_ROOK = 0b0000000000000000000000000000000000000000000000000000000000000100;
const U64 PROMOTION_QUEEN = 0b0000000000000000000000000000000000000000000000000000000000001000;
const U64 EN_PASSANT_MASK = 0b0000000000000000000000000000000000000000000000000000000000110000;
const U64 EN_PASSANT_FALSE = 0b0000000000000000000000000000000000000000000000000000000000010000;
const U64 EN_PASSANT_TRUE = 0b0000000000000000000000000000000000000000000000000000000000100000;

// A structure to hold a move
typedef struct s_Move {
    U64 origin = 0; // origin square in bitboard form
    U64 dest = 0; // destination square in bitboard form

    // TODO: rearrange the bits of these flags
    // Flags contains a few pieces of information (64 bits total)
    //   bits 1-4  : determines the promotion type
    //   bits 5&6  : if the move was en passant (01 = false, 10 = true, other = error/unknown)
    //   bits 7-12 : the piece that is captured
    U64 flags = 0;
} Move;

class MoveGeneration {
public:
    static Move *generate_move(int from, int to) {
        Move *genericMove = new Move;
        genericMove->origin = (U64) pow(2, from);
        genericMove->dest = (U64) pow(2, to);
        return genericMove;
    }
};


#endif //CE2023VELA_MOVEGENERATION_H
