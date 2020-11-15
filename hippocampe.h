/*
 *  hippocampe.h
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

#ifndef HIPPOCAMPE_H
#define HIPPOCAMPE_H

struct Coup
{
  short cd;   // Colonne de départ
  short ld;   // Rangée de départ
  short ca;   // Colonne d'arrivée
  short la;   // Rangée d'arrivée
  short spec; // Spécificité (promotion, prise en passant)
};

struct ListeCoups // Liste chaînée
{
  struct Coup * liste;
  short longueur;
};

struct Position
{
  short diagramme[10][10];
  short trait;
  short pep;
  float ccoups;
  short roques;            // Nombre de 0 à 15 indiquant les roques encore possibles
  struct Coup * possibles; // Liste des coups légaux
  short nb_possibles;      // Nombre de coups possibles
  short eval;              // Évaluation de la position
  short departage;         // Champ aléatoire utilisé pour départager deux positions de même valeur
};

struct MVar
{
  struct Position orig;     // Pointeur sur la position d'origine
  struct Coup variante[20]; // Meilleure variante
  short longueur;
};

struct Partie
{
  char* nom;                // Nom du joueur (programme)
  short couleur;            // Couleur du joueur (programme)
  short type_opp;           // Type d'adversaire (Hippocampe, ordinateur, humain)
  char* nom_opp;            // Nom de l'adversaire
  float temps;              // Temps restant (en secondes)
  struct Position posInit;  // Position initiale
  struct Position posAct;
  struct ListeCoups coups;  // Liste des coups joués
  char resultat;
};

#define HUMAIN 1
#define ORDINATEUR 2

#define POS_INIT "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define VRAI 1
#define FAUX 0

#define VERSION "0.4.2"
#define REVISION "0.1"

#endif
