#ifndef SERVICES_H
#define SERVICES_H
#include "common.h"
bool authentification(const char *nom_utilisateur, const char *mdp);
void get_date_complete(char *time_str);                // 1
void get_liste_fichiers(char *dir_name, char *buff);   // 2
void get_contenu_fichier(char *file_name, char *buff); // 3
void get_time_taken(bool capture, char *buff);         // 4

#endif