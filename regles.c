/*
 *  regles.c
 *  Author(s) : Jérémy Pages <jeremy.pages@eleves.ec-nantes.fr>
 *  Copyright (C) 2008 Jérémy Pages
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
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

#include "liste.h"
#include "regles.h"
#include "partie.h"
#include "hippocampe.h"

#include <stdio.h>
#include <stdlib.h>

short depVer[8] = {1, 1, 0,-1,-1,-1, 0, 1};
short depHor[8] = {0, 1, 1, 1, 0,-1,-1,-1};
short cavVer[8] = {2, 1,-1,-2,-2,-1, 1, 2};
short cavHor[8] = {1, 2, 2, 1,-1,-2,-2,-1};

void coupsPion(struct Position * p, int col, int lig)
{
  int k, piece;
  int hauteur = (5 + (2 * lig - 9) * p->trait) / 2;
  struct Coup c;

  if (hauteur != 5)	/* le pion n'est pas sur la septième rangée. */
  {
    if (p->diagramme[col][lig + p->trait] == VIDE)
    {
      c = definirCoup(col, lig, col, lig + p->trait, 0);
      //printf("Definition du coup\n");
      *p = ajouterCoupPossible(p, c);
      //printf("Avance d'une case\n");
    }
    for (k = -1; k <= 1; k = k + 2) /* Prise */
    {
      if (p->diagramme[col + k][lig + p->trait]*p->trait < 0 && p->diagramme[col + k][lig + p->trait]*p->trait > -7)
      {
        c = definirCoup(col, lig, col + k, lig + p->trait, 0);
        *p = ajouterCoupPossible(p, c);
      }
    }
    if (hauteur == 0 && p->diagramme[col][lig + p->trait] == 0 && p->diagramme[col][lig + 2 * p->trait] == 0) /* Avance de 2 cases */
    {
      c = definirCoup(col, lig, col, lig + 2 * p->trait, -1);
      *p = ajouterCoupPossible(p, c);
    }
    if (hauteur == 3 && (col == p->pep + 1 || col == p->pep - 1) && p->pep != 0) /* Prise en passant */
    {
      c = definirCoup(col, lig, p->pep, lig + p->trait, 1);
      *p = ajouterCoupPossible(p, c);
    }
  }
  else
  {
    if (p->diagramme[col][lig + p->trait] == VIDE)
    {
      for (piece = CAVALIER; piece <= DAME; piece++)
      {
        c = definirCoup(col, lig, col, lig + p->trait, piece);
        *p = ajouterCoupPossible(p, c);
      }
    }
    for (k = -1; k <= 1; k = k + 2)
    {
      if (p->diagramme[col + k][lig + p->trait]*p->trait < 0 && p->diagramme[col + k][lig + p->trait]*p->trait > -7)
      {
        for (piece = CAVALIER; piece <= DAME; piece++)
        {
          c = definirCoup(col, lig, col + k, lig + p->trait, piece);
          *p = ajouterCoupPossible(p, c);
        }
      }
    }
  }
}

void coupsFtd(struct Position * p, int col, int lig, int dep, int arr, int inc)
{
  int i, k;
  struct Coup c;

  //afficherPosition(*p);

  for (i = dep; i <= arr; i = i + inc)
  {
    //printf("%s%d%c","direction ",i,'\n');
    k = 1;
    while (p->diagramme[col + k * depHor[i]][lig + k * depVer[i]] == VIDE)
    {
      c = definirCoup(col, lig, col + k * depHor[i], lig + k * depVer[i], 0);
      *p = ajouterCoupPossible(p, c);
      k++;
    }
    //printf("%s%d%c%d%c","Phase 1 ",col+k*depHor[i],' ',lig+k*depVer[i],'\n');
    if (p->diagramme[col + k * depHor[i]][lig + k * depVer[i]]*p->trait > -7 &&
        p->diagramme[col + k * depHor[i]][lig + k * depVer[i]]*p->trait < 0)
    {
      c = definirCoup(col, lig, col + k * depHor[i], lig + k * depVer[i], 0);
      *p = ajouterCoupPossible(p, c);
    }
  }
}

void coupsCav(struct Position * p, int col, int lig)
{
  int i;
  struct Coup c;

  for (i = 0; i <= 7; i++)
  {
    if (col + cavHor[i] >= A && col + cavHor[i] <= H && lig + cavVer[i] >= r1 && lig + cavVer[i] <= r8)
    {
      if (p->diagramme[col + cavHor[i]][lig + cavVer[i]]*p->trait <= 0 &&
          p->diagramme[col + cavHor[i]][lig + cavVer[i]]*p->trait > -7 )
      {
        /* On ajoute le coup à la liste */
        c = definirCoup(col, lig, col + cavHor[i], lig + cavVer[i], 0);
        *p = ajouterCoupPossible(p, c);
      }
    }
  }
}

void coupsRoi(struct Position * p, int col, int lig)
{
  int i, k1, k2, roquePossible = 1;
  int arrivee, di, fi, mouvement;
  short caseRoi[2];
  struct Coup c;

  for (i = 0; i <= 7; i++)
  {
    if (p->diagramme[col + depHor[i]][lig + depVer[i]]*p->trait > -7 &&
        p->diagramme[col + depHor[i]][lig + depVer[i]]*p->trait <= 0)
    {
      c = definirCoup(col, lig, col + depHor[i], lig + depVer[i], 0);
      *p = ajouterCoupPossible(p, c);
    }
  }

  /* Roque */
  for (k1 = 1 - p->trait; k1 <= 2 - p->trait; k1++)
  {
    /* Petit roque */
    if (k1 == 1 - p->trait)
    {
      arrivee = arrivee_pr;
      di = di_pr;
      fi = fi_pr;
      if (p->trait == BLANC)
      {
        mouvement = p->roques & PRB;
      }
      else
      {
        mouvement = p->roques & PRN;
      }
    }
    /* Grand roque */
    if (k1 == 2 - p->trait)
    {
      arrivee = arrivee_gr;
      di = di_gr;
      fi = fi_gr;
      if (p->trait == BLANC)
      {
        mouvement = p->roques & GRB;
      }
      else
      {
        mouvement = p->roques & GRN;
      }
    }
    if (mouvement)
    {
      /* Vérifier que toutes les cases entre le roi et la tour sont vides. */
      for (k2 = di; k2 <= fi; k2++)
      {
        if (p->diagramme[k2][(9 - 7 * p->trait) / 2] != VIDE)
        {
          roquePossible = 0;
        }
      }
      if (roquePossible)
      {
        /* Vérifier qu'aucune des cases sur lesquelles le roi doit passer n'est contrôlée par l'adversaire. */
        k2 = depart;
        while (k2 != arrivee)
        {
          caseRoi[0] = k2;
          caseRoi[1] = (9 - 7 * p->trait) / 2;
          if (verifierCase(*p, caseRoi, p->trait))
          {
            roquePossible = 0;
          }
          k2 = k2 + (arrivee - depart) / 2;
        }
        if (roquePossible) /* v0.4.2.0.1 */
        {
          c = definirCoup(depart, (9 - 7 * p->trait) / 2, arrivee, (9 - 7 * p->trait) / 2, 10 + k1);
          *p = ajouterCoupPossible(p, c);
        }
      }
    }
  }
}

// détection du contrôle d'une case

int verifierCase(struct Position p, short caseTest[2], short couleur)
{
  int dir, k, k1, k2, caseControlee;
  short depVer[8] = {1, 1, 0,-1,-1,-1, 0, 1};
  short depHor[8] = {0, 1, 1, 1, 0,-1,-1,-1};
  short cavVer[8] = {2, 1,-1,-2,-2,-1, 1, 2};
  short cavHor[8] = {1, 2, 2, 1,-1,-2,-2,-1};

  dir = 0;

  while (dir <= 7) // Contrôle des Cavaliers
  {
    if (caseTest[0] + cavHor[dir] > 0 && caseTest[0] + cavHor[dir] < 9 && caseTest[1] + cavVer[dir] > 0 && caseTest[1] + cavVer[dir] < 9)
    {
      if (p.diagramme[caseTest[0] + cavHor[dir]][caseTest[1] + cavVer[dir]] == 2 * couleur)
      {
        return VRAI;
      }
    }
    dir++;
  }
  dir = 0;
  k = 1;

  while (dir <= 7)
  {
    switch ((p.diagramme[caseTest[0] + k * depHor[dir]][caseTest[1] + k * depVer[dir]]) / couleur)
    {
    case VIDE:
      k++;
      if (k > 7)
      {
        dir++;
        k = 1;
      }
      break;
    case PION:
      if (k == 1 && ((-p.trait == BLANC && (dir == 1 || dir == 7)) || (-p.trait == NOIR && (dir == 3 || dir == 5))))
      {
        return VRAI;
      }
      else
      {
        dir++;
        k = 1;
      }
      break;
    case CAVALIER:
      dir++;
      k = 1;
      break;
    case FOU:
      if (dir == 1 || dir == 3 || dir == 5 || dir == 7)
      {
        return VRAI;
      }
      else
      {
        dir++;
        k = 1;
      }
      break;
    case TOUR:
      if (dir == 0 || dir == 2 || dir == 4 || dir == 6)
      {
        return VRAI;
      }
      else
      {
        dir++;
        k = 1;
      }
      break;
    case DAME:
      return VRAI;
      break;
    case ROI:
      if (k == 1)
      {
        return VRAI;
      }
      else
      {
        dir++;
        k = 1;
      }
      break;
    default:
      dir++;
      k = 1;
      break;
    }
  }
  return FAUX;
}

// vérification de la légalité du coup passé en paramètre

int verifierCoup(struct Position p, struct Coup c)
{
  struct Position p1 = entrerCoup(p, c);

  int i = 1, j = 1, legal;
  short caseRoi[2];

  // Repérage du Roi
  while (p1.diagramme[i][j] != -ROI * p1.trait)
  {
    j++;
    if (j == 9)
    {
      i++;
      j = 1;
    }

  }
  caseRoi[0] = i;
  caseRoi[1] = j;
  legal = !verifierCase(p1, caseRoi, p1.trait);

  return legal;
}

// ajout d'un coup, si ce coup est légal.

struct Position ajouterCoupPossible(struct Position * p, struct Coup c)
{
  if (verifierCoup(*p, c))
  {
    p->possibles[p->nb_possibles] = c;
    p->nb_possibles++;
    //printf("%s%d%c","Nombre de coups : ",p->nb_possibles,'\n');
  }
  return *p;
}

char resultat(struct Position p)
{
  int i = 1, j = 1;
  short caseRoi[2];

  if (p.ccoups >= 50.0)
  {
    return 'C';		// Nulle (règle des 50 coups)
  }

  if (p.nb_possibles == 0)
  {
    // Repérage du Roi
    while (p.diagramme[i][j] != ROI * p.trait)
    {
      j++;
      if (j == 9)
      {
        i++;
        j = 1;
      }
    }
    caseRoi[0] = i;
    caseRoi[1] = j;

    if (verifierCase(p, caseRoi, -p.trait))
    {
      if (p.trait == BLANC)
      {
        return 'N';		// Gain noir
      }
      if (p.trait == NOIR)
      {
        return 'B';		// Gain blanc
      }
    }
    else
    {
      return 'P';		// Nulle
    }
  }

  return '*';
}
