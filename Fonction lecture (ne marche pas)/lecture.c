#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include <time.h>
#include "lecture.h"


/* Fonction qui lit les fichiers mp3 */
void lire(int *p_lecture, int *switcher, int *lecture, FMOD_CHANNEL *canal, FMOD_SYSTEM *system, char *son, FMOD_SOUND *morceau, SDL_Rect pos_button, float volume, unsigned int *length, char *temp_complet, SDL_Surface *total_time, TTF_Font *police_time, TTF_Font *police2, SDL_Color couleur_blanche, SDL_Color couleur_mauve, SDL_Rect pos_t_time, SDL_Surface *barre, SDL_Rect pos_barre, SDL_Surface *menu, char *titre, char *nom_son, SDL_Surface *nom_morceau, SDL_Rect pos_nom_morceau, TTF_Font *police1, SDL_Rect pos_spectre, SDL_Surface *spectre, SDL_Rect pos_fond, char *date, SDL_Surface *artiste, SDL_Rect pos_artiste, SDL_Surface *button)
{
/* Initialisation */
FMOD_RESULT resultat;
/* FMOD */
if ((*p_lecture)) FMOD_Channel_Stop(canal);
/* On joue le nouveau son */
resultat=FMOD_System_CreateSound(system, son, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &morceau);
   if (resultat!=FMOD_OK)
   {
   fprintf(stderr, "Erreur lors de l'ouverture du son\n");
   exit(EXIT_FAILURE);
   }
FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, morceau, 0, NULL);
/* On remet le button de lecture a zero */
pos_button.x=pos_barre.x-button->w;
FMOD_Sound_SetLoopCount(morceau, -1); /* Répétition infini */
FMOD_System_GetChannel(system, 0, &canal); /* On récupère le canal */
/* On remet le volume */
FMOD_Channel_SetVolume(canal, volume);
/* On récupère a nouveau la longueur du fichier mp3 */
fprintf(stderr, "ici\n");
FMOD_Sound_GetLength(morceau, &(*length), FMOD_TIMEUNIT_MS);
sprintf(temp_complet, " / %d:%d", (*length)/60000, ((*length)%60000)/1000); 
fprintf(stderr, "ici2\n");
total_time=TTF_RenderText_Blended(police_time, temp_complet, couleur_blanche);
fprintf(stderr, "ici3\n");
pos_t_time.x=(pos_barre.x+barre->w)-total_time->w;
pos_t_time.y=menu->h;
/* On récupère le nom du fichier mp3 */
sprintf(titre, "%s", nom_son);
nom_morceau=TTF_RenderText_Blended(police1, titre, couleur_blanche);
fprintf(stderr, "ici4\n");
pos_nom_morceau.x=pos_spectre.x+(spectre->w/2-nom_morceau->w/2);
pos_nom_morceau.y=pos_fond.y;
/* On récupère la date */
time_t timestamp=time(NULL);
strftime(date, sizeof(date), "%A %d %B %Y", localtime(&timestamp));
artiste=TTF_RenderText_Blended(police2, date, couleur_mauve);
pos_artiste.x=pos_spectre.x+(spectre->w/2-artiste->w/2);;
pos_artiste.y=pos_spectre.y/2-artiste->h/2;
/* On affecte la valeur 1 a lecture */
(*lecture)=1;
fprintf(stderr, "ici5\n");
if (!(*p_lecture) && !(*switcher)) { (*p_lecture)=1; (*switcher)=1;}  
fprintf(stderr, "ici6\n");   
}
