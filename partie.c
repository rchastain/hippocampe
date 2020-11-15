/*
 *  partie.c
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

#include "liste.h"
#include "recherche.h"
#include "regles.h"
#include "sortie.h"
#include "temps.h"

#define abs(n) ((n >= 0) ? n : (-n))

extern struct Partie enCours;
extern struct MVar mvProv;

struct Position convFEN(char* codeFEN)
{
  struct Position p;
  int index = 0, champ = 1, ligne = 8, colonne = 1, iccoups = 0;
  int i, j;

  for (i = 0; i <= 9; i++)
  {
    p.diagramme[0][i] = EXTERIEUR;
    p.diagramme[9][i] = EXTERIEUR;
    p.diagramme[i][0] = EXTERIEUR;
    p.diagramme[i][9] = EXTERIEUR;
  }
  for (i = A; i <= H; i++)
  {
    for (j = r1; j <= r8; j++)
    {
      p.diagramme[i][j] = VIDE;
    }
  }
  p.trait = AUCUNE;
  p.pep = 0;
  p.ccoups = 0.0;
  p.roques = 0;
  afficherPosition(p, 0);
  while (codeFEN[index] != 0)
  {
    if (codeFEN[index] == ' ')
    {
      champ++;
    }
    if (codeFEN[index] == '/' && champ == 1)
    {
      ligne--;
      colonne = 1;
    }
    if (isdigit(codeFEN[index]))
    {
      if (champ == 1)
      {
        colonne = colonne + codeFEN[index] - 48;
      }
      if (champ == 5)
      {
        iccoups = iccoups * 10 + (codeFEN[index] - 48);
      }
    }
    if (isalpha(codeFEN[index]))
    {
      switch (champ)
      {
      case 1:
        switch (toupper(codeFEN[index]))
        {
        case 'P':
          p.diagramme[colonne][ligne] = PION;
          break;
        case 'N':
          p.diagramme[colonne][ligne] = CAVALIER;
          break;
        case 'B':
          p.diagramme[colonne][ligne] = FOU;
          break;
        case 'R':
          p.diagramme[colonne][ligne] = TOUR;
          break;
        case 'Q':
          p.diagramme[colonne][ligne] = DAME;
          break;
        case 'K':
          p.diagramme[colonne][ligne] = ROI;
          break;
        }
        if (islower(codeFEN[index]))
        {
          p.diagramme[colonne][ligne] = p.diagramme[colonne][ligne] * NOIR;
        }
        colonne++;
        break;
      case 2:
        if (codeFEN[index] == 'w')
        {
          p.trait = BLANC;
        }
        if (codeFEN[index] == 'b')
        {
          p.trait = NOIR;
        }
        break;
      case 3:
        switch (codeFEN[index])
        {
        case 'K':
          p.roques = p.roques + PRB;
          break;
        case 'Q':
          p.roques = p.roques + GRB;
          break;
        case 'k':
          p.roques = p.roques + PRN;
          break;
        case 'q':
          p.roques = p.roques + GRN;
          break;
        }
        break;
      case 4:
        p.pep = codeFEN[index] - 96;
        break;
      }
    }
    index++;
  }
  p.ccoups = (float)(iccoups) / 2;
  p.possibles = NULL;
  p.eval = VIDE;
  afficherPosition(p, 0);
  return p;
}

struct Partie reglerPartie(struct Cadence c, char* FEN, short couleur)
{
  struct Partie p;

  p.nom = "Hippocampe";
  p.posInit = convFEN(FEN);
  p.posAct = p.posInit;
  p.couleur = couleur;
  p.type_opp = HUMAIN;
  p.nom_opp = NULL;
  p.temps = c.base1; // Initialisation de la pendule
  p.resultat = '*';
  p.coups = initListe(1000);

  return p;
}

void reglerAdv(char* nom, int type)
{
  enCours.nom_opp = nom;
  enCours.type_opp = type;
}

struct Coup obtenirCoup(char* arg)
{
  struct Coup c;

  c.cd = 0;
  c.ld = 0;
  c.ca = 0;
  c.la = 0;
  c.spec = 0;

  if (strlen(arg) == 4 || strlen(arg) == 5)
  {
    if (arg[0] >= 'a' && arg[0] <= 'h')
      c.cd = arg[0] - 96;
    if (arg[1] >= '1' && arg[1] <= '8')
      c.ld = arg[1] - 48;
    if (arg[2] >= 'a' && arg[2] <= 'h')
      c.ca = arg[2] - 96;
    if (arg[3] >= '1' && arg[3] <= '8')
      c.la = arg[3] - 48;
    // Prise en passant ?
    if ((c.cd != c.ca) && enCours.posAct.diagramme[c.ca][c.la] == VIDE
        && abs(enCours.posAct.diagramme[c.cd][c.ld]) == PION)
      c.spec = 1;
    // Roque ?
    if (((c.ca - c.cd == 2) || (c.ca - c.cd == -2)) &&
        abs(enCours.posAct.diagramme[c.cd][c.ld]) == ROI)
    {
      if (c.ca == G)
      {
        c.spec = 11 - ((-2 * c.ld / 7) + (9 / 7));
      }
      else
      {
        c.spec = 12 - ((-2 * c.ld / 7) + (9 / 7));
      }
    }
    if (strlen(arg) == 5)
    {
      printf("%d%c", arg[4], '\n');
      switch (arg[4])
      {
      case 'q':
        c.spec = DAME;
        break;
      case 'r':
        c.spec = TOUR;
        break;
      case 'b':
        c.spec = FOU;
        break;
      case 'n':
        c.spec = CAVALIER;
        break;
        /*default:
          c.spec = INDEFINI;*/
      }
    }
  }
  return c;
}

struct Position entrerCoup(struct Position p, struct Coup c)
{
  struct Position p1 = p;

  // En cas de prise ou de poussee de pion
  if (p1.diagramme[c.ca][c.la] != VIDE || abs(p1.diagramme[c.cd][c.ld] == PION))
  {
    p1.ccoups = 0;
  }
  else
  {
    p1.ccoups = p1.ccoups + 0.5;
  }

  /* Si le roi a bougé, le roque n'est plus possible */
  if (p1.diagramme[c.cd][c.ld] == ROI * BLANC)
  {
    p1.roques = p1.roques & 0x000a;
  }
  if (p1.diagramme[c.cd][c.ld] == ROI * NOIR)
  {
    p1.roques = p1.roques & 0x0005;
  }

  /* Pour la tour aussi */
  if ((p1.diagramme[c.cd][c.ld] == TOUR * BLANC && c.cd == H && c.ld == r1) || (c.ca == H && c.la == r1))
    p1.roques = p1.roques & 0x000e;
  if ((p1.diagramme[c.cd][c.ld] == TOUR * BLANC && c.cd == A && c.ld == r1) || (c.ca == A && c.la == r1))
    p1.roques = p1.roques & 0x000b;
  if ((p1.diagramme[c.cd][c.ld] == TOUR * NOIR && c.cd == H && c.ld == r8) || (c.ca == H && c.la == r8))
    p1.roques = p1.roques & 0x000d;
  if ((p1.diagramme[c.cd][c.ld] == TOUR * NOIR && c.cd == A && c.ld == r8) || (c.ca == A && c.la == r8))
    p1.roques = p1.roques & 0x0007;

  p1.diagramme[c.ca][c.la] = p1.diagramme[c.cd][c.ld];
  p1.diagramme[c.cd][c.ld] = VIDE;

  switch (c.spec)
  {
  case -1:    /* Un pion vient d'avancer de 2 cases */
    p1.pep = c.cd;
    break;
  case 0:     /* Valeur par défaut */
    p1.pep = 0;
    break;
  case 1:     /* Prise en passant (à revoir) */
    if (p1.pep != 0)
    {
      p1.diagramme[p1.pep][(9 + p1.trait) / 2] = VIDE;
    }
    break;
  default:
    if (c.spec < 6)   /* Promotion */
    {
      p1.diagramme[c.ca][c.la] = c.spec * p1.trait;
      p1.pep = 0;
    }
    else    /* Roque */
    {
      switch (c.spec)
      {
      case 10:
        p1.diagramme[H][r1] = VIDE;
        p1.diagramme[F][r1] = TOUR * BLANC;
        p1.roques = p1.roques & 0x000a;
        break;
      case 11:
        p1.diagramme[A][r1] = VIDE;
        p1.diagramme[D][r1] = TOUR * BLANC;
        p1.roques = p1.roques & 0x000a;
        break;
      case 12:
        p1.diagramme[H][r8] = VIDE;
        p1.diagramme[F][r8] = TOUR * NOIR;
        p1.roques = p1.roques & 0x0005;
        break;
      case 13:
        p1.diagramme[A][r8] = VIDE;
        p1.diagramme[D][r8] = TOUR * NOIR;
        p1.roques = p1.roques & 0x0005;
        break;
      }
    }
  }
  p1.trait = -p.trait;

  return p1;
}
