# Application de gestion d'un lieu culturel

## Compilation

### Windows (MinGW / MSYS2)

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\lieu_culturel.exe
```

### Linux / macOS

```bash
cmake -S . -B build
cmake --build build
./build/lieu_culturel
```

> Si le dossier `build` existe déjà avec une autre configuration, le supprimer avant de relancer

Lancer l'exécutable **depuis la racine du projet** : `programme.txt` est lu et sauvegardé dans le dossier courant.

## Structure du projet

```text
.
├── CMakeLists.txt
├── programme.txt     données chargées et sauvegardées par le programme
├── include/          fichiers .hpp
├── src/              fichiers .cpp (dont main.cpp)
└── tests/            tests unitaires Google Test
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
La date est au format `AAAA-MM-JJ` et l'heure au format `HH:MM`.

## Fonctionnalites

- Affichage du programme et recherche par titre.
- Affichage du programme à une date.
- Programmation avec contrôle des conflits dans une même salle.
- Réservation avec contrôle de la capacité.
- Calcul des recettes et statistiques génériques (prix, durées, réservations, salles les plus fréquentées).
- Ajout administratif de salles et d'événements.
- Sauvegarde et rechargement du programme.
- Les erreurs de lecture indiquent le fichier et le numéro de ligne.

## Tests unitaires (Google Test)

Désactivés par défaut. Google Test est téléchargé par CMake (connexion requise la première fois).
Les tests sont dans le dossier `tests/` :

- `test_event.cpp` : validation des événements (date, heure, durée, capacité...) et réservation de places
- `test_venue.cpp` : salles, programmation, conflits d'horaires, réservations

Google Test est téléchargé automatiquement par CMake (connexion internet requise la première fois).

```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build --target unit_tests
cd build
ctest --output-on-failure
```

Si le dossier `build` existe déjà avec une autre configuration, le supprimer avant de relancer.
Résultat attendu : `100% tests passed`. Pour ajouter un fichier de test, le lister dans `add_executable(unit_tests ...)` de `CMakeLists.txt`.

## Documentation

- `UML.md` : diagramme de classes et cas d'utilisation
- `DECISIONS.md` : décisions de conception et résultat de l'ajout d'un nouveau type d'événement