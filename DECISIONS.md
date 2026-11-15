# Décisions de conception

## Responsabilités

- `Room` représente une salle et sa capacité.
- `Event` contient les informations communes aux événements, valide ses données dans son constructeur et définit les opérations polymorphes.
- `Concert`, `Play`, `Screening` et `Workshop` portent les informations propres à chaque type.
- `Venue` gère les salles, les événements, les réservations et les questions du directeur. Elle fournit aussi les séries de données (prix, durées, réservations) utilisées par les statistiques.
- `DataManager` est responsable de la lecture et de l'écriture du fichier.
- `Analytics.hpp` contient les fonctions génériques de minimum, maximum et moyenne.
- `main.cpp` contient le menu, la saisie sécurisée et l'administration protégée par code.

## Choix techniques

1. L'héritage public représente la relation « un concert est un événement ».
2. `Event` est abstraite : on ne crée que des événements concrets.
3. `Venue` possède les événements avec `std::unique_ptr<Event>`. Le destructeur virtuel de `Event` permet une destruction correcte.
4. Les salles sont stockées par valeur dans un `std::vector<Room>` : `Venue` en est propriétaire (composition).
5. Les événements sont stockés dans un seul vecteur polymorphe, ce qui évite de modifier `Venue` pour chaque nouveau type.
6. Les statistiques utilisent des templates pour fonctionner avec `double` et `int`. `Analytics.hpp` ne dépend pas de `Venue` : elle travaille sur des `std::vector<T>` que `Venue` fournit (`prices()`, `durations()`, `attendances()`).
7. Le chargement construit un nouvel objet `Venue`. En cas d'erreur, cet objet temporaire est détruit et aucun programme partiellement chargé n'est utilisé.
8. Le fichier indique son type au premier champ. Le chargement choisit la classe concrète à partir de ce tag (fonction `makeEvent`, interne à `DataManager.cpp`).
9. Les données sont validées dans les constructeurs (`Room`, `Event` et ses classes filles) : un objet invalide ne peut pas exister. Les règles qui dépendent de plusieurs objets sont dans `Venue::scheduleEvent` : salle existante, capacité de l'événement inférieure ou égale à celle de la salle, titre unique par date, pas de chevauchement dans une même salle.
10. Le projet est compilé en une bibliothèque statique `lieu_culturel_lib` (tout sauf `main.cpp`), liée à l'exécutable `lieu_culturel` et aux tests. Cela évite de compiler deux fois les mêmes fichiers.

## Extensibilité : ajout d'un type Exhibition

Pour ajouter un type `Exhibition`, il faudrait :
- créer `include/Exhibition.hpp` et `src/Exhibition.cpp` ;
- ajouter `src/Exhibition.cpp` dans la liste de `add_library` de `CMakeLists.txt` ;
- ajouter la création de ce type dans la fonction `makeEvent` de `DataManager.cpp` ;
- ajouter le choix et la saisie de ses informations dans `main.cpp`.

Il n'est pas nécessaire de modifier `Event`, `Venue`, `Room` ou les autres types d'événements. Dans cette version, `Workshop` sert déjà d'exemple d'un type ajouté au même mécanisme.

## Limites connues de cette version pédagogique

- Les champs texte du fichier ne peuvent pas contenir `;`.
- La validation de date contrôle le format, le mois et le nombre de jours (années bissextiles comprises), sans autre règle de calendrier : une date passée est acceptée.
- Les événements qui dépassent minuit sont refusés.
- Le code administrateur `CULT` est un code de démonstration à remplacer avant une utilisation réelle.
- Les réservations sont enregistrées comme un nombre de places, sans identité de client ni numéros de sièges.
- `programme.txt` est lu et écrit dans le dossier courant : le programme doit être lancé depuis la racine du projet.
- La recherche par titre renvoie le premier événement trouvé, même si le même titre existe à plusieurs dates.
- `minimum`, `maximum` et `average` ne sont pas définies sur un vecteur vide : `main.cpp` vérifie qu'il y a des données avant de les appeler.