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
8. Le fichier indique son type au premier champ. Le chargement choisit la classe concrète à partir de ce tag (fonction `makeEvent`, interne à `DataManager.cpp` : elle est dans un espace de noms anonyme, comme `parseInt`, `parseDouble` et `splitLine`).
9. Les données sont validées dans les constructeurs (`Room`, `Event` et ses classes filles) : un objet invalide ne peut pas exister. Les règles qui dépendent de plusieurs objets sont dans `Venue::scheduleEvent` : salle existante, capacité de l'événement inférieure ou égale à celle de la salle, titre unique par date, pas de chevauchement dans une même salle.
10. Le projet est compilé en une bibliothèque statique `lieu_culturel_lib` (tout sauf `main.cpp`), liée à l'exécutable `lieu_culturel` et aux tests. Cela évite de compiler deux fois les mêmes fichiers.

## Résultat de l'ajout d'un nouveau type d'événement : Workshop

Après la conception initiale (Concert, Play, Screening), nous avons ajouté un quatrième type, `Workshop` (atelier, avec un animateur). Le bilan, mesuré dans l'historique git :

| Fichier | Changement |
| --- | --- |
| `include/Workshop.hpp`, `src/Workshop.cpp` | créés (44 lignes au total) |
| `src/DataManager.cpp` | +8 lignes : une branche `WORKSHOP` dans `makeEvent` |
| `src/main.cpp` | une option « 4 Atelier » dans le menu et une branche de saisie dans `addEvent` |
| `CMakeLists.txt` | +1 ligne : `src/Workshop.cpp` dans `add_library` |
| `Event`, `Venue`, `Room`, `Analytics.hpp` | **aucune modification** |

Conséquences concrètes :

- `Venue` ne connaît que `Event` : programmation, détection des conflits, réservations, recettes et statistiques fonctionnent sans changement pour le nouveau type.
- La sauvegarde fonctionne dès que `Workshop::save` est écrite ; seul le chargement a besoin d'une branche, parce qu'il doit choisir la classe à partir du tag.
- Le point qui reste à modifier à la main est `makeEvent` (et le menu de `main.cpp`) : l'ajout d'un type n'est pas totalement automatique. Une table d'enregistrement des types (tag → fonction de création) supprimerait cette étape, mais serait disproportionnée pour quatre types.

Pour un cinquième type (par exemple `Exhibition`), la même recette s'applique : créer les deux fichiers, les ajouter à `CMakeLists.txt`, ajouter la branche dans `makeEvent` et le choix dans `main.cpp`.

## Limites connues de cette version pédagogique

- Les champs texte du fichier ne peuvent pas contenir `;`.
- La validation de date contrôle le format, le mois et le nombre de jours (années bissextiles comprises), sans autre règle de calendrier : une date passée est acceptée.
- Les événements qui dépassent minuit sont refusés.
- Le code administrateur `CULT` est un code de démonstration à remplacer avant une utilisation réelle.
- Les réservations sont enregistrées comme un nombre de places, sans identité de client ni numéros de sièges.
- `programme.txt` est lu et écrit dans le dossier courant : le programme doit être lancé depuis la racine du projet.
- La recherche par titre renvoie le premier événement trouvé, même si le même titre existe à plusieurs dates.
- `minimum`, `maximum` et `average` ne sont pas définies sur un vecteur vide : `main.cpp` vérifie qu'il y a des données avant de les appeler.