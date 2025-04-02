#ifndef DISPLAY_H
#define DISPLAY_H

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

int afficher(void);

#endif /* DISPLAY_H */
