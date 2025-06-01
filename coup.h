#ifndef COUP_H
#define COUP_H

#include <stdint.h>

typedef uint64_t bitboard;
typedef uint32_t coup;

typedef enum {
	P_VIDE = 0,
	P_PION = 1,
	P_CAVALIER = 2,
	P_FOU = 3,
	P_TOUR = 4,
	P_DAME = 5,
	P_ROI = 6
} COUP_piece_e;

typedef enum {
	C_NORMAL = 0,
	C_CAPTURE = 1,
	C_DOUBLE = 2,
	C_EN_PASSANT = 3,
	C_PROMO = 4,
	C_PROMO_CAPTURE = 5,
	C_PETIT_ROQUE = 6,
	C_GRAND_ROQUE = 7,
	C_NULL = 8
} COUP_type_e;


#define COUP_SOURCE(coup)	(coup & 0x3f)
#define COUP_DEST(coup)		((coup & 0xfc0) >> 6)
#define COUP_PIECE(coup)	((coup & 0x7000) >> 12)
#define COUP_PROMO(coup)	((coup & 0x38000) >> 15)
#define COUP_CAPT(coup)		((coup & 0x1c0000) >> 18)
#define COUP_TYPE(coup)		((coup & 0x1e00000) >> 21)

#define COUP(source, dest, piece, promo, capt, type) ((source & 0x3f) 	   \
												|    ((dest & 0x3f) << 6)  \
												| 	 ((piece & 0x7) << 12) \
												|    ((promo & 0x7) << 15) \
												|    ((capt & 0x7) << 18)  \
												|    ((type & 0xf) << 21))
typedef struct {
	struct {
		bitboard pions;
		bitboard cavaliers;
		bitboard fous;
		bitboard tours;
		bitboard dame;
		bitboard roi;
	} blancs;
	struct {
		bitboard pions;
		bitboard cavaliers;
		bitboard fous;
		bitboard tours;
		bitboard dame;
		bitboard roi;
	} noirs;
	bitboard pieces_noires;
	bitboard pieces_blanches;	
	int cote : 1;
} COUP_plateau_s;

#define B	0
#define N 	1

int lister_coups(COUP_plateau_s p, int* taille, coup* coups);
COUP_plateau_s appliquer_coup(coup, COUP_plateau_s, int); 
char* posToString(int a, char* res);

#endif /* COUP_H */
