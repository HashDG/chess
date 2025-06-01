#include <stdio.h>
#include <limits.h>

#include "coup.h"
#include "error.h"

#include "tour.h"

int lister_coups_pions(COUP_plateau_s plateau, int cote, int* taille, coup* coups);
int lister_coups_tours(COUP_plateau_s plateau, int cote, int* taille, coup* coups);
int peek_lsb(bitboard*);
int pop_lsb(bitboard*);
int peek_msb(bitboard*);
int pop_msb(bitboard*);
int lig(int);
char col(int);


int lister_coups(COUP_plateau_s p, int* taille, coup* coups) {
	int resultat;
	
	resultat = lister_coups_pions(p, p.cote, taille, coups);
	
	if (resultat == REUSSITE) {
		resultat = lister_coups_tours(p, p.cote, taille, coups);
	}
	
	return resultat;
}



int lister_coups_tours(COUP_plateau_s p, int cote, int* taille, coup* coups) {
	bitboard tours_tmp, tours, masque;
	bitboard pieces_adverses;
	bitboard pieces_joueur;
	int source;
	char position[3];
	
	if (cote == B) {
		tours_tmp = tours = p.blancs.tours;
		pieces_adverses = p.pieces_noires;
		pieces_joueur = p.pieces_blanches;
	} else {
		tours_tmp = tours = p.noirs.tours;
		pieces_adverses = p.pieces_blanches;
		pieces_joueur = p.pieces_noires;
	}
	
	while (tours_tmp) {
		bitboard pa_tmp, pj_tmp;
		int pa_closest = 0, pj_closest = 0;
		
		source = pop_lsb(&tours_tmp);
		
		/*******************NORD**********************************/
		
		masque = TOUR_NORD(source);
		pa_tmp = pieces_adverses & masque;
		pj_tmp = pieces_joueur & masque;
		
		if (pa_tmp > 0) {
			pa_closest = peek_lsb(&pa_tmp);
		} else {
			pa_closest = -1;
		}
		if (pj_tmp > 0) {
			pj_closest = peek_lsb(&pj_tmp);
		} else {
			pj_closest = -1;
		}
		
		puts("*******************NORD");
		printf("source : %s\n", posToString(source, position));
		printf("masque : %lx\n", masque);
		printf("plus proche adversaire : %s\n", posToString(pa_closest, position));
		printf("plus proche allié : %s\n", posToString(pj_closest, position));
		
		/* Cas où la tour est bloquée par une pièce de sa couleur */
		if (pa_closest > pj_closest && pj_closest >= 0) {
			for (int i = source + 8; i < pj_closest; i += 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		/* Cas où la tour peut aller jusqu'à capturer un pion */
		} else if (pa_closest >= 0) {
			for (int i = source + 8; i < pa_closest; i += 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
			coups[--(*taille)] = COUP(source, pa_closest, P_TOUR, P_VIDE, P_VIDE, C_CAPTURE);
		} else {
			for (int i = source + 8; i <= 56 + col(source) - 'a'; i += 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
		
		/*******************EST**********************************/
		
		masque = TOUR_EST(source);
		pa_tmp = pieces_adverses & masque;
		pj_tmp = pieces_joueur & masque;
		
		if (pa_tmp > 0) {
			pa_closest = peek_lsb(&pa_tmp);
		} else {
			pa_closest = -1;
		}
		if (pj_tmp > 0) {
			pj_closest = peek_lsb(&pj_tmp);
		} else {
			pj_closest = -1;
		}
		
		puts("*******************EST");
		printf("source : %s\n", posToString(source, position));
		printf("masque : %lx\n", masque);
		printf("plus proche adversaire : %s\n", posToString(pa_closest, position));
		printf("plus proche allié : %s\n", posToString(pj_closest, position));
		
		/* Cas où la tour est bloquée par une pièce de sa couleur */
		if (pa_closest > pj_closest && pj_closest >= 0) {
			for (int i = source + 1; i < pj_closest; i += 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		/* Cas où la tour peut aller jusqu'à capturer un pion */
		} else if (pa_closest >= 0) {
			for (int i = source + 1; i < pa_closest; i += 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
			coups[--(*taille)] = COUP(source, pa_closest, P_TOUR, P_VIDE, P_VIDE, C_CAPTURE);
		/* Cas où il n'y a pas de limite */
		} else {
			for (int i = source + 1; i <= 7 + (lig(source) - 1) * 8 ; i += 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
		
		/*******************SUD**********************************/
		
		masque = TOUR_SUD(source);
		pa_tmp = pieces_adverses & masque;
		pj_tmp = pieces_joueur & masque;
		
		if (pa_tmp > 0) {
			pa_closest = peek_msb(&pa_tmp);
		} else {
			pa_closest = -1;
		}
		if (pj_tmp > 0) {
			pj_closest = peek_msb(&pj_tmp);
		} else {
			pj_closest = -1;
		}
		
		puts("*******************SUD");
		printf("source : %s\n", posToString(source, position));
		printf("masque : %lx\n", masque);
		printf("plus proche adversaire : %s\n", posToString(pa_closest, position));
		printf("plus proche allié : %s\n", posToString(pj_closest, position));
		
		/* Cas où la tour est bloquée par une pièce de sa couleur */
		if (pa_closest < pj_closest && pj_closest >= 0) {
			for (int i = source - 8; i > pj_closest; i -= 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		/* Cas où la tour peut aller jusqu'à capturer un pion */
		} else if (pa_closest >= 0) {
			for (int i = source - 8; i > pa_closest; i -= 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
			coups[--(*taille)] = COUP(source, pa_closest, P_TOUR, P_VIDE, P_VIDE, C_CAPTURE);
		} else {
			for (int i = source - 8; i >= 0 + col(source) - 'a'; i -= 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
		
		/*******************OUEST**********************************/
		
		masque = TOUR_OUEST(source);
		pa_tmp = pieces_adverses & masque;
		pj_tmp = pieces_joueur & masque;
		
		if (pa_tmp > 0) {
			pa_closest = peek_msb(&pa_tmp);
		} else {
			pa_closest = -1;
		}
		if (pj_tmp > 0) {
			pj_closest = peek_msb(&pj_tmp);
		} else {
			pj_closest = -1;
		}
		
		puts("*******************OUEST");
		printf("source : %s\n", posToString(source, position));
		printf("masque : %lx\n", masque);
		printf("masque ALLIÉS : %lx\n", pj_tmp);
		printf("plus proche adversaire : %s\n", posToString(pa_closest, position));
		printf("plus proche allié : %s\n", posToString(pj_closest, position));
		
		/* Cas où la tour est bloquée par une pièce de sa couleur */
		if (pa_closest < pj_closest && pj_closest >= 0) {
			for (int i = source - 1; i > pj_closest; i -= 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		/* Cas où la tour peut aller jusqu'à capturer un pion */
		} else if (pa_closest >= 0) {
			for (int i = source - 1; i > pa_closest; i -= 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
			coups[--(*taille)] = COUP(source, pa_closest, P_TOUR, P_VIDE, P_VIDE, C_CAPTURE);
		} else {
			for (int i = source - 1; i >= (lig(source) - 1) * 8 ; i -= 1) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
	}
	return REUSSITE;
}

int lister_coups_pions(COUP_plateau_s p, int cote, int* taille, coup* coups) {
	bitboard temp, pions;
	bitboard pieces_adverses;
	bitboard pieces_joueur;
	bitboard avancement = 0;
	char coup_simple = 0;
	int source = 0, dest = 0;
	
	if (cote == B) {
		temp = pions = p.blancs.pions;
		pieces_adverses = p.pieces_noires;
		pieces_joueur = p.pieces_blanches;
	} else {
		temp = pions = p.noirs.pions;
		pieces_adverses = p.pieces_blanches;
		pieces_joueur = p.pieces_noires;
	}
	
	// printf("Dans lister coup : %x, %x\n", pieces_adverses, pieces_joueur);
	
	while (temp && *taille > 0) {
		source = pop_lsb(&temp);
		
		if (col(source) > 'a') {
			if (cote == B && (((pions & (1ULL << source)) << 7) & pieces_adverses & ~pieces_joueur) && lig(source) < 8) {
				dest = source + 7;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
			} else if (cote == N && (((pions & (1ULL << source)) >> 9) & pieces_adverses & ~pieces_joueur) && lig(source) > 0) {
				dest = source - 9;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
			}
		}
		
		if (col(source) < 'h') {
			if (cote == B && (((pions & (1ULL << source)) << 9) & pieces_adverses & ~pieces_joueur) && lig(source) < 8) {
				dest = source + 9;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
			} else if (cote == N && (((pions & (1ULL << source)) >> 7) & pieces_adverses & ~pieces_joueur) && lig(source) > 0) {
				dest = source - 7;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
			}
		}
		
		coup_simple = 0;
		if (cote == B && (((pions & (1ULL << source)) << 8) & ~pieces_adverses & ~pieces_joueur)) {
			dest = source + 8;
			coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
			coup_simple = 1;
		} else if (cote == N && (((pions & (1ULL << source)) >> 8) & ~pieces_adverses & ~pieces_joueur)) {
			dest = source - 8;
			coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
			coup_simple = 1;
		}
		
		if (coup_simple) {
			if (cote == B && (((pions & (1ULL << source)) << 16) & ~pieces_adverses & ~pieces_joueur) && lig(source) == 2) {
				dest = source + 16;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_DOUBLE);
			} else if (cote == N && (((pions & (1ULL << source)) >> 16) & ~pieces_adverses & ~pieces_joueur) && lig(source) == 7) {
				dest = source - 16;
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_DOUBLE);
			}
		}
	}
	return REUSSITE;
}

COUP_plateau_s appliquer_coup(coup c, COUP_plateau_s p, int cote) {
	int source, dest, piece, promo, capt, type;
	bitboard tmp;
	 
	source = COUP_SOURCE(c);
	dest = COUP_DEST(c);
	piece = COUP_PIECE(c);
	promo = COUP_PROMO(c);
	capt = COUP_CAPT(c);
	type = COUP_TYPE(c);
	
	printf("COUP, src : %d, dest : %d, piece : %d\n", source, dest, piece);
	
	switch (piece) {
	case P_PION:
		if (cote == B) {
			p.blancs.pions &= ~(1ULL << source);
			p.blancs.pions |= (1ULL << dest);
			p.pieces_blanches &= ~(1ULL << source);
			p.pieces_blanches |= (1ULL << dest);			
		} else {
			p.noirs.pions &= ~(1ULL << source);
			p.noirs.pions |= (1ULL << dest);
			p.pieces_noires &= ~(1ULL << source);
			p.pieces_noires |= (1ULL << dest);
		}
		break;
	case P_TOUR:
		if (cote == B) {
			printf("avant tours : %lx\n", p.blancs.tours);			
			p.blancs.tours &= ~(1ULL << source);
			p.blancs.tours |= (1ULL << dest);
			printf("après tours : %lx\n", p.blancs.tours);			
			p.pieces_blanches &= ~(1ULL << source);
			p.pieces_blanches |= (1ULL << dest);
		} else {
			p.noirs.tours &= ~(1ULL << source);
			p.noirs.tours |= (1ULL << dest);
			p.pieces_noires &= ~(1ULL << source);
			p.pieces_noires |= (1ULL << dest);
		}
		break;
	}
	
	if (cote == B) {
		p.noirs.pions &= ~p.pieces_blanches;
		p.noirs.cavaliers &= ~p.pieces_blanches;
		p.noirs.fous &= ~p.pieces_blanches;
		p.noirs.tours &= ~p.pieces_blanches;
		p.noirs.dame &= ~p.pieces_blanches;
		p.pieces_noires &= ~p.pieces_blanches;
	} else {
		p.blancs.pions &= ~p.pieces_noires;
		p.blancs.cavaliers &= ~p.pieces_noires;
		p.blancs.fous &= ~p.pieces_noires;
		p.blancs.tours &= ~p.pieces_noires;
		p.blancs.dame &= ~p.pieces_noires;
		p.pieces_blanches &= ~p.pieces_noires;
	}
	
	return p;
}

int lig(int pos) {
	return pos / 8 + 1;
}

char col(int pos) {
	return pos % 8 + 'a';
}

char recuperer(int indice, COUP_plateau_s plateau) {
	if ( plateau.noirs.pions & (1ULL << indice)
		||  plateau.blancs.pions & (1ULL << indice)) return P_PION;
	else if ( plateau.noirs.cavaliers & (1ULL << indice)
		||  plateau.blancs.cavaliers & (1ULL << indice)) return P_CAVALIER;
	else if ( plateau.noirs.fous & (1ULL << indice)
		||  plateau.blancs.fous & (1ULL << indice)) return P_FOU;
	else if ( plateau.noirs.tours & (1ULL << indice)
		||  plateau.blancs.tours & (1ULL << indice)) return P_TOUR;
	else if ( plateau.noirs.dame & (1ULL << indice)
		||  plateau.blancs.dame & (1ULL << indice)) return P_DAME;
	else if ( plateau.noirs.roi & (1ULL << indice)
		||  plateau.blancs.roi & (1ULL << indice)) return P_ROI;
	else return P_VIDE;
}

int peek_lsb(bitboard* b) {
	bitboard lsb = *b & -(*b);
	int index = __builtin_ctzll(lsb);
	return index;
}

int pop_lsb(bitboard* b) {
	bitboard lsb = *b & -(*b);
	int index = __builtin_ctzll(lsb);
	*b &= ~lsb;
	return index;
}

int peek_msb(bitboard* b) {
	int index = 63 - __builtin_clzll(*b);
	return index;
}

int pop_msb(bitboard* b) {
	int index = 63 - __builtin_clzll(*b);
	*b &= ~(1ULL << index);
	return index;
}

char* posToString(int a, char* res) {
	if (a >= 0 && a <= 64) {
		char lettre = a % 8 + 'a', chiffre = a / 8 + '1';
		res[0] = lettre;
		res[1] = chiffre;
		res[2] = '\0'; 
	} else {
		res[0] = 'I';
		res[1] = 'N';
		res[2] = 'C';
	}
	return res;
}
