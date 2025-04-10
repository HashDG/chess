#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "display.h"
#include "error.h"

#define TAILLE		8

/*
 * Les bits à gauche sont le haut du plateau 
 */
typedef uint64_t bitboard ;

int premier_bit(bitboard);
int dernier_bit(bitboard);
char recuperer(int);
int pos(char*);
int traiter_coup(char*);
int traiter_coup_pion(char*);
int placer(char, int);
int retirer(char, int);
bool existe(bitboard, int);

bitboard pb = 0, pn = 0;
bitboard cb = 0, cn = 0;
bitboard fb = 0, fn = 0;
bitboard tb = 0, tn = 0;
bitboard rb = 0, rn = 0;
bitboard db = 0, dn = 0;

#define BLANCS		(pb | cb | fb | tb | rb | db)
#define NOIRS		(pn | cn | fn | tn | rn | dn)

bool trait_aux_blancs;

#define COTE_COURANT (trait_aux_blancs ? BLANCS : NOIRS) 
#define AUTRE_COTE   (trait_aux_blancs ? NOIRS : BLANCS)

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
	
}

/*
 *	Pc		=> Pièce (sauf pion) Coordonnées (a1 -> h8)
 *	PCxc	=> Pièce (sauf pion) Colonne (si deux pièces peuvent le jouer) X (prise) Coordonnées 
 *  Pcc+ 	=> échec
 *  Pcc	
 * 	On va big utiliser regex ébauche de regex ([rRdDcCtTfF]|)([a-h][1-8]|[a-h]|[1-8])(?:(x|)([a-h][1-8])|)
 */
int traiter_coup(char* coup) {
	int resultat = 0;
	
	// prise par un pion
	if (1) {
		resultat = traiter_coup_pion(coup);
	}
	
	return resultat;
}

/*
 * Si le coup est valide, la fonction renvoie la position de la case à l'initiative
 */
int traiter_coup_pion(char* coup) {
	if (pos(coup) < 0 && coup[1] != 'x') return ERR_FORMAT;

	char origine[3] = {0}, temp[3] = {0};
	bitboard pion, npion;
	
	/* dxe4 */
	if (coup[1] == 'x') {
		printf("destination : %s\n", coup + 2);
		/* si la destination est un pion de la couleur courante erreur */
		if (existe(COTE_COURANT, pos(coup + 2))) return ERR_DESTINATION;
		/* si la destination n'est pas de l'autre couleur erreur*/
		if (!existe(AUTRE_COTE, pos(coup + 2))) return ERR_DESTINATION;
		
		origine[0] = coup[0];
		origine[1] = coup[3] - (trait_aux_blancs ? 1 : -1);
		
		printf("origine : %s\n", origine);
		
		/* si l'origine n'est pas de la couleur courante erreur*/
		if (!existe(COTE_COURANT, pos(origine))) return ERR_ORIGINE;
		
		pion = (1ULL << pos(origine));
		
		if (trait_aux_blancs) {
			npion = pion << (8 + (coup[2] - coup[0]));
		} else {
			npion = pion >> (8 + (coup[0] - coup[2]));
		}
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*/
		if (trait_aux_blancs) {		
			pb = (pb & ~pion) | npion;
			pn &= ~pb;
			cn &= ~pb;
			fn &= ~pb;
			tn &= ~pb;
			dn &= ~pb;
		} else {
			pn = (pn & ~pion) | npion;
			pb &= ~pn;
			cb &= ~pn;
			fb &= ~pn;
			tb &= ~pn;
			db &= ~pn;
		}
	} else { /* d4 */
		if (existe(BLANCS | NOIRS, pos(coup))) return ERR_DESTINATION;
		
		origine[0] = coup[0];
		temp[0] = coup[0];
		if (coup[1] ==  (trait_aux_blancs ? '4' : '5')) {
			origine[1] = coup[1] - 2 * (trait_aux_blancs ? 1 : -1);
			temp[1] = coup[1] - (trait_aux_blancs ? 1 : -1);
			printf("origine: %s, temp: %s\n", origine, temp);
			if (!existe(COTE_COURANT, pos(origine))
				|| existe(COTE_COURANT, pos(temp))) {
				origine[1] = temp[1];
				
				pion = (1ULL << pos(origine));
				if (trait_aux_blancs) {
					npion = pion << 8;
				} else {
					npion = pion >> 8;
				}
			} else {
				pion = (1ULL << pos(origine));
				if (trait_aux_blancs) {
					npion = pion << 16;
				} else {
					npion = pion >> 16;
				}
			}
		} else {
			origine[1] = coup[1] - (trait_aux_blancs ? 1 : -1);
			pion = (1ULL << pos(origine));
			if (trait_aux_blancs) {
				npion = pion << 8;
			} else {
				npion = pion >> 8;
			}
		}
		
		printf("origine : %s, %c\n", origine, recuperer(pos(origine)));
		if (!existe(COTE_COURANT, pos(origine))) return ERR_ORIGINE;
		
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*/
		if (trait_aux_blancs) {
			pb = (pb & ~pion) | npion;
		} else {
			pn = (pn & ~pion) | npion;
		}
	}
	
	return REUSSITE; 
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

bool existe(bitboard bb, int pos) {
	return (bb & (1ULL << pos)) != 0ULL;
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



















