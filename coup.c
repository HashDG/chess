#include <stdio.h>

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
int col(int);

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
		masque = TOUR_NORD(source);
		pa_tmp = pieces_adverses & masque;
		pj_tmp = pieces_joueur & masque;
		
		if (pa_tmp > 0) {
			pa_closest = peek_lsb(&pa_tmp);
		}
		if (pj_tmp > 0) {
			pj_closest = peek_lsb(&pj_tmp);
		}
		
		printf("adverse plus proche : %d, joueur plus proche : %d\n", pa_closest, pj_closest);
		printf("masque : %lx, source :%d\n", masque, source);
		
		/* Cas où la tour est bloquée par une pièce de sa couleur */
		if (pa_closest > pj_closest) {	
			for (int i = source + 8; i < pj_closest; i += 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
		/* Cas où la tour peut aller jusqu'à capturer un pion */
		} else {
			for (int i = source + 8; i < pa_closest; i += 8) {
				coups[--(*taille)] = COUP(source, i, P_TOUR, P_VIDE, P_VIDE, C_NORMAL);
			}
			coups[--(*taille)] = COUP(source, pa_closest, P_TOUR, P_VIDE, P_VIDE, C_CAPTURE);
		}
		
		
	}
	
	/*
	printf("Origine : %d, nord: %lx, sud : %lx, est : %lx, ouest : %lx\n", source, TOUR_NORD(source), TOUR_SUD(source), TOUR_EST(source), TOUR_OUEST(source));
	source = 1;
	printf("Origine : %d, nord: %lx, sud : %lx, est : %lx, ouest : %lx\n", source, TOUR_NORD(source), TOUR_SUD(source), TOUR_EST(source), TOUR_OUEST(source));
	source = 24;
	printf("Origine : %d, nord: %lx, sud : %lx, est : %lx, ouest : %lx\n", source, TOUR_NORD(source), TOUR_SUD(source), TOUR_EST(source), TOUR_OUEST(source));
	source = 56;
	printf("Origine : %d, nord: %lx, sud : %lx, est : %lx, ouest : %lx\n", source, TOUR_NORD(source), TOUR_SUD(source), TOUR_EST(source), TOUR_OUEST(source));
	source = 35;
	printf("Origine : %d, nord: %lx, sud : %lx, est : %lx, ouest : %lx\n", source, TOUR_NORD(source), TOUR_SUD(source), TOUR_EST(source), TOUR_OUEST(source));*/
	
	/************************************************************************** 
	 * pour chaque tours de la bonne couleur
	 * 	traitement par direction:
	 *			-> charger le masque d'orientation
	 *			-> masque sur tous les pions adverses et sur mes pièces 
	 *			-> peek_lsb / msb sur les masques
	 *			boucle insertion dans les coups
	 *
	 *
	 * fin-pour
	 */
	 return REUSSITE;
}

int lister_coups_pions(COUP_plateau_s p, int cote, int* taille, coup* coups) {
	bitboard temp, pions;
	bitboard pieces_adverses;
	bitboard pieces_joueur;
	bitboard avancement = 0;
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
		if (cote == B) {			
			avancement = ((pions & (1ULL << source)) << 8) & ~pieces_adverses & ~pieces_joueur;
		} else {
			// printf("temp : %llu, source : %d\n", temp, source);
			avancement = ((pions & (1ULL << source)) >> 8) & ~pieces_adverses & ~pieces_joueur;
		}
		
		if (avancement) {
			dest = source + (cote == B ? 8 : -8);
			
			// printf("Simple avancement : src %d, dst %d\n", source, dest);
	
			coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
	
			if (cote == B && source >= 8 && source <= 15) {			
				avancement = ((pions & (1ULL << source)) << 16) & ~pieces_adverses & ~pieces_joueur;
			} else if (cote == N && source >= 48 && source <= 55) {
				avancement = ((pions & (1ULL << source)) >> 16) & ~pieces_adverses & ~pieces_joueur;
			}
			if (avancement) {
				dest = source + (cote == B ? 16 : -16);
				// printf("Double avancement : src %d, dst %d\n", source, dest);
				coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_NORMAL);
			}
		}
		
		if (col(source) < 8) {
			if (cote == B && lig(source) < 8) {			
				avancement = ((pions & (1ULL << source)) << 9) & pieces_adverses & ~pieces_joueur;
				if (avancement) {
					dest = source + 9;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
				}
			} else if (cote == N && lig(source) > 0) {
				avancement = ((pions & (1ULL << source)) >> 7) & pieces_adverses & ~pieces_joueur;
				if (avancement) {
					dest = source - 7;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
				}
			}
		}
		if (col(source) > 0) {
			if (cote == B && lig(source) < 8) {			
				avancement = ((pions & (1ULL << source)) << 7) & pieces_adverses & ~pieces_joueur;
				if (avancement) {
					dest = source + 7;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
				}
			} else if (cote == N && lig(source) > 0) {
				avancement = ((pions & (1ULL << source)) >> 9) & pieces_adverses & ~pieces_joueur;
				if (avancement) {
					dest = source - 9;
					coups[--(*taille)] = COUP(source, dest, P_PION, P_VIDE, P_VIDE, C_CAPTURE);
				}
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
	case P_TOUR:
		if (cote == B) {
			p.blancs.tours &= ~(1ULL << source);
			p.blancs.tours |= (1ULL << dest);
			p.pieces_blanches &= ~(1ULL << source);
			p.pieces_blanches |= (1ULL << dest);			
		} else {
			p.noirs.tours &= ~(1ULL << source);
			p.noirs.tours |= (1ULL << dest);
			p.pieces_noires &= ~(1ULL << source);
			p.pieces_noires |= (1ULL << dest);
		}
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
