#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "display.h"
#include "error.h"
#include "coup.h"

#define TAILLE		8

int premier_bit(bitboard);
int dernier_bit(bitboard);
char recuperer(int);
int pos(char*);
int placer(char, int);
int retirer(char, int);
bool existe(bitboard, int);

int main(void) {
	
	COUP_plateau_s p;
	
	p.blancs.pions = 	0x000000000000ff00;
	p.noirs.pions = 	0x00ff000000000000;
	p.blancs.cavaliers = 0x0000000000000042;
	p.noirs.cavaliers = 0x4200000000000000;
	p.blancs.fous = 	0x0000000000000024;
	p.noirs.fous = 		0x2400000000000000;
	p.blancs.tours = 	0x0000000000000081;
	p.noirs.tours = 	0x8100000000000000;
	p.blancs.roi = 		0x0000000000000010;
	p.noirs.roi = 		0x1000000000000000;
	p.blancs.dame = 	0x0000000000000008;
	p.noirs.dame = 		0x0800000000000000;
	p.pieces_blanches = 0x000000000000ffff;
	p.pieces_noires =   0xffff000000000000;
	
	int nb_coups = 300;
	coup coups[300] = {0};
	char position[3];
	
	afficher(p);
	
	puts("##############################");
	puts("Appliquer un coup pour la tour");
	puts("##############################");
	
	coup c = COUP(pos("a1"), pos("a3"), P_TOUR, 0, 0, C_NORMAL);	
	p = appliquer_coup(c, p, B);
	
	c = COUP(pos("a7"), pos("a5"), P_PION, 0, 0, C_NORMAL);
	p = appliquer_coup(c, p, N);
	
	c = COUP(pos("a2"), pos("a5"), P_PION, 0, 0, C_NORMAL);
	p = appliquer_coup(c, p, B);
	afficher(p);
	
	nb_coups = 300;
	p.cote = B;
	lister_coups(p, &nb_coups, coups);
	
	printf("Nombre de coups : %d\n", 300 - nb_coups);
	
	for (int i = nb_coups; i < 300; i++) {
		char src[3] = {0}, dst[3] = {0}, typ = 0, *type, piece = 0;
				
		posToString(COUP_SOURCE(coups[i]), src);
		posToString(COUP_DEST(coups[i]), dst);
		
		typ = COUP_TYPE(coups[i]);
		piece = COUP_PIECE(coups[i]);
		
		switch (typ) {
		case C_NORMAL: 
			type = "Normal";
			break;
		case C_CAPTURE:
			type = "Capture";
			break;
		}
		
		printf("Origine : %s, destination : %s, piece : %d, type : %s\n", src, dst, piece, type);
	}
	
	return 0;
}


/*
 *	a1 : 0, b1 : 1, c1 : 2 ... h8 : 63
 */
int pos(char* loc) {
	char lettre = loc[0], chiffre = loc[1];
	
	if (chiffre >= '1' && chiffre <= '8') {
		if (lettre >= 'a' && lettre <= 'h') {
			return lettre - 'a' + (chiffre - '1') * TAILLE;
		} else if (lettre >= 'A' && lettre <= 'H') {
			return lettre - 'A' + (chiffre - '1') * TAILLE;
		}
	}
	return -1;
}















