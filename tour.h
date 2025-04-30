#ifndef TOUR_H
#define TOUR_H

#include "coup.h"


// 							0000000000000081
// Colonnes (fichiers)
static const bitboard COLS[8] = {
    0x8080808080808080ULL,  // File H
    0x4040404040404040ULL, // File G
    0x2020202020202020ULL, // File F
    0x1010101010101010ULL, // File E
    0x0808080808080808ULL, // File D
    0x0404040404040404ULL, // File C
    0x0202020202020202ULL, // File B
    0x0101010101010101ULL // File A
};

// Gestion des masques de ligne
static const bitboard MASQUE_LIG[8] = {0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00};

static inline bitboard TOUR_NORD(int x) {
	int col = x % 8, lig = x / 8;
	int shift = (lig + 1) * 8;
	
	if (shift >= 64) return 0;
	else return COLS[col] << shift;
}

static inline bitboard TOUR_SUD(int x) {
	int col = x % 8, lig = x / 8;
	int shift = (8 - lig) * 8;
	
	if (shift >= 64) return 0;
	else return COLS[col] >> shift;
}

static inline bitboard TOUR_EST(int x) {
	int col = x % 8, lig = x / 8;
	bitboard ligne = MASQUE_LIG[col];
	
	if (lig == 0) return ligne;
	else return ligne << (lig * 8);
}

static inline bitboard TOUR_OUEST(int x) {
	int col = x % 8, lig = x / 8;
	uint8_t l = ~(MASQUE_LIG[col] | (1ULL << (7 - col)));
	bitboard ligne = l;
	
	if (lig == 0) return ligne;
	else return ligne << (lig * 8);
}

#endif /* TOUR_H */

/*
 * cas 3 :
 * hori = 0xff
 *	res  = 0xe0
 * si 0 - 7 => >> 8 * 8	=> col 0
 * si 8 - 15 => >> 7 * 8
 */
