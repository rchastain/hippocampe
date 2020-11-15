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

void afficherPosition(struct Position p)
{
  int i, j;
  char c;
  char s[512] = "** +=====================+\n";
  
  for (i = r8 + 1; i >= r1 - 1; i--)
  {
    for (j = A - 1; j <= H + 1; j++)
    {
      switch (abs(p.diagramme[j][i]))
      {
        case PION:     c = 'P'; break;
        case CAVALIER: c = 'C'; break;
        case FOU:      c = 'F'; break;
        case TOUR:     c = 'T'; break;
        case DAME:     c = 'D'; break;
        case ROI:      c = 'R'; break;
        case VIDE:     c = '.';
      }
      if (p.diagramme[j][i] < 0 && isalpha(c))
      {
        c = tolower(c);
      }
      if (p.diagramme[j][i] == EXTERIEUR)
      {
        c = '$';
      }
      if (j == A - 1)
        sprintf(s + strlen(s), "**");
      if ((j == A - 1 || j == H + 1) && (i == r8 + 1 || i == r1 - 1))
        sprintf(s + strlen(s), " |");
      else if (j == A - 1 || j == H + 1)
        sprintf(s + strlen(s), " %c", i + 48);
      else if (i == r8 + 1 || i == r1 - 1)
        sprintf(s + strlen(s), " %c", j + 64);
      else
        sprintf(s + strlen(s), " %c", c);
      if (j == A - 1 || j == H)
        sprintf(s + strlen(s), "  ");
    }
    sprintf(s + strlen(s), "\n");
    if (i == r8 + 1 || i == r1)
      sprintf(s + strlen(s), "** |                     |\n");
  }
  sprintf(s + strlen(s), "** +=====================+\n** Trait : %d\n", p.trait);
  sprintf(s + strlen(s), "** En passant : %d\n", p.pep);
  sprintf(s + strlen(s), "** Coups : %.1f\n", p.ccoups);
  sprintf(s + strlen(s), "** Roque : %d\n", p.roques);
  journal("", s, "", 1);
}

void afficherEval(struct MVar mv)
{
  int i;

  //printf("eval=%d dep=%d\n", mv.orig.eval, mv.orig.departage);
  for (i = 0; i < mv.longueur; i++)
  {
    //printf("coup %d cd=%d ld=%d ca=%d la=%d\n", i, mv.variante[i].cd, mv.variante[i].ld, mv.variante[i].ca, mv.variante[i].la);
  }
}
