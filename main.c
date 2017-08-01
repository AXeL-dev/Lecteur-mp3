/* Crée le : 04/06/2013 */
/* Modifié le : 09/06/2013 */
/* Dernière modification : 13/06/2013 */
/* Commentaire : Le code est un peu (trooop) laid, et mal organisé , j'ai essayer d'utiliser une fonction pour lire le son mais malheureusement ça ne marche pas correctement et les arguments à passer sont trop nombreux ! */
/* Idée d'amélioration : Ajouter une playliste pour lire plusieurs fichiers l'un après l'autre */
/* A revoir : (DONE !) l'avancement de la musique est mal codé (le button qui avance dans la barre en dessous) */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include <time.h>
#include <windows.h>
#include "const.h"
#include "pixelparpixel.h"
#include "explorateur.h"


/* Fontion principale */
int main (int argc, char *argv[])
{
/* Initialisation SDL */
SDL_Surface *ecran=NULL, *menu=NULL, *play=NULL, *pause=NULL, *barre=NULL, *button=NULL, *fond=NULL, *spectre=NULL, *vol=NULL, *barre_vol=NULL, *button_vol=NULL, *quitter=NULL, *open=NULL, *close=NULL, *replay=NULL;
SDL_Rect pos_menu, pos_play_pause, pos_barre, pos_button, pos_fond, pos_spectre, pos_vol, pos_barre_vol, pos_button_vol, pos_quitter, pos_open, pos_close, pos_replay;
SDL_Event event;
int continuer=1, t_p=0, t_a=0, lecture=0, p_lecture=0, i=0, j=0, h_barre=0, motion=0, switcher=0, reste=0, seconde=0;
float somme=0;
if (SDL_Init(SDL_INIT_VIDEO)==-1)
{
fprintf(stderr, "Erreur d'initialisation de la sdl : %s\n", SDL_GetError());
exit(EXIT_FAILURE);
}
/* Initialisation FMOD */
int n_son=0; /* nouveau_son */
char son[200]="Initialisation", nom_son[30]="Initialisation"; /* ou ={0} meme chose (ça va changer dans tt les cas) */
FMOD_SYSTEM *system;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
FMOD_SOUND *morceau=NULL;
FMOD_CHANNEL *canal=NULL;
FMOD_RESULT resultat;
float v_spectre[512], volume=1.0; /* Volume max */
/* Initialisation TTF */
unsigned int length=0, position=0;
char titre[30]={0}, date[30]={0};
char temp_complet[8]={0}, temp_en_cours[5]={0};
SDL_Surface *nom_morceau=NULL, *artiste=NULL, *total_time=NULL, *time_left=NULL;
SDL_Rect pos_nom_morceau, pos_artiste, pos_t_time, pos_time_l;
TTF_Font *police1=NULL, *police2=NULL, *police_time=NULL;
SDL_Color couleur_blanche={255, 255, 255}; /* Couleur blanche */
SDL_Color couleur_mauve={164, 89, 179}; /* Couleur mauve */
TTF_Init();
/* SDL */ 
SDL_WM_SetIcon(IMG_Load("ressource\\icone.jpg"), NULL);
ecran=SDL_SetVideoMode(L_FENETRE, H_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
SDL_WM_SetCaption("Lecteur mp3", NULL);
menu=IMG_Load("ressource\\menu.jpg");
pos_menu.x=0;
pos_menu.y=0;
play=IMG_Load("ressource\\play.jpg"); /* play et pause ont les memes hauteurs et largeures */
pause=IMG_Load("ressource\\pause.jpg");
pos_play_pause.x=play->w/3;
pos_play_pause.y=menu->h;
barre=IMG_Load("ressource\\barre.png");
pos_barre.x=play->w*2;
pos_barre.y=menu->h+(((ecran->h-menu->h)/2)-barre->h);
button=IMG_Load("ressource\\button.png");
pos_button.x=pos_barre.x-button->w/4; /* -button->w (/4=4pixel) */
pos_button.y=pos_barre.y+(barre->h/2-button->h/2);
fond=IMG_Load("ressource\\fond.jpg");
pos_fond.x=pos_barre.x/2;
pos_fond.y=(menu->h/2)-(fond->h/2);
spectre=SDL_CreateRGBSurface(SDL_HWSURFACE, L_SPECTRE, H_SPECTRE, 32, 0, 0, 0, 0);
pos_spectre.x=menu->w-((spectre->w)+(menu->w-(pos_barre.x+barre->w)));
pos_spectre.y=pos_fond.y+(fond->h-spectre->h); /* (ecran->h/2)-(spectre->h/2) */
vol=IMG_Load("ressource\\volume.jpg");
pos_vol.x=pos_spectre.x+(spectre->w/6); /* Pour que ça soit un peu au milieu */
pos_vol.y=pos_spectre.y-(vol->h*2);
barre_vol=IMG_Load("ressource\\barre_volume.png");
pos_barre_vol.x=pos_vol.x+(vol->w*2);
pos_barre_vol.y=pos_vol.y+(vol->h/2-barre_vol->h/2);
button_vol=IMG_Load("ressource\\button_volume.png");
pos_button_vol.x=pos_barre_vol.x+barre_vol->w;
pos_button_vol.y=pos_barre_vol.y+(barre_vol->h/2-button_vol->h/2);
quitter=IMG_Load("ressource\\quitter.jpg");
replay=IMG_Load("ressource\\replay.jpg");
pos_replay.x=pos_play_pause.x+(replay->w*2)-6; /* -6 pixel */
pos_replay.y=pos_play_pause.y-(replay->h/2)-6;
open=IMG_Load("ressource\\open.jpg");
pos_open.x=pos_replay.x+(open->w)+10; /* +10 pixel */
pos_open.y=pos_replay.y;
close=IMG_Load("ressource\\close.jpg");
pos_close.x=menu->w-(close->w)-4;
pos_close.y=4; /* +4 pixel */
/* FMOD */
// resultat=FMOD_System_CreateSound(system, son, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &morceau);
//  if (resultat!=FMOD_OK)
//  {
//  if (n_son) fprintf(stderr, "Erreur lors de l'ouverture du son\n");
//  exit(EXIT_FAILURE);
//  }
// FMOD_Sound_SetLoopCount(morceau, -1); /* Répétition infini */
// FMOD_System_GetChannel(system, 0, &canal); /* On récupère le canal */
/* On récupère la longueur de la musique */
// FMOD_Sound_GetLength(morceau, &length, FMOD_TIMEUNIT_MS);
/* TTF */
police1=TTF_OpenFont("ressource\\arialbd.ttf", 15);
police2=TTF_OpenFont("ressource\\jokerman.ttf", 15);
police_time=TTF_OpenFont("ressource\\arialbd.ttf", 10);
// TTF_SetFontStyle(police, TTF_STYLE_BOLD);
/* On récupère le titre de la chanson et la date */
// sprintf(titre, "%s", nom_son);
// time_t timestamp=time(NULL);
// strftime(date, sizeof(date), "%A %d %B %Y", localtime(&timestamp));
// FMOD_TAG tag;
// FMOD_Sound_GetTag(morceau, "ARTIST", 0, &tag);
// sprintf(art, "%s", /*(char*)*/tag.data);
// nom_morceau=TTF_RenderText_Blended(police1, titre, couleur_blanche);
// pos_nom_morceau.x=pos_spectre.x+(spectre->w/2-nom_morceau->w/2);
// pos_nom_morceau.y=pos_fond.y;
// artiste=TTF_RenderText_Blended(police2, date, couleur_mauve);
// pos_artiste.x=pos_spectre.x+(spectre->w/2-artiste->w/2);;
// pos_artiste.y=pos_spectre.y/2-artiste->h/2;
/* On récupère le temp en cours et total de la music */
// sprintf(temp_complet, " / %d:%d", length/60000, (length%60000)/1000); 
// sprintf(temp_en_cours, "00:00"); /* C'est juste une initialisation */
// total_time=TTF_RenderText_Blended(police_time, temp_complet, couleur_blanche);
// pos_t_time.x=(pos_barre.x+barre->w)-total_time->w;
// pos_t_time.y=menu->h;
/* Initialisation du chemin de l'explorateur */
char chaine[200]; /* il fallait mieu nommer chemin au lieu de chaine */
strcpy(chaine, "C:\\Documents and Settings\\");
char user[20]={0};
DWORD d=64;
GetUserName(user, &d);
strcat(chaine, user);
strcat(chaine, "\\Bureau");
/* Gestion d'événements */
int pixel=8; /* c'est juste pour régler le soucis de motivation du button de volume au début */
while (continuer)
{
SDL_PollEvent(&event); 
   switch (event.type) 
   {
   case SDL_QUIT: continuer=0; break;
// case SDL_KEYDOWN:
   //   switch (event.key.keysym.sym)
   //   {
   //   case SDLK_DOWN:
   //   if(volume > 0.0) volume -= 0.1;
   //   FMOD_Channel_SetVolume(canal, volume);
   //   break;
   //   case SDLK_UP:
   //   if(volume < 1.0) volume += 0.1;
   //   FMOD_Channel_SetVolume(canal, volume);
   //   break;
   //  } break; 
   case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
             if (event.button.x>=pos_play_pause.x && event.button.x<=pos_play_pause.x+play->w && event.button.y>=pos_play_pause.y && event.button.y<=pos_play_pause.y+play->h) 
             {
             FMOD_BOOL etat;
             FMOD_Channel_GetPaused(canal, &etat);
                if (lecture==0) /* ou !lecture */
                {
                   if (!p_lecture && !switcher)
                   {
                   goto Explorer;
                   // FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, morceau, 0, NULL);
                   // p_lecture=1;
                   // switcher=1;
                   }
                   else FMOD_Channel_SetPaused(canal, 0);
                lecture=1;
                }
                else
                { 
                FMOD_Channel_SetPaused(canal, 1);
                lecture=0;
                }
             // seconde=0;
             SDL_WaitEvent(&event);
             }
             else if (p_lecture && event.button.x>=pos_barre_vol.x && event.button.x<=pos_barre_vol.x+barre_vol->w+pixel && event.button.y>=pos_barre_vol.y && event.button.y<=pos_barre_vol.y+barre_vol->h) 
             {
             motion=1;
             if (pixel>0) pixel=0;
             pos_button_vol.x=event.button.x-(button_vol->w/2);
             volume=(float)(event.button.x-pos_barre_vol.x)/100;
             FMOD_Channel_SetVolume(canal, volume);
             // SDL_WaitEvent(&event);
             }
             else if (event.button.x>=pos_quitter.x && event.button.x<=pos_quitter.x+quitter->w && event.button.y>=pos_quitter.y && event.button.y<=pos_quitter.y+quitter->h) 
             {
                if (p_lecture) 
                {
                p_lecture=0;
                // switcher=1;
                pos_quitter.x=pos_barre.x+(barre->w-quitter->w);
                pos_quitter.y=pos_fond.y;
                }
                else p_lecture=1;
             }
             else if (event.button.x>=pos_replay.x && event.button.x<=pos_replay.x+replay->w && event.button.y>=pos_replay.y && event.button.y<=pos_replay.y+replay->h) 
             {
                if (switcher)
                {
                FMOD_Channel_Stop(canal);
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, morceau, 0, NULL);
                FMOD_Channel_SetVolume(canal, volume); /* On remet le volume */
                lecture=1;
                pos_button.x=pos_barre.x-button->w/4; /* -button->w (/4=4pixel) */
                somme=0;
                reste=0;
                }
             }
             else if (event.button.x>=pos_close.x && event.button.x<=pos_close.x+close->w && event.button.y>=pos_close.y && event.button.y<=pos_close.y+close->h) 
             {
                if (switcher)
                {
                FMOD_Channel_Stop(canal);
                p_lecture=0;
                switcher=0;
                lecture=0;
                // volume=1.0;
                pos_button.x=pos_barre.x-button->w/4; /* -button->w (/4=4pixel) */
                somme=0;
                reste=0;
                }
             }
             else if (event.button.x>=pos_open.x && event.button.x<=pos_open.x+open->w && event.button.y>=pos_open.y && event.button.y<=pos_open.y+open->h) 
             {
             Explorer:
             /* Appel a l'explorateur + pause si la lecture est en cours */
             if (lecture && switcher) FMOD_Channel_SetPaused(canal, 1);
             n_son=explorer(ecran, chaine, user, son, &continuer, nom_son);
             SDL_WaitEvent(&event);
             if (lecture && switcher) FMOD_Channel_SetPaused(canal, 0);
             if (n_son)
             {
             /* FMOD (lecture du (nouveau) fichier mp3) */
             if (p_lecture) FMOD_Channel_Stop(canal);
             /* On joue le nouveau son */
             resultat=FMOD_System_CreateSound(system, son, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &morceau);
                if (resultat!=FMOD_OK)
                {
                fprintf(stderr, "Erreur lors de l'ouverture du son\n");
                exit(EXIT_FAILURE);
                }
             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, morceau, 0, NULL);
             /* On remet le button de lecture a zero */
             pos_button.x=pos_barre.x-button->w/4; /* -button->w (/4=4pixel) */
             somme=0;
             reste=0;
             /* Posibilité de mettre un if içi car récupérer le canal et la répétition une seule fois au début suffit */
             FMOD_Sound_SetLoopCount(morceau, -1); /* Répétition infini */
             FMOD_System_GetChannel(system, 0, &canal); /* On récupère le canal */
             /* On remet le volume */
             FMOD_Channel_SetVolume(canal, volume);
             /* On récupère a nouveau la longueur du fichier mp3 */
             FMOD_Sound_GetLength(morceau, &length, FMOD_TIMEUNIT_MS);
             sprintf(temp_complet, " / %d:%d", length/60000, (length%60000)/1000); 
             total_time=TTF_RenderText_Blended(police_time, temp_complet, couleur_blanche);
             pos_t_time.x=(pos_barre.x+barre->w)-total_time->w;
             pos_t_time.y=menu->h;
             /* On récupère le nom du fichier mp3 */
             sprintf(titre, "%s", nom_son);
             nom_morceau=TTF_RenderText_Blended(police1, titre, couleur_blanche);
             pos_nom_morceau.x=pos_spectre.x+(spectre->w/2-nom_morceau->w/2);
             pos_nom_morceau.y=pos_fond.y;
             /* On récupère la date */
             time_t timestamp=time(NULL);
             strftime(date, sizeof(date), "%A %d %B %Y", localtime(&timestamp));
             artiste=TTF_RenderText_Blended(police2, date, couleur_mauve);
             pos_artiste.x=pos_spectre.x+(spectre->w/2-artiste->w/2);;
             pos_artiste.y=pos_spectre.y/2-artiste->h/2;
             /* On affecte la valeur 1 a lecture */
             lecture=1;
             if (!p_lecture && !switcher) { p_lecture=1; switcher=1;}  
             }
             }
        break;
        } 
        break;
        case SDL_MOUSEBUTTONUP: motion=0; break;
        case SDL_MOUSEMOTION:
        if (motion)
        {
           if (event.button.x>=pos_barre_vol.x && event.button.x<=pos_barre_vol.x+barre_vol->w) 
           {
           pos_button_vol.x=event.motion.x-(button_vol->w/2);
           volume=(float)(event.button.x-pos_barre_vol.x)/100;
           FMOD_Channel_SetVolume(canal, volume);
           }
        }
        break;
   }
t_a=SDL_GetTicks(); 
if (t_a - t_p < 25) SDL_Delay(25 - (t_a - t_p)); /* 25 ms car fmod redonne les coordonées du spectre chaque 25ms */
// seconde+=0.025; /* +25 miliseconde (1seconde=1000ms) */
  // else
  // {
   FMOD_Channel_GetPosition(canal, &position, FMOD_TIMEUNIT_MS); /* On récupère la position actuelle */
      if (lecture==1 && ((position%60000)/1000)-seconde==1) /* Si la lecture est actif et si une seconde passe */
         {
          // fprintf(stderr, "%d,%d\n", ((position%60000)/1000), seconde);
          /* C'est pas la meilleur méthode mais elle fonctionne comme meme */
            if (pos_button.x<=pos_barre.x+(barre->w-button->w)) 
            {
            if (position==length) pos_button.x=pos_barre.x+(barre->w-button->w)+1; /* +1 pixel pour foirer la condition (if) juste en haut */
            else pos_button.x+=barre->w/(length/1000)+reste;
            somme+=(float)barre->w/(length/1000) - (int)barre->w/(length/1000);
            // fprintf(stderr, "%f\n", somme);
            // int x=barre->w/(length/1000), y; 
            // if (x!=0) y=barre->w/x; /* barre->w=440 */
            // else y=length/1000;
            if (somme>=1.00) { reste=1; somme-=1.00;} /* somme=y-somme; */ 
            else reste=0;
            }
            else 
            {
               if (position/60000==0 && (position%60000)/1000==1) /* Si le son est fini et le prochain commence */
               {
                  pos_button.x=(pos_barre.x-button->w/4)+(barre->w/(length/1000)); /* -button->w (/4=4pixel) */
                  somme=0;
                  reste=0;
               }
            }
            // FMOD_BOOL isplaying;
            // FMOD_Channel_IsPlaying(canal, &isplaying);
            // if (!isplaying) pos_button.x=pos_barre.x; /* -button->w */ /* Si la lecture est fini */ 
         }
   t_p=t_a;
   SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
   SDL_BlitSurface(menu, NULL, ecran, &pos_menu);
      if (p_lecture) 
      {
      SDL_FillRect(spectre, NULL, SDL_MapRGB(spectre->format, 35, 45, 71));
      FMOD_Channel_GetSpectrum(canal, v_spectre, 512, 0, FMOD_DSP_FFT_WINDOW_RECT);
      SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
      pos_quitter.x=quitter->w;
      pos_quitter.y=pos_fond.y;
      SDL_BlitSurface(nom_morceau, NULL, ecran, &pos_nom_morceau);
      SDL_BlitSurface(artiste, NULL, ecran, &pos_artiste);
      SDL_BlitSurface(vol, NULL, ecran, &pos_vol);
      SDL_BlitSurface(barre_vol, NULL, ecran, &pos_barre_vol);
      SDL_BlitSurface(button_vol, NULL, ecran, &pos_button_vol);
      SDL_LockSurface(spectre);
         for (i=0; i<L_SPECTRE; i++)
         {
         h_barre=v_spectre[i]*15*H_SPECTRE;
         if (h_barre > H_SPECTRE) h_barre=H_SPECTRE;
            for (j=H_SPECTRE-h_barre; j<H_SPECTRE; j++)
            {
            setPixel(spectre, i, j, SDL_MapRGB(spectre->format, 255 - (j / RATIO), j / RATIO, 0));
            }   
         }
      SDL_UnlockSurface(spectre);
      SDL_BlitSurface(spectre, NULL, ecran, &pos_spectre);
      }
   if (lecture) SDL_BlitSurface(pause, NULL, ecran, &pos_play_pause);
   else SDL_BlitSurface(play, NULL, ecran, &pos_play_pause);
   if (switcher) SDL_BlitSurface(quitter, NULL , ecran, &pos_quitter);
   SDL_BlitSurface(barre, NULL, ecran, &pos_barre);
      if (switcher)
      {
      sprintf(temp_en_cours, "%d:%d", position/60000, (position%60000)/1000);
      seconde=(position%60000)/1000; /* affectation de seconde (incrementation avec le temp) */
      time_left=TTF_RenderText_Blended(police_time, temp_en_cours, couleur_blanche);
      pos_time_l.x=pos_t_time.x-time_left->w;
      pos_time_l.y=menu->h;
      SDL_BlitSurface(total_time, NULL, ecran, &pos_t_time);
      SDL_BlitSurface(time_left, NULL, ecran, &pos_time_l);
      }
   SDL_BlitSurface(replay, NULL, ecran, &pos_replay);
   SDL_BlitSurface(open, NULL, ecran, &pos_open);
   SDL_BlitSurface(close, NULL, ecran, &pos_close);
   SDL_BlitSurface(button, NULL, ecran, &pos_button);
   SDL_Flip(ecran);
  // }
}
/* Vidage TTF */
SDL_FreeSurface(nom_morceau);
SDL_FreeSurface(artiste);
SDL_FreeSurface(total_time);
SDL_FreeSurface(time_left);
TTF_CloseFont(police1);
TTF_CloseFont(police2);
TTF_CloseFont(police_time);
TTF_Quit();
/* Vidage FMOD */
FMOD_Sound_Release(morceau);
FMOD_System_Close(system);
FMOD_System_Release(system);
/* Vidage SDL */
SDL_FreeSurface(menu);
SDL_FreeSurface(play);
SDL_FreeSurface(pause);
SDL_FreeSurface(barre);
SDL_FreeSurface(button);
SDL_FreeSurface(fond);
SDL_FreeSurface(spectre);
SDL_FreeSurface(vol);
SDL_FreeSurface(barre_vol);
SDL_FreeSurface(button_vol);
SDL_FreeSurface(quitter);
SDL_FreeSurface(replay);
SDL_FreeSurface(open);
SDL_FreeSurface(close);
SDL_Quit();
return EXIT_SUCCESS;
}
