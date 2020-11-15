/*
 *  sortie.c
 *  Author(s) : Jérémy Pages <jeremy.pages@eleves.ec-nantes.fr>
 *  Copyright (C) 2008 Jérémy Pages
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "regles.h"

#define abs(n) ((n >= 0) ? n : -n)

extern FILE *fjournal;

/* Ajout d'une ligne dans le fichier *.log */

void journal(char *prefixe, char *texte, char *suffixe, int flush)
{
  if (texte != NULL)
  {
    fprintf(fjournal, "%s%s%s", prefixe, texte, suffixe);
    if (flush)
      fflush(fjournal);
  }
}

/* Envoi d'un message à l'utilisateur */

void envoi(char *message)
{
  printf("%s\n", message);
  //fflush(stdout);
  journal("<- ", message, "\n", 1);
}

void afficherPosition(struct Position p, int affich)
{
  int x, y;
  char c;
  char s[512] = "** @== A B C D E F G H ==@\n";
  
  for (y = r8 + 1; y >= r1 - 1; y--)
  {
    for (x = A - 1; x <= H + 1; x++)
    {
      switch (abs(p.diagramme[x][y]))
      {
        case PION:     c = 'P'; break;
        case CAVALIER: c = 'C'; break;
        case FOU:      c = 'F'; break;
        case TOUR:     c = 'T'; break;
        case DAME:     c = 'D'; break;
        case ROI:      c = 'R'; break;
        case VIDE:     c = (((x + y) % 2 == 1) ? '.' : ':');
      }
      if (p.diagramme[x][y] < 0 && isalpha(c))
      {
        c = tolower(c);
      }
      if (p.diagramme[x][y] == EXTERIEUR)
      {
        c = '$';
      }
      if (y != r8 + 1 && y != r1 - 1)
      {
        if (x == A - 1)
          sprintf(s + strlen(s), "**");
        if (x == A - 1 || x == H + 1)
          sprintf(s + strlen(s), " %c", y + 48);
        else
          sprintf(s + strlen(s), " %c", c);
        if (x == A - 1 || x == H)
          sprintf(s + strlen(s), "  ");
      }
    }
    if (y != r8 + 1 && y != r1 - 1)
      sprintf(s + strlen(s), "\n");
    if (y == r8 + 1 || y == r1)
      sprintf(s + strlen(s), "** |                     |\n");
  }
  sprintf(s + strlen(s), "** @== A B C D E F G H ==@\n** Trait : %d\n", p.trait);
  sprintf(s + strlen(s), "** En passant : %d\n", p.pep);
  sprintf(s + strlen(s), "** Coups : %.1f\n", p.ccoups);
  sprintf(s + strlen(s), "** Roque : %d\n", p.roques);
  if (affich)
    printf(s);
  else
    journal("", s, "", 1);
}

void afficherEval(struct MVar mv, int affich)
{
  int i;
  char s[512] = "";
  
  sprintf(s + strlen(s), "eval=%d dep=%d\n", mv.orig.eval, mv.orig.departage);
    
  for (i = 0; i < mv.longueur; i++)
  {
    sprintf(s + strlen(s), "coup %d %c%d%c%d\n", i, mv.variante[i].cd + 96, mv.variante[i].ld, mv.variante[i].ca + 96, mv.variante[i].la);
  }
  
  if (affich)
    printf(s);
  else
    journal("", s, "", 1);
}
