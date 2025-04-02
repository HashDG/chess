#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "display.h"

#define TAILLE		8

/*
 * Les bits à gauche sont le haut du plateau 
 */
typedef uint64_t bitboard ;

int premier_bit(bitboard);
int dernier_bit(bitboard);
char recuperer(int);
unsigned int pos(char*);
int traiter_coup(char*);
int verifier_coup(int);
int placer(char, int);
int retirer(char, int);

bitboard pb = 0, pn = 0;
bitboard cb = 0, cn = 0;
bitboard fb = 0, fn = 0;
bitboard tb = 0, tn = 0;
bitboard rb = 0, rn = 0;
bitboard db = 0, dn = 0;

bool trait_aux_blancs;

int main(void) {
	pb = 0x000000000000ff00;
	pn = 0x00ff000000000000;
	cb = 0x0000000000000042;
	cn = 0x4200000000000000;
	fb = 0x0000000000000024;
	fn = 0x2400000000000000;
	tb = 0x0000000000000081;
	tn = 0x8100000000000000;
	rb = 0x0000000000000010;
	rn = 0x1000000000000000;
	db = 0x0000000000000008;
	dn = 0x0800000000000000;
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
	
	
	char entree[2];
	char sortie = 0;
	int loc = 0;
	trait_aux_blancs = true;
	
	do {
		afficher();
		puts("Donnez une coordonnée");
		gets(entree);
		loc = pos(entree);
		if (loc >= 0 && loc <= 63) {
			traiter_coup(entree);
			trait_aux_blancs = !trait_aux_blancs;
		} else if (entree[0] == 'z' && entree[1] == 'z') {
			sortie = 1;
		} else {
			puts("Entrée invalide");
		}
		
	} while (!sortie);

	puts("Sortie !");
	
}

/*
 *	Pcc		=> Pièce (sauf pion) Coordonnées (a1 -> h8)
 *	PCxcc	=> Pièce (sauf pion) Colonne (si deux pièces peuvent le jouer) X (prise) Coordonnées 
 *  Pcc+ 	=> échec
 *  Pcc	
 * 	On va big utiliser regex ébauche de regex ([rRdDcCtTfF]|)([a-h][1-8]|[a-h]|[1-8])(?:(x|)([a-h][1-8])|)
 */
int traiter_coup(char* coup) {
	
	int origine = verifier_coup(pos(coup));
	// printf("origine : %d, destination : %d\n", origine, pos(coup) );
	if (origine >= 0) {
		retirer(PION, origine);
		placer(PION, pos(coup));
	} else {
		puts("Erreur !");
	}
}

/*
 * Si le coup est valide, la fonction renvoie la position de la case à l'initiative
 */
int verifier_coup(int loc) {
	if (loc < 0 || loc > 63) return -1;
	 
	/* Tableaux des cases occupées blanches et noires */
	bitboard bb = pb | cb | fb | tb | rb | db, bn = pn | fn | tn | rn | dn;
	bitboard temp; 
	bitboard mask = 0;
	
	/* on prend la position désirée, on calcule à partir du bitboard adapté un masque plaçant l'origine et les positions possibles */
	if (trait_aux_blancs) {
		temp = pb;
		/* VALABLE SEULEMENT POUR LES PIONS !!!!! */
		if (loc >= pos("a4") && loc <= pos("h4")) {
			mask = (1ULL << loc) | (1ULL << loc - TAILLE) | (1ULL << loc - TAILLE * 2) ;
		} else {
			mask = (1ULL << loc) | (1ULL << loc - TAILLE);	
		}
	} else {
		temp = pn;
		if (loc >= pos("a5") && loc <= pos("h5")) {
			mask = (1ULL << loc) | (1ULL << loc + TAILLE) | (1ULL << loc + TAILLE * 2) ;
		} else {
			mask = (1ULL << loc) | (1ULL << loc + TAILLE);	
		}
		printf("loc : %d, mask: %x\n", loc, mask);
	}
	
	/* on applique le masque sur le bb du type de pion */
	temp ^= mask;
	
	if (trait_aux_blancs) {
		if ((temp & pb) != pb) {
			if (loc >= pos("a4") && loc <= pos("h4")) loc -= TAILLE;
			return loc - TAILLE;
		}
	} else {
		if ((temp & pn) != pn) {
			if (loc >= pos("a5") && loc <= pos("h5")) loc += TAILLE;
		 	return loc + TAILLE;
		}
	}
		
	/* si le résultat n'a pas fait bouger le masque initial (ça peut avoir changé à côté) => ratio */
	
	return -1; 
}

int afficher(void) {
	for (int i = TAILLE; i > 0; i--) {
		for (int j = 0; j < TAILLE; j++) {
			printf(" %c ", recuperer( (i - 1) * TAILLE + j));
		}
		printf("\n");
	}
}

/*
 *	a1 : 0, b1 : 1, c1 : 2 ... h8 : 63
 */ 
unsigned int pos(char* loc) {
	char lettre = loc[0], chiffre = loc[1];
	
	if (chiffre >= '1' && chiffre <= '8') {
		if (lettre >= 'a' && lettre <= 'h') {
			return lettre - 'a' + (chiffre - '1') * TAILLE;
		} else if (lettre >= 'A' && lettre <= 'H') {
			return lettre - 'A' + (chiffre - '1') * TAILLE;
		}
	}
	return 99;
}

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



















