#ifndef ERROR_H
#define ERROR_H

#define REUSSITE           	0
#define ERR_DESTINATION   	-1
#define ERR_ORIGINE       	-2
#define ERR_FORMAT			-3
#define ERR_UNKNOWN       	-99

static inline const char *error(int errcode) {
    switch (errcode) {
        case REUSSITE:         	return "Aucune erreur";
        case ERR_DESTINATION:  	return "Destination invalide";
        case ERR_ORIGINE:   	return "Origine introuvable";
        case ERR_FORMAT:		return "Erreur de format de coup";
        case ERR_UNKNOWN:      	return "Erreur inconnue";
        default:               	return "Code d'erreur non reconnu";
    }
}
#endif /* ERROR_H */
