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
uint16_t loc(int);

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
	
	p.cote = B;
	lister_coups(p, &nb_coups, coups);
	
	printf("nb %d\n", nb_coups);
	
	for (int i = nb_coups; i < 300; i++) {
		char src[3] = {0}, dst[3] = {0};
		
		src[0] = loc(COUP_SOURCE(coups[i])) >> 8;
		src[1] = loc(COUP_SOURCE(coups[i])) & 0xff;
		dst[0] = loc(COUP_DEST(coups[i])) >> 8;
		dst[1] = loc(COUP_DEST(coups[i])) & 0xff;
		
		printf("Origine : %s, destination : %s\n", src, dst);
	}
	
	puts("##############################");
	puts("Passage aux noirs");
	puts("##############################");
	
	nb_coups = 300;
	p.cote = N;
	lister_coups(p, &nb_coups, coups);
	puts("hehe");
	
	printf("nb %d\n", nb_coups);
	
	for (int i = nb_coups; i < 300; i++) {
		char src[3] = {0}, dst[3] = {0};
		
		src[0] = loc(COUP_SOURCE(coups[i])) >> 8;
		src[1] = loc(COUP_SOURCE(coups[i])) & 0xff;
		dst[0] = loc(COUP_DEST(coups[i])) >> 8;
		dst[1] = loc(COUP_DEST(coups[i])) & 0xff;
		
		printf("Origine : %s, destination : %s\n", src, dst);
	}
	
	afficher(p);
	
	puts("##############################");
	puts("Appliquer un coup");
	puts("##############################");
	
	coup c = COUP(pos("c7"), pos("c3"), P_PION, 0, 0, C_NORMAL);
	printf("AVANT COUP : %lx\n", p.pieces_noires);
	appliquer_coup(c, &p, N);
	printf("APRES COUP : %lx\n", p.pieces_noires);
	
	afficher(p);
	
	nb_coups = 300;
	p.cote = B;
	lister_coups(p, &nb_coups, coups);
	
	printf("Nombre de coups : %d\n", 300 - nb_coups);
	
	for (int i = nb_coups; i < 300; i++) {
		char src[3] = {0}, dst[3] = {0}, typ = 0, *type;
		
		src[0] = loc(COUP_SOURCE(coups[i])) >> 8;
		src[1] = loc(COUP_SOURCE(coups[i])) & 0xff;
		dst[0] = loc(COUP_DEST(coups[i])) >> 8;
		dst[1] = loc(COUP_DEST(coups[i])) & 0xff;
		typ = COUP_TYPE(coups[i]);
		
		switch (typ) {
		case C_NORMAL: 
			type = "Normal";
			break;
		case C_CAPTURE:
			type = "Capture";
			break;
		}
		
		printf("Origine : %s, destination : %s, type : %s\n", src, dst, type);
	}
	
	return 0;
	
	/*
	printf("Première position des pions blancs : %d\n", premier_bit(pb));
	printf("Première position des pions noirs  : %d\n", premier_bit(pn));
	printf("Dernière position des pions blancs : %d\n", dernier_bit(pb));
	printf("Dernière position des pions noirs  : %d\n", dernier_bit(pn));
	
	char* loc = "a2";
	printf("Position %s (%02d) : pion blanc\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	
	loc = "d7";
	printf("Position %s (%02d) : pion noir\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	
	loc = "e8";
	printf("Position %s (%02d) : roi noir\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	
	loc = "e1";
	printf("Position %s (%02d) : roi blanc\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	printf("- premier_bit(rb) : %d\n", premier_bit(rb));
	printf("- dernier_bit(rb) : %d\n", dernier_bit(rb));
	
	loc = "a8";
	printf("Position %s (%02d) : tour noire\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	
	loc = "h1";
	printf("Position %s (%02d) : tour blanche\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	
	loc = "b8";
	printf("Position %s (%02d) : cavalier noir\n", loc, pos(loc));
	printf("- recuperer(%02d) : %c\n", pos(loc), recuperer(pos(loc)));
	*/
	
	/*
	char entree[5] = {0};
	char sortie = 0, res = 0;
	int loc = 0;
	trait_aux_blancs = true;

	do {
		afficher();
		puts("Donnez une coordonnée");
		gets(entree);
		loc = pos(entree);
		if (loc >= 0 && loc <= 63
			|| entree[1] == 'x') {
			res = traiter_coup(entree);
			if (!res) {
				trait_aux_blancs = !trait_aux_blancs;
			} else {
				printf("Résultat : %s\n", error(res));
			}
		} else if (entree[0] == 'e' && entree[1] == 'x' && entree[2] == 'i' && entree[3] == 't') {
			sortie = 1;
		} else {
			puts("Entrée invalide");
		}
	} while (!sortie);

	puts("Sortie !");
	*/
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

uint16_t loc(int a) {
	char lettre = a % 8 + 'a', chiffre = a / 8 + '1';
	return lettre << 8 | chiffre;
}

/*
bool existe(bitboard bb, int pos) {
	return (bb & (1ULL << pos)) != 0ULL;
}*/
/*
int placer(char piece, int pos) {
	switch (piece) {
	case PION:
		if (trait_aux_blancs) pb |= (1ULL << pos);
		else pn |= (1ULL << pos);
		break;
	case CAVALIER:
		if (trait_aux_blancs) cb |= (1ULL << pos);
		else cn |= (1ULL << pos);
		break;
	case FOU:
		if (trait_aux_blancs) fb |= (1ULL << pos);
		else fn |= (1ULL << pos);
		break;
	case TOUR:
		if (trait_aux_blancs) tb |= (1ULL << pos);
		else tn |= (1ULL << pos);
		break;
	case ROI:
		if (trait_aux_blancs) rb |= (1ULL << pos);
		else rn |= (1ULL << pos);
		break;
	case DAME:
		if (trait_aux_blancs) db |= (1ULL << pos);
		else dn |= (1ULL << pos);
		break;
	default:
		return 1;
	}
	return 0;
}

int retirer(char piece, int pos) {
	switch (piece) {
	case PION:
		if (trait_aux_blancs) pb &= ~(1ULL << pos);
		else pn &= ~(1ULL << pos);
		break;
	case CAVALIER:
		if (trait_aux_blancs) cb &= ~(1ULL << pos);
		else cn &= ~(1ULL << pos);
		break;
	case FOU:
		if (trait_aux_blancs) fb &= ~(1ULL << pos);
		else fn &= ~(1ULL << pos);
		break;
	case TOUR:
		if (trait_aux_blancs) tb &= ~(1ULL << pos);
		else tn &= ~(1ULL << pos);
		break;
	case ROI:
		if (trait_aux_blancs) rb &= ~(1ULL << pos);
		else rn &= ~(1ULL << pos);
		break;
	case DAME:
		if (trait_aux_blancs) db &= ~(1ULL << pos);
		else dn &= ~(1ULL << pos);
		break;
	default:
		return 1;
	}
	return 0;
}

char recuperer(int indice) {
	if ( pb & (1ULL << indice) ) return BLANC(PION);
	else if ( cb & (1ULL << indice) ) return BLANC(CAVALIER); 
	else if ( fb & (1ULL << indice) ) return BLANC(FOU); 
	else if ( tb & (1ULL << indice) ) return BLANC(TOUR);
	else if ( rb & (1ULL << indice) ) return BLANC(ROI); 
	else if ( db & (1ULL << indice) ) return BLANC(DAME);
	else if ( pn & (1ULL << indice) ) return NOIR(PION);
	else if ( cn & (1ULL << indice) ) return NOIR(CAVALIER); 
	else if ( fn & (1ULL << indice) ) return NOIR(FOU); 
	else if ( tn & (1ULL << indice) ) return NOIR(TOUR);
	else if ( rn & (1ULL << indice) ) return NOIR(ROI); 
	else if ( dn & (1ULL << indice) ) return NOIR(DAME);
	else return VIDE;
}

int premier_bit(bitboard bb) {
	return (int) log2(bb & -bb);
}

int dernier_bit(bitboard bb) {
	return (int) log2(bb);
}

*/

















