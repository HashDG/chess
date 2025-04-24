#ifndef TOUR_H
#define TOUR_H

#include "coup.h"


// 							0000000000000081
bitboard vert[8]  = {0x8080808080808080, 0x4040404040404040, 0x2020202020202020, 0x1010101010101010, 0x0808080808080808, 0x0404040404040404, 0x0202020202020202, 0x0101010101010101};

#define TOUR_NORD(x) (vert[x%8] << (x/8 + 1))
#define TOUR_SUD(x) (vert[x%8] >> (8-(x/8+1)))					 
							 
// bitboard hori[8]  = {0x00000000000000ff, 0x000000000000ff00, 0x0000000000ff0000, 0x00000000ff000000, 0x000000ff00000000, 0x0000ff0000000000, 0x00ff000000000000, 0xff00000000000000};

#define TOUR_EST(x) (((uint8_t)0xff << (8-x%8)) << ((x/8)*8) )
#define TOUR_OUEST(x) (((uint8_t) 0xff >>(8-x%8)) << ((x/8)*8) )

#endif /* TOUR_H */

/*
 * cas 3 :
 * hori = 0xff
 *	res  = 0xe0
 */
