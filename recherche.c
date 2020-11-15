/*
 *  recherche.c
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

#include "hippocampe.h"
#include "regles.h"
#include "recherche.h"
#include "sortie.h"
#include "temps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)

extern struct MVar mvprov;
extern int prof_max;
extern struct Partie enCours;
extern struct Cadence cad;

void *rechercher(struct Position *p)
{
  int i, prof_act = 1;
  int temps_ecoule = 0;
  float tempsAlloue;
  struct timeval t0;
  char coup[6], res;
  char chaine[12] = "move ";

  //printf("%s%d\n", "trait=", p->trait);
  //afficherPosition(p);

  gettimeofday(&t0, NULL);
  tempsAlloue = allouer(cad, enCours.temps);
  /* Recherche, par un algorithme alpha-beta, sur une profondeur
   * croissante */
  while (prof_act <= prof_max && !temps_ecoule)
  {
    //printf("%s%d%c", "Profondeur de recherche : ", prof_act, '\n');
    mvprov = alphabeta(*p, prof_act, 0, -MAT, MAT);
    //afficherEval(mvprov);
    temps_ecoule = alarme(t0, tempsAlloue);
    prof_act++;
  }

  coup[0] = 96 + mvprov.variante[0].cd;
  coup[1] = 48 + mvprov.variante[0].ld;
  coup[2] = 96 + mvprov.variante[0].ca;
  coup[3] = 48 + mvprov.variante[0].la;
  coup[4] = '\0';
  
  /* v0.4.2.0.1 */
  coup[5] = '\0';
  if (mvprov.variante[0].spec == DAME)
  {
    coup[4] = 'q';
  }
  if (mvprov.variante[0].spec == TOUR)
  {
    coup[4] = 'r';
  }
  if (mvprov.variante[0].spec == FOU)
  {
    coup[4] = 'b';
  }
  if (mvprov.variante[0].spec == CAVALIER)
  {
    coup[4] = 'n';
  }
  
  strcat(chaine, coup);
  *p = entrerCoup(*p, mvprov.variante[0]);
  afficherPosition(*p, 0);
  envoi(chaine);
  
  chercherCoups(p, 0);
  //printf("Nombre de coups possibles : %d\n", p->nb_possibles);
  res = resultat(*p);
  if (res == 'B')
  {
    envoi("1-0 {White mates}");
  }
  if (res == 'N')
  {
    envoi("0-1 {Black mates}");
  }
  if (res == 'C')
  {
    envoi("1/2-1/2 {50 move rule}");
  }
  if (res == 'M')
  {
    envoi("1/2-1/2 {Insufficient material}");
  }
  if (res == 'P')
  {
    envoi("1/2-1/2 {Stalemate}");
  }
  if (res == 'R')
  {
    envoi("1/2-1/2 {Draw by repetition}");
  }
  //enCours.couleur = AUCUNE;
}

struct MVar alphabeta(struct Position p, int prof, int niveau, short a, short b)
{
  int i;
  struct MVar mv, mv1;
  struct Position p1;
  struct Coup meilleur;
  short alpha = -MAT, beta = MAT, refdep = -32768;
  char res;

  mv1.orig = p;
  if (niveau == prof) // Extrémité de l'arbre.
  {
    mv1.orig.eval = evalStat(p);
    mv1.orig.departage = rand();
    //printf("dep=%d\n", mv1.orig.departage);
    mv1.longueur = 0;
    return mv1;
  }
  else
  {
    chercherCoups(&p, 0);
    res = resultat(p);
    if (isupper(res)) /* car les résultats non définitifs ne sont pas représentés par une lettre majuscule */
    {
      mv1.orig.departage = rand();
      mv1.longueur = 0;
      if (res == 'B')
      {
        mv1.orig.eval = MAT - niveau;
      }
      if (res == 'N')
      {
        mv1.orig.eval = -(MAT - niveau);
      }
      if (res == 'C' || res == 'M' || res == 'P' || res == 'R')
      {
        mv1.orig.eval = 0;
      }
      return mv1;
    }
    meilleur = p.possibles[0];

    if (p.trait == NOIR) // noeud Min
    {
      for (i = 0; i < p.nb_possibles; i++)
      {
        //printf("%s%d %s%d %s%d %s%d%c", "cd=", p.possibles[i].cd, "ld=", p.possibles[i].ld, "ca=", p.possibles[i].ca, "la=", p.possibles[i].la, '\n');
        p1 = entrerCoup(p, p.possibles[i]);
        mv = alphabeta(p1, prof, niveau + 1, a, min(b, beta));

        if (niveau <= 0)
        {
          //printf("%s%d %s%d %c%c%c%c %s%d %s%d %s%d%c", "prof=", prof, "i=", i, 96 + p.possibles[i].cd, 48 + p.possibles[i].ld, 96 + p.possibles[i].ca, 48 + p.possibles[i].la, "eval=", mv.orig.eval, "alpha=", alpha, "beta=", beta, '\n');
        }
        if (mv.orig.eval < beta)
        {
          beta = mv.orig.eval;
          refdep = mv.orig.departage;
          meilleur = p.possibles[i];
          if (a > beta)
          {
            //printf("%s%d %s%d %s%d %s%d%c", "cd=", meilleur.cd, "ld=", meilleur.ld, "ca=", meilleur.ca, "la=", meilleur.la, '\n');
            mv1 = ajouterCoup(&mv, meilleur);
            mv1.orig.eval = beta;
            mv1.orig.departage = refdep;
            p.nb_possibles = 0;
            //afficherEval(mv1);
            free(p.possibles);
            return mv1;
          }
        }
        if (mv.orig.eval == beta && mv.orig.departage > refdep)
        {
          refdep = mv.orig.departage;
          meilleur = p.possibles[i];
        }
      }
      //printf("%s%d %s%d %s%d %s%d%c", "cd=", meilleur.cd, "ld=", meilleur.ld, "ca=", meilleur.ca, "la=", meilleur.la, '\n');
      mv1 = ajouterCoup(&mv, meilleur);
      mv1.orig.eval = beta;
      mv1.orig.departage = refdep;
      p.nb_possibles = 0;
      //afficherEval(mv1);
      free(p.possibles);
      return mv1;
    }
    else // noeud Max
    {
      for (i = 0; i < p.nb_possibles; i++)
      {
        //printf("%s%d %s%d %s%d %s%d%c", "cd=", p.possibles[i].cd, "ld=", p.possibles[i].ld, "ca=", p.possibles[i].ca, "la=", p.possibles[i].la, '\n');
        p1 = entrerCoup(p, p.possibles[i]);
        mv = alphabeta(p1, prof, niveau + 1, max(a, alpha), b);

        if (niveau <= 0)
        {
          //printf("%s%d %s%d %c%c%c%c %s%d %s%d %s%d%c", "prof=", prof, "i=", i, 96 + p.possibles[i].cd, 48 + p.possibles[i].ld, 96 + p.possibles[i].ca, 48 + p.possibles[i].la, "eval=", mv.orig.eval, "alpha=", alpha, "beta=", beta, '\n');
        }
        if (mv.orig.eval > alpha)
        {
          alpha = mv.orig.eval;
          refdep = mv.orig.departage;
          meilleur = p.possibles[i];
          if (alpha > b)
          {
            //printf("%s%d %s%d %s%d %s%d%c", "cd=", meilleur.cd, "ld=", meilleur.ld, "ca=", meilleur.ca, "la=", meilleur.la, '\n');
            mv1 = ajouterCoup(&mv, meilleur);
            mv1.orig.eval = alpha;
            mv1.orig.departage = refdep;
            p.nb_possibles = 0;
            //afficherEval(mv1);
            free(p.possibles);
            return mv1;
          }
        }
        if (mv.orig.eval == alpha && mv.orig.departage > refdep)
        {
          refdep = mv.orig.departage;
          meilleur = p.possibles[i];
        }
      }
      //printf("%s%d %s%d %s%d %s%d%c", "cd=", meilleur.cd, "ld=", meilleur.ld, "ca=", meilleur.ca, "la=", meilleur.la, '\n');
      mv1 = ajouterCoup(&mv, meilleur);
      mv1.orig.eval = alpha;
      mv1.orig.departage = refdep;
      p.nb_possibles = 0;
      //afficherEval(mv1);
      free(p.possibles);
      return mv1;
    }
  }
}

void chercherCoups(struct Position * p, int affich)
{
  int i, j;
  
  p->possibles = malloc(200 * sizeof(struct Coup));
  p->nb_possibles = 0;

  for (i = A; i <= H; i++)
  {
    for (j = r1; j <= r8; j++)
    {
      switch (p->diagramme[i][j]*p->trait)
      {
      case PION:
        coupsPion(p, i, j);
        break;
      case CAVALIER:
        coupsCav(p, i, j);
        break;
      case FOU:
        coupsFtd(p, i, j, 1, 7, 2);
        break;
      case TOUR:
        coupsFtd(p, i, j, 0, 6, 2);
        break;
      case DAME:
        coupsFtd(p, i, j, 0, 7, 1);
        break;
      case ROI:
        coupsRoi(p, i, j);
        break;
      }
    }
  }
  if (affich)
  {
    printf("Nombre de coups : %d\n", p->nb_possibles);
    for (i = 0; i < p->nb_possibles; i++)
    {
      printf("%d%d%d%d\n", p->possibles[i].cd, p->possibles[i].ld, p->possibles[i].ca, p->possibles[i].la);
    }
  }
}

short evalStat(struct Position p)
{
  short eval = 0;
  int i, j;

  EvalFunc func = {100, 300, 325, 500, 900, -100, -300, -325, -500, -900};

  for (i = A; i <= H; i++)
  {
    for (j = r1; j <= r8; j++)
    {
      if (p.diagramme[i][j] > 0 && p.diagramme[i][j] <= 5)
      {
        eval = eval + func[p.diagramme[i][j] - 1];
      }
      if (p.diagramme[i][j] < 0 && p.diagramme[i][j] >= -5)
      {
        eval = eval + func[4 - p.diagramme[i][j]];
      }
    }
  }
  //afficherPosition(p);
  //printf("%s%d%c", "evalpos : ", eval, '\n');
  return eval;
}
