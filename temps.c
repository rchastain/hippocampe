/*
 *  temps.c
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
#include <sys/time.h>
#include <time.h>

#include "hippocampe.h"
#include "sortie.h"
#include "temps.h"
#include "entree.h"

#define max(a, b) ((a > b) ? a : b)

extern struct Partie enCours;

struct Cadence reglerCadence(struct Cadence *c, int param, int val)
{
  switch (param)
  {
  case BASE1 :
    c->base1 = val;
    break;
  case INC1 :
    c->inc = val;
    break;
  case N1 :
    c->n1 = val;
    break;
  default :
    break;
  }
  return *c;
}

struct Cadence initCadence()
{
  struct Cadence c;

  c.base1 = 300;
  c.inc = 0;
  c.n1 = 40;

  return c;
}

float allouer(struct Cadence c, float tempsCourant)
{
  float tempsAlloue;
  float lambda1 = 100, lambda2 = 0.5;

  //printf("Nombre de coups : %d\n", c.n1);

  if (c.n1 == 0) // Cadence KO
  {
    tempsAlloue = max((tempsCourant - c.base1 / 20) / lambda1, 0);
  }
  else // Cadence "Tournoi"
  {
    tempsAlloue = lambda2 * (tempsCourant / c.n1);
  }

  //printf("Temps alloué = %f secondes\n", tempsAlloue);
  return tempsAlloue;
}

int alarme(struct timeval t0, float alloue)
{
  struct timeval t1;
  float tempsEcoule = 0;

  gettimeofday(&t1, NULL);

  tempsEcoule = (float)(t1.tv_sec - t0.tv_sec) + (float)(t1.tv_usec - t0.tv_usec) / 1000000;
  //printf("Temps écoulé = %f Temps alloué = %f\n", tempsEcoule, alloue);
  if (tempsEcoule < alloue)
  {
    return 0;
  }
  else
  {
    enCours.temps = enCours.temps - tempsEcoule;
    return 1;
  }
}

void synchroniser(float time)
{
  float diff = enCours.temps - time;
  enCours.temps = time;
}
