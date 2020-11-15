/*
 *  regles.h
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

#ifndef REGLES_H
#define REGLES_H

#include "hippocampe.h"

#define BLANC 1
#define NOIR -1
#define AUCUNE 0

#define depart 5
#define arrivee_gr 3
#define arrivee_pr 7
#define di_gr 2
#define di_pr 6
#define fi_gr 4
#define fi_pr 7

#define r1 1
#define r2 2
#define r3 3
#define r4 4
#define r5 5
#define r6 6
#define r7 7
#define r8 8

#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8

#define PION 1
#define CAVALIER 2
#define FOU 3
#define TOUR 4
#define DAME 5
#define ROI 6

#define VIDE 0
#define EXTERIEUR -100

#define INDEFINI -100

// Possibilités de roques
#define PRB 1
#define PRN 2
#define GRB 4
#define GRN 8

void coupsPion(struct Position * p, int col, int lig);
void coupsFtd(struct Position * p, int col, int lig, int dep, int arr, int inc);
void coupsCav(struct Position * p, int col, int lig);
void coupsRoi(struct Position * p, int col, int lig);
int verifierCase(struct Position p, short caseTest[2], short couleur);
int verifierCoup(struct Position p, struct Coup c);
struct Position ajouterCoupPossible(struct Position * p, struct Coup c);
char resultat(struct Position p);

#endif
