#ifndef DISPLAY_H
#define DISPLAY_H

#include "coup.h"

#ifdef EN
#define PION 		'P'
#define ROI 		'K'
#define DAME		'Q'
#define TOUR		'R'
#define FOU			'B'
#define CAVALIER	'N'
#define VIDE      	'.'
#define NOIR(a) 	(a + 32)
#else
#define PION 		'P'
#define ROI 		'R'
#define DAME		'D'
#define TOUR		'T'
#define FOU			'F'
#define CAVALIER	'C'
#define VIDE      	'.'
#define NOIR(a) 	(a + 32)
#endif

#define BLANC(a) 	(a)
#define TAILLE 		8


static inline void afficher(COUP_plateau_s p) {
	for (int i = TAILLE; i > 0; i--) {
		printf("%d | ", i);
		for (int j = 0; j < TAILLE; j++) {
			int idx = (i - 1) * TAILLE + j;
			char piece;
			
			if (p.blancs.pions & (1ULL << idx)) piece = BLANC(PION);
			else if (p.noirs.pions & (1ULL << idx)) piece = NOIR(PION);
			else if (p.blancs.cavaliers & (1ULL << idx)) piece = BLANC(CAVALIER);
			else if (p.noirs.cavaliers & (1ULL << idx)) piece = NOIR(CAVALIER);
			else if (p.blancs.fous & (1ULL << idx)) piece = BLANC(FOU);
			else if (p.noirs.fous & (1ULL << idx)) piece = NOIR(FOU);
			else if (p.blancs.tours & (1ULL << idx)) piece = BLANC(TOUR);
			else if (p.noirs.tours & (1ULL << idx)) piece = NOIR(TOUR);
			else if (p.blancs.dame & (1ULL << idx)) piece = BLANC(DAME);
			else if (p.noirs.dame & (1ULL << idx)) piece = NOIR(DAME);
			else if (p.blancs.roi & (1ULL << idx)) piece = BLANC(ROI);
			else if (p.noirs.roi & (1ULL << idx)) piece = NOIR(ROI);
			else piece = VIDE;
			
			printf("%c ", piece);
		}
		printf("\n");
	}
	puts("    ________________");
	puts("    a b c d e f g h");
}

#endif /* DISPLAY_H */
