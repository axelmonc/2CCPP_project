# Décisions de conception

## Responsabilités

- `Room` représente une salle et sa capacité.
- `Event` contient les informations communes aux événements et définit les opérations polymorphes.
- `Concert`, `Play`, `Screening` et `Workshop` portent les informations propres à chaque type.
- `Venue` gère les salles, les événements, les réservations et les questions du directeur.
- `DataManager` est responsable de la lecture et de l'écriture du fichier.
- `Analytics.hpp` contient les fonctions génériques de minimum, maximum et moyenne.

## Choix techniques

1. L'héritage public représente la relation « un concert est un événement ».
2. `Event` est abstraite : on ne crée que des événements concrets.
3. `Venue` possède les événements avec `std::unique_ptr<Event>`. Le destructeur virtuel de `Event` permet une destruction correcte.
4. Les salles sont stockées par valeur dans un `std::vector<Room>`.
5. Les événements sont stockés dans un seul vecteur polymorphe, ce qui évite de modifier `Venue` pour chaque nouveau type.
6. Les statistiques utilisent des templates pour fonctionner avec `double` et `int`.
7. Le chargement construit un nouvel objet `Venue`. En cas d'erreur, cet objet temporaire est détruit et aucun programme partiellement chargé n'est utilisé.
8. Le fichier indique son type au premier champ. Le chargement choisit la classe concrète à partir de ce tag.

## Extensibilité : ajout d'un type Exhibition

Pour ajouter un type `Exhibition`, il faudrait :
- créer `Exhibition.hpp` et `Exhibition.cpp` ;
- ajouter la création de ce type dans la partie factory de `DataManager.cpp` ;
- ajouter le choix et la saisie de ses informations dans `main.cpp`.

Il n'est pas nécessaire de modifier `Event`, `Venue`, `Room` ou les autres types d'événements. Dans cette version, `Workshop` sert déjà d'exemple d'un type ajouté au même mécanisme.

## Limites connues de cette version pédagogique

- Les champs texte du fichier ne peuvent pas contenir `;`.
- La validation de date contrôle le format, le mois et le nombre de jours (années bissextiles comprises), sans autre règle de calendrier.
- Les événements qui dépassent minuit sont refusés.
- Le code administrateur `CULT` est un code de démonstration à remplacer avant une utilisation réelle.
- Les réservations sont enregistrées comme un nombre de places, sans identité de client ni numéros de sièges.
