/*
 *  partie.h
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

#ifndef PARTIE_H
#define PARTIE_H

#include <stdio.h>
#include "hippocampe.h"
#include "recherche.h"
#include "temps.h"

struct Partie reglerPartie(struct Cadence c, char* FEN, short couleur);
struct Position convFEN(char* codeFEN);
FILE* ecrirePGN(char* fichier);
void reglerAdv(char* nom, int type);
struct Coup obtenirCoup(char* arg);
struct Position entrerCoup(struct Position p, struct Coup c);
void jouerCoup(struct Coup c, struct Partie p);

#endif
