#include <stdio.h>

#include "coup.h"
#include "error.h"

#define COUP(source, dest, piece, promo, capt, type) ((source & 0x3f) | ((dest & 0x3f) << 6) | ((piece & 0x7) << 12) | ((promo & 0x7) << 13) | ((capt & 0x7) << 18) | ((type & 0xf) << 21))

int lister_coups_pions(bitboard pions, COUP_plateau_s plateau, int cote, int* taille, coup* coups);
int pop_lsb(bitboard* b);
int lig(int);
int col(int);

int lister_coups(COUP_plateau_s p, int* taille, coup* coups) {
	if (p.cote == B) {
		return lister_coups_pions(p.blancs.pions, p, p.cote, taille, coups);
	} else {
		return lister_coups_pions(p.noirs.pions, p, p.cote, taille, coups);
	}
}

int lister_coups_pions(bitboard pions, COUP_plateau_s plateau, int cote, int* taille, coup* coups) {
	bitboard temp = pions;
	bitboard pieces_adverses;
	bitboard pieces_joueur;
	bitboard avancement = 0;
	int source = 0, dest = 0;
	
	if (cote == B) {
		pieces_adverses = plateau.pieces_noires;
		pieces_joueur = plateau.pieces_blanches;
	} else {
		pieces_adverses = plateau.pieces_blanches;
		pieces_joueur = plateau.pieces_noires;
	}
	
	while (temp && *taille > 0) {
		source = pop_lsb(&temp);
		if (cote == B) {			
			avancement = ((pions & (1ULL << source)) << 8) & ~pieces_adverses;
		} else {
			printf("temp : %llu, source : %d\n", temp, source);
			avancement = ((pions & (1ULL << source)) >> 8) & ~pieces_adverses;
		}
		
		if (avancement) {
			dest = source + (cote == B ? 8 : -8);
			
			printf("Simple avancement : src %d, dst %d\n", source, dest);
	
			coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
	
			if (cote == B && source >= 8 && source <= 15) {			
				avancement = ((pions & (1ULL << source)) << 16) & ~pieces_adverses;
			} else if (cote == N && source >= 48 && source <= 55) {
				avancement = ((pions & (1ULL << source)) >> 16) & ~pieces_adverses;
			}
			if (avancement) {
				dest = source + (cote == B ? 16 : -16);
				printf("Double avancement : src %d, dst %d\n", source, dest);
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
		
		if (col(source) < 8) {
			if (cote == B && lig(source) < 8) {			
				avancement = ((pions & (1ULL << source)) << 9) & pieces_adverses;
				if (avancement) {
					dest = source + 9;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
				}
			} else if (cote == N && lig(source) > 0) {
				avancement = ((pions & (1ULL << source)) >> 7) & pieces_adverses;
				if (avancement) {
					dest = source - 7;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
				}
			}
		}
		if (col(source) > 0) {
			if (cote == B && lig(source) < 8) {			
				avancement = ((pions & (1ULL << source)) << 7) & pieces_adverses;
				if (avancement) {
					dest = source + 7;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
				}
			} else if (cote == N && lig(source) > 0) {
				avancement = ((pions & (1ULL << source)) >> 9) & pieces_adverses;
				if (avancement) {
					dest = source - 9;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
				}
			}
		}		
	}
	return REUSSITE;
}

int lig(int pos) {
	return pos / 8;
}

int col(int pos) {
	return pos % 8;
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

int pop_lsb(bitboard* b) {
	bitboard lsb = *b & -(*b);
	int index = __builtin_ctzll(lsb);
	*b &= ~lsb;
	return index;
}














/*
 * Il va falloir revoir le code, déjà abstraire les bitboards et rendre ce fichier indépendant des états des bitboards 
 * Les fonctions qui traitent de la validité n'ont besoin que du bitboard du type de pion visé, les autres types de sa couleur (éviter le cannibalisme) => utile pour l'entrée utilisateur
 * On a besoin de savoir si un coup mène à un échec de son propre roi (cas de clouage) => pour l'annuler
 * Renvoyer la liste des coups légaux pour une pièce donnée / toutes les pièces => utile pour vérifier l'entrée utilisateur / en vue de lancer le traitement !!! <<<<<<
 *
 *
 *
 *
 */

/*
 *	Pc		=> Pièce (sauf pion) Coordonnées (a1 -> h8)
 *	PCxc	=> Pièce (sauf pion) Colonne (si deux pièces peuvent le jouer) X (prise) Coordonnées 
 *  Pcc+ 	=> échec
 *  Pcc	
 * 	On va big utiliser regex ébauche de regex ([rRdDcCtTfF]|)([a-h][1-8]|[a-h]|[1-8])(?:(x|)([a-h][1-8])|)
 *
int traiter_coup(char* coup) {
	int resultat = 0;
	
	// prise par un pion
	if (coup[0] == TOUR) {
		coup++;
		if (pos(coup) < 0 && coup[1] != 'x') return ERR_FORMAT;
		resultat = traiter_coup_tour(coup);
	} else {
		if (pos(coup) < 0 && coup[1] != 'x') return ERR_FORMAT;
		resultat = traiter_coup_pion(coup);
	}
	
	return resultat;
}


 *
 * Si le coup est valide, la fonction renvoie la position de la case à l'initiative
 *
int traiter_coup_pion(char* coup) {
	char origine[3] = {0}, temp[3] = {0};
	bitboard pion, npion;
	
	/* dxe4 *
	if (coup[1] == 'x') {
		printf("destination : %s\n", coup + 2);
		/* si la destination est un pion de la couleur courante erreur *
		if (existe(COTE_COURANT, pos(coup + 2))) return ERR_DESTINATION;
		/* si la destination n'est pas de l'autre couleur erreur*
		if (!existe(AUTRE_COTE, pos(coup + 2))) return ERR_DESTINATION;
		
		origine[0] = coup[0];
		origine[1] = coup[3] - (trait_aux_blancs ? 1 : -1);
		
		printf("origine : %s\n", origine);
		
		/* si l'origine n'est pas de la couleur courante erreur*
		if (!existe(COTE_COURANT, pos(origine))) return ERR_ORIGINE;
		
		pion = (1ULL << pos(origine));
		
		if (trait_aux_blancs) {
			npion = pion << (8 + (coup[2] - coup[0]));
		} else {
			npion = pion >> (8 + (coup[0] - coup[2]));
		}
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*
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
	} else { /* d4 *
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
		
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*
		if (trait_aux_blancs) {
			pb = (pb & ~pion) | npion;
		} else {
			pn = (pn & ~pion) | npion;
		}
	}
	
	return REUSSITE; 
}

int traiter_coup_tour(char* coup) {
	char origine[3] = {0}, temp[3] = {0};
	bitboard pion, npion;
	
	if (coup[0] == 'x') { /* Txe4 *
		printf("destination : %s\n", coup + 2);
		/* si la destination est un pion de la couleur courante erreur *
		if (existe(COTE_COURANT, pos(coup + 2))) return ERR_DESTINATION;
		/* si la destination n'est pas de l'autre couleur erreur*
		if (!existe(AUTRE_COTE, pos(coup + 2))) return ERR_DESTINATION;
		
		origine[0] = coup[0];
		origine[1] = coup[3] - (trait_aux_blancs ? 1 : -1);
		
		printf("origine : %s\n", origine);
	
		/* si l'origine n'est pas de la couleur courante erreur*
		if (!existe(COTE_COURANT, pos(origine))) return ERR_ORIGINE;
		
		pion = (1ULL << pos(origine));
		
		if (trait_aux_blancs) {
			npion = pion << (8 + (coup[2] - coup[0]));
		} else {
			npion = pion >> (8 + (coup[0] - coup[2]));
		}
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*
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
	} else if (coup[1] == 'x') { /* Taxd4 *
	
	
	
	} else if (coup[1] >= 'a' && coup[1] <= 'h') /* Tad4 *
	
	} else { /* Td4 *
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
		
		
		/* On met à jour le déplacement du pion, puis on force l'inversion des plateaux étrangers*
		if (trait_aux_blancs) {
			pb = (pb & ~pion) | npion;
		} else {
			pn = (pn & ~pion) | npion;
		}
	}
	
	return REUSSITE;
}
*/
