# Application de gestion d'un lieu culturel

## Compilation avec g++

Depuis le dossier du projet :

```bash
g++ -Wall -Wextra -std=c++17 -Iinclude src/*.cpp -o lieu_culturel
./lieu_culturel
```

Le fichier `programme.txt` doit rester à la racine du programme.

## Compilation avec CMake

```bash
cmake -S . -B build
cmake --build build
./build/lieu_culturel
```

## Administration

Le code administrateur de cette version de démonstration est `CULT`.
Il est défini dans `main.cpp`.

## Types d'evenements

- Concert : artistes et durée du sound-check.
- Théâtre : durée de l'entracte.
- Projection : classification.
- Atelier : animateur.

## Format du fichier programme.txt

Une ligne par objet, séparateur `;`.

```text
ROOM;id;nom;capacite
CONCERT;titre;date;heure;duree;salle;capacite;prix;reservations;artistes;soundcheck
PLAY;titre;date;heure;duree;salle;capacite;prix;reservations;entracte
SCREENING;titre;date;heure;duree;salle;capacite;prix;reservations;classification
WORKSHOP;titre;date;heure;duree;salle;capacite;prix;reservations;animateur
```

Les champs texte ne doivent pas contenir le caractère `;`.

## Fonctionnalites

- Affichage du programme et recherche par titre.
- Affichage du programme à une date.
- Programmation avec contrôle des conflits dans une même salle.
- Réservation avec contrôle de la capacité.
- Calcul des recettes et statistiques génériques.
- Ajout administratif de salles et d'événements.
- Sauvegarde et rechargement du programme.
- Les erreurs de lecture indiquent le fichier et le numéro de ligne.
