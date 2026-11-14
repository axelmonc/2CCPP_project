# Application de gestion d'un lieu culturel

## Compilation

### Windows (MinGW / MSYS2)

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON
cmake --build build
cd build
ctest --output-on-failure
```

### Linux / macOS

```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
cd build && ctest --output-on-failure
```

> Si le dossier `build` existe déjà avec une autre configuration, le supprimer avant de relancer

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

## Tests unitaires (Google Test)

Les tests sont dans le dossier `tests/` :

- `test_event.cpp` : validation des événements (date, heure, durée, capacité...) et réservation de places
- `test_venue.cpp` : salles, programmation, conflits d'horaires, réservations

Google Test est téléchargé automatiquement par CMake (connexion internet requise la première fois).
Les tests sont désactivés par défaut : la compilation normale ne dépend pas de Google Test.

### Lancer les tests (Windows, MinGW)

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON
cmake --build build --target unit_tests
cd build
ctest --output-on-failure
cd ..
```

### Lancer les tests (Linux / macOS)

```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build --target unit_tests
cd build && ctest --output-on-failure
```

Si le dossier `build` existe déjà avec une autre configuration, le supprimer avant de relancer.
Résultat attendu : `100% tests passed`.

### Ajouter un test

Dans un fichier de `tests/`, ajouter un cas :

```cpp
TEST(NomDuGroupe, NomDuTest) {
    EXPECT_EQ(1 + 1, 2);
}
```

Assertions utiles : `EXPECT_EQ`, `EXPECT_TRUE`, `EXPECT_FALSE`, `EXPECT_DOUBLE_EQ`, `EXPECT_THROW`.
Pour un nouveau fichier de test, l'ajouter dans la liste `add_executable(unit_tests ...)` de `CMakeLists.txt`.