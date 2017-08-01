#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "explorateur.h"

/* P.S: Cet explorateur est trop nul par-rapport à d'autres :/ */
/* Fonction pour explorer les fichiers dans les dossiers */
int explorer(SDL_Surface *ecran, char *chaine, char *user, char *son, int *continuer, char *nom_son)
{
/* SDL */
SDL_Surface *explorateur=NULL;
SDL_Rect pos_explorateur;
SDL_Event event;
int c_a_explorer=1; /* continuer_a_explorer */
// explorateur=SDL_CreateRGBSurface(SDL_HWSURFACE, L_EXPLORATEUR, H_EXPLORATEUR, 32, 0, 0, 0, 0);
explorateur=IMG_Load("ressource\\explorateur.jpg");
pos_explorateur.x=(ecran->w/2)-(explorateur->w/2); 
pos_explorateur.y=((ecran->h-50)/2)-(explorateur->h/2); /* -50 pixel car la hauteur du menu n'est que 250 pixel et de l'ecran c'est 300 */
/* TTF */
// TTF_Init();
int i=0, save=0, blit=1, racine=0;
// int save_precedent=0, deplacement=0;
SDL_Surface *texte[8]={NULL};
/* 8 disque/8 surface de 0à7 car j'ai 8place dans l'explorateur chaque place de 20 pixel a peut pret */
char disque0[200]={0}, disque1[200]={0}, disque2[200]={0}, disque3[200]={0}, disque4[200]={0}, disque5[200]={0}, disque6[200]={0}, disque7[200]={0};
SDL_Rect pos_texte;
SDL_Color couleur_blanche={255, 255, 255}; /* Couleur blanche */
TTF_Font *police=NULL;
police=TTF_OpenFont("ressource\\arialbd.ttf", 15);
pos_texte.x=pos_explorateur.x+106+8; /* 8 pixel */
pos_texte.y=pos_explorateur.y+8;
/* DIRENT */
DIR* rep=NULL;
struct dirent* fichier=NULL;
char precedent[200];
strcpy(precedent, chaine);
/* Gestion d'événements */
while (c_a_explorer)
{
SDL_WaitEvent(&event);
   switch (event.type)
   {
   case SDL_QUIT: c_a_explorer=0; *continuer=0; break;
   case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
           if (event.button.x>=pos_explorateur.x+269 && event.button.x<=pos_explorateur.x+269+(399-269) && event.button.y>=pos_explorateur.y+168 && event.button.y<=pos_explorateur.y+168+(199-168)) 
           { /* Quitter */
           c_a_explorer=0;
           return 0;
           }
           else if (event.button.x>=pos_explorateur.x+108 && event.button.x<=pos_explorateur.x+108+(187-108) && event.button.y>=pos_explorateur.y+168 && event.button.y<=pos_explorateur.y+168+(199-168)) 
           { /* Haut */
              if (save>8)
              {
              if (!racine) blit=1; /* Si ce n'est pas la case Ordinateur */
              else blit=0;
              i=0;
              if ((save%8)!=0) save-=((save%8)+8);
              else save-=16;      
              // save_precedent=save;  
              // deplacement=1;         
              }
           }
           else if (event.button.x>=pos_explorateur.x+189 && event.button.x<=pos_explorateur.x+189+(267-189) && event.button.y>=pos_explorateur.y+168 && event.button.y<=pos_explorateur.y+168+(199-168)) 
           { /* Bas */
              if (!racine) blit=1; /* Si ce n'est pas la case Ordinateur */
              else blit=0;
              i=0;
              if ((save%8)!=0) save-=save%8;
              // if (save>0) save_precedent=save;
              // if (save>8) deplacement=1;
           }
           else if (event.button.x>=pos_explorateur.x+(399-40) && event.button.x<=pos_explorateur.x+399 && event.button.y>=pos_explorateur.y && event.button.y<=pos_explorateur.y+21) 
           { /* Precedent */
           char *pch;
           pch=strrchr(precedent, '\\');
           // fprintf(stderr, "%d : ", pch-precedent);
              if (pch-precedent>2 && racine==0) /* si on arrive pas au dossier racine */
              {
                 precedent[pch-precedent]='\0';  /* On tranche la chaine */
                 blit=1;
                 racine=0;
              }
              else if (pch-precedent==2 && racine==0) /* si on arrive a la racine */
              {
                 precedent[pch-precedent+1]='\0'; /* On efface ce qui suit la racine */
                 blit=1;
                 racine=0;
              }
           strcpy(chaine, precedent); /* "." dossier courant/actuel , ".." dossier avant actuel */
           // fprintf(stderr, "%s\n", precedent);
           i=0;
              // if (deplacement) 
              // {              
                // save=save_precedent;
                // deplacement=0;
              // }
              // else save=save_precedent=0;
           save=0;
           }
           else if (event.button.x>=pos_explorateur.x && event.button.x<=pos_explorateur.x+106 && event.button.y>=pos_explorateur.y && event.button.y<=pos_explorateur.y+64) 
           { /* Bureau */
           strcpy(chaine, "C:\\Documents and Settings\\");
           strcat(chaine, user);
           strcat(chaine, "\\Bureau");
           blit=1;
           i=0;
           save=0;
           racine=0;
           }
           else if (event.button.x>=pos_explorateur.x && event.button.x<=pos_explorateur.x+106 && event.button.y>=pos_explorateur.y+66 && event.button.y<=pos_explorateur.y+66+(137-66)) 
           { /* Mes Documents */
           strcpy(chaine, "C:\\Documents and Settings\\");
           strcat(chaine, user);
           strcat(chaine, "\\Mes documents");
           blit=1;
           i=0;
           save=0;
           racine=0;
           }
           else if (event.button.x>=pos_explorateur.x && event.button.x<=pos_explorateur.x+106 && event.button.y>=pos_explorateur.y+138 && event.button.y<=pos_explorateur.y+138+(199-138)) 
           { /* Ordinateur */
           /* Initialisation de toutes les variables disques */
           strcpy(disque0, "");
           strcpy(disque1, "");
           strcpy(disque2, "");
           strcpy(disque3, "");
           strcpy(disque4, "");
           strcpy(disque5, "");
           strcpy(disque6, "");
           strcpy(disque7, "");
           /* On prépare la surface de l'explorateur */
           SDL_BlitSurface(explorateur, NULL, ecran, &pos_explorateur);
           /* le maximum des disques/lecteurs amovibles/locaux qu'on peut détecter est 8 ! */
           char buffer[100], *Lettre_Lecteur;
           int c, d=0, type; 
           unsigned short Nombre_de_Lecteur, Copie_n_Lecteur;
           Nombre_de_Lecteur=GetLogicalDriveStrings(100, buffer);
           if (Nombre_de_Lecteur<1) exit(EXIT_FAILURE); /* S'il n'y a aucun lecteur */
           Copie_n_Lecteur=Nombre_de_Lecteur;
              while(Copie_n_Lecteur>4) /* Strictement plus grand que 4 car Nombre_de_Lecteur-4 est le minimum (le dernier lecteur) */
              { /* Nombre_de_Lecteur-(Copie_n_Lecteur-4)=Nombre_de_Lecteur(8-4)=Nombre_de_Lecteur-4 (le minimum) */
              c=Nombre_de_Lecteur-(Copie_n_Lecteur-4);
              Lettre_Lecteur=&buffer[c];
              // fprintf(stderr, ".%s\n", Lettre_Lecteur);
              type=GetDriveType(Lettre_Lecteur);
                  if (type==3 || type==2) /* Si disques/lecteurs locaux ou amovibles */
                  {
                     // strcat(Lettre_Lecteur, "\\");
                     texte[d]=TTF_RenderText_Blended(police, Lettre_Lecteur, couleur_blanche); 
                     if (d==0) strcpy(disque0, Lettre_Lecteur);
                     else if (d==1) strcpy(disque1, Lettre_Lecteur);
                     else if (d==2) strcpy(disque2, Lettre_Lecteur);
                     else if (d==3) strcpy(disque3, Lettre_Lecteur);
                     else if (d==4) strcpy(disque4, Lettre_Lecteur);
                     else if (d==5) strcpy(disque5, Lettre_Lecteur);
                     else if (d==6) strcpy(disque6, Lettre_Lecteur);
                     else if (d==7) strcpy(disque7, Lettre_Lecteur);
                     d++;
                  }
              Copie_n_Lecteur-=4;
              }
           /* Blit */ 
           pos_texte.y=pos_explorateur.y+8;
           for (c=0; c<d; c++)
           {
              SDL_BlitSurface(texte[c], NULL, ecran, &pos_texte);
              pos_texte.y+=20;
           }
           SDL_Flip(ecran);
           blit=0;
           i=0;
           save=0;
           racine=1; /* On indique qu'on explore les disques locaux ou amovibles */
           }
           /* Gestion des choix de fichier mp3 ou dossier (choix d'utilisateur) */
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+((399-44)-(106+8)) && event.button.y>=pos_explorateur.y+8 && event.button.y<=pos_explorateur.y+8+15)
           {
              if (strcmp(disque0, "0 fichier mp3 trouvé !")!=0)
              {
                 if (strchr(disque0, '.')==NULL) /* Si c'est un dossier */
                 {
                    if (!racine) /* Si ce n'est pas un dossier racine */
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque0);
                    }
                    else 
                    { 
                    strcpy(chaine, disque0); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque0);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque0);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+20 && event.button.y<=pos_explorateur.y+8+15+20)
           {
              if (strlen(disque1)>0)
              {
                 if (strchr(disque1, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque1);
                    }
                    else 
                    { 
                    strcpy(chaine, disque1); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque1);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque1);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+40 && event.button.y<=pos_explorateur.y+8+15+40)
           {
              if (strlen(disque2)>0)
              {
                 if (strchr(disque2, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque2);
                    }
                    else 
                    { 
                    strcpy(chaine, disque2); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque2);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque2);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+60 && event.button.y<=pos_explorateur.y+8+15+60)
           {
              if (strlen(disque3)>0)
              {
                 if (strchr(disque3, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque3);
                    }
                    else 
                    { 
                    strcpy(chaine, disque3); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque3);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque3);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+80 && event.button.y<=pos_explorateur.y+8+15+80)
           {
              if (strlen(disque4)>0)
              {
                 if (strchr(disque4, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque4);
                    }
                    else 
                    { 
                    strcpy(chaine, disque4); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque4);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque4);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+100 && event.button.y<=pos_explorateur.y+8+15+100)
           {
              if (strlen(disque5)>0)
              {
                 if (strchr(disque5, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque5);
                    }
                    else 
                    { 
                    strcpy(chaine, disque5); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque5);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque5);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+120 && event.button.y<=pos_explorateur.y+8+15+120)
           {
              if (strlen(disque6)>0)
              {
                 if (strchr(disque6, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque6);
                    }
                    else 
                    { 
                    strcpy(chaine, disque6); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque6);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque6);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
           else if (event.button.x>=pos_explorateur.x+106+8 && event.button.x<=pos_explorateur.x+106+8+(399-(106+8)) && event.button.y>=pos_explorateur.y+8+140 && event.button.y<=pos_explorateur.y+8+15+140)
           {
              if (strlen(disque7)>0)
              {
                 if (strchr(disque7, '.')==NULL)
                 {
                    if (!racine)
                    {
                    if (strlen(chaine)>3) strcat(chaine, "\\");
                    strcat(chaine, disque7);
                    }
                    else 
                    { 
                    strcpy(chaine, disque7); 
                    racine=0;
                    }
                 blit=1;
                 i=0;
                 save=0;
                 // deplacement=1;
                 }
                 else
                 {
                 strcpy(nom_son, disque7);
                 nom_son[30]='\0';
                 strcpy(son, chaine);
                 strcat(son, "\\");
                 strcat(son, disque7);
                 c_a_explorer=0;
                 return 1;
                 }
              }
           }
        break;
        }
   break;
   } 
     if (blit)
     {
   // SDL_FillRect(explorateur, NULL , SDL_MapRGB(explorateur->format, 255, 255, 255));
   SDL_BlitSurface(explorateur, NULL, ecran, &pos_explorateur);
   strcpy(precedent, chaine);
     rep=opendir(chaine);
     int trouver=0, j=0, maj=0;
     pos_texte.y=pos_explorateur.y+8;
     while ((fichier=readdir(rep))!=NULL)
     {
        if (i>save+7) break; /* Pour sortir du while */
        if (strstr(fichier->d_name, ".mp3")!=NULL || strchr(fichier->d_name, '.')==NULL)
        { /* Les dossiers qui ont un point dans leur noms ne seront pas détécté */
        trouver=1;
           if (i>=save)
           {
           maj=1; /* Activation de la mise à jour de l'ecran */
           texte[j]=TTF_RenderText_Blended(police, fichier->d_name, couleur_blanche);
           /* Enregistrement des disques (mp3) trouvé */
              if (i==save) /* save+0 */
              {
              strcpy (disque0, fichier->d_name);
              /* On initialise les autres a zéro */
              strcpy (disque1, "");
              strcpy (disque2, "");
              strcpy (disque3, "");
              strcpy (disque4, "");
              strcpy (disque5, "");
              strcpy (disque6, "");
              strcpy (disque7, "");
              }
              else if (i==save+1) strcpy (disque1, fichier->d_name);
              else if (i==save+2) strcpy (disque2, fichier->d_name);
              else if (i==save+3) strcpy (disque3, fichier->d_name);
              else if (i==save+4) strcpy (disque4, fichier->d_name);
              else if (i==save+5) strcpy (disque5, fichier->d_name);
              else if (i==save+6) strcpy (disque6, fichier->d_name);
              else if (i==save+7) strcpy (disque7, fichier->d_name);
           if (texte[j]->w>(399-(106+8))) texte[j]->w=399-(106+8); /* si le texte dépasse la largeur de l'explorateur */
           if (j==0 && texte[j]->w>((399-44)-(106+8))) texte[j]->w=(399-44)-(106+8); /* si on apprcohe du button precedent */
           SDL_BlitSurface(texte[j], NULL, ecran, &pos_texte);
           pos_texte.y+=20;
           j++;
           }
        i++;
        }
     } /* Sortie du while */
        /* Sauvegarde */
        if (i>save+7) save=i;
        else save+=j;
        // fprintf(stderr, "save : %d\n", save);
        /* Si on ne trouve aucun fichier mp3 */
        if (!trouver) 
        {
        maj=1;
        // deplacement=1;
        texte[0]=TTF_RenderText_Blended(police, "0 fichier mp3 trouvé !", couleur_blanche);
        strcpy (disque0, "0 fichier mp3 trouvé !");
        /* On initialise les autres aussi a zéro car il faut que rien ne soit dedans */
        strcpy (disque1, "");
        strcpy (disque2, "");
        strcpy (disque3, "");
        strcpy (disque4, "");
        strcpy (disque5, "");
        strcpy (disque6, "");
        strcpy (disque7, "");
        SDL_BlitSurface(texte[0], NULL, ecran, &pos_texte);
        }
     blit=0;
     closedir(rep);
   /* Mise à jour de l'ecran */
   if (maj) SDL_Flip(ecran); 
     }   
}
/* Vidage TTF */
for (i=0; i<8; i++) SDL_FreeSurface(texte[i]);
/* Vidage SDL */
SDL_FreeSurface(explorateur);
}
