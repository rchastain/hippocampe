/*
 *  recherche.h
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

#ifndef RECHERCHE_H
#define RECHERCHE_H

#include "liste.h"
#include "partie.h"

void *rechercher(struct Position *p, int affich);
typedef int EvalFunc[10];	    // valeurs des paramètres
struct MVar alphabeta(struct Position p, int profondeur, int niveau, short a, short b, int affich);
void chercherCoups(struct Position * p, int affich);
short evalStat(struct Position p);

#define MAT 32000    // équivalent au mat.

#endif
