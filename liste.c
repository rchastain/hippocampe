/*
 *  liste.c
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

#include <stdlib.h>
#include <stdio.h>

#include "hippocampe.h"

struct Coup definirCoup(short cd, short ld, short ca, short la, short type)
{
  struct Coup c;

  c.ld = ld;
  c.la = la;
  c.cd = cd;
  c.ca = ca;
  c.spec = type;

  return c;
}

struct ListeCoups initListe(short longueurMax)
{
  struct ListeCoups coups;

  coups.longueur = 0;
  coups.liste = malloc(longueurMax * sizeof(struct Coup));

  return coups;
}

struct MVar ajouterCoup(struct MVar *mv, struct Coup c)
{
  int i;

  for (i = mv->longueur - 1; i >= 0; i--)
  {
    mv->variante[i + 1] = mv->variante[i];
  }
  mv->variante[0] = c;
  mv->longueur++;

  return *mv;
}
