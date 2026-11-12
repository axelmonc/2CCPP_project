# Diagramme de classes

```mermaid
classDiagram
    class Room {
        -int m_id
        -string m_name
        -int m_capacity
        +id() int
        +name() string
        +capacity() int
    }

    class Event {
        <<abstract>>
        -string m_title
        -string m_date
        -string m_startTime
        -int m_duration
        -int m_roomId
        -int m_capacity
        -int m_bookedSeats
        -double m_price
        +book(int) bool
        +display(ostream) void
        +save(ostream) void*
        +type() string*
    }

    class Concert {
        -string m_lineUp
        -int m_soundCheckMinutes
    }
    class Play {
        -int m_intervalMinutes
    }
    class Screening {
        -string m_rating
    }
    class Workshop {
        -string m_instructor
    }

    class Venue {
        -vector~Room~ m_rooms
        -vector~unique_ptr~Event~~ m_events
        +addRoom(Room) void
        +scheduleEvent(unique_ptr~Event~) void
        +bookSeats(string, string, int) bool
        +totalTakings() double
    }

    class DataManager {
        <<fonctions libres>>
        +saveVenue(Venue, string) void
        +loadVenue(string) unique_ptr~Venue~
        +makeEvent(string, vector~string~) unique_ptr~Event~
    }

    class Analytics {
        <<template>>
        +minimum(vector~T~) T
        +maximum(vector~T~) T
        +average(vector~T~) double
    }

    Event <|-- Concert
    Event <|-- Play
    Event <|-- Screening
    Event <|-- Workshop
    Venue o-- Room
    Venue *-- Event
    DataManager ..> Venue
    Analytics ..> Venue
```

# Cas d'utilisation

- Visiteur : consulter le programme, rechercher un événement et réserver des places.
- Directeur : consulter les recettes, les statistiques et les salles les plus fréquentées.
- Administrateur : s'authentifier, ajouter une salle et programmer un événement.
- Application : sauvegarder les données et recharger le programme au démarrage.
