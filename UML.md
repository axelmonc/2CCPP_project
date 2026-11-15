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
        +title() string
        +date() string
        +startTime() string
        +duration() int
        +roomId() int
        +capacity() int
        +bookedSeats() int
        +price() double
        +endMinutes() int
        +availableSeats() int
        +book(int) bool
        +display(ostream) void
        +save(ostream) void*
        +type() string*
    }

    class Concert {
        -string m_lineUp
        -int m_soundCheckMinutes
        +type() string
        +display(ostream) void
        +save(ostream) void
    }
    class Play {
        -int m_intervalMinutes
        +type() string
        +display(ostream) void
        +save(ostream) void
    }
    class Screening {
        -string m_rating
        +type() string
        +display(ostream) void
        +save(ostream) void
    }
    class Workshop {
        -string m_instructor
        +type() string
        +display(ostream) void
        +save(ostream) void
    }

    class Venue {
        -vector~Room~ m_rooms
        -vector~unique_ptr~Event~~ m_events
        +addRoom(Room) void
        +findRoom(int) Room*
        +scheduleEvent(unique_ptr~Event~) void
        +findEvent(string) Event*
        +bookSeats(string, string, int) bool
        +displayProgramme() void
        +displayEventsByDate(string) void
        +displayRooms() void
        +totalTakings() double
        +displayBusiestRooms() void
        +prices() vector~double~
        +durations() vector~int~
        +attendances() vector~int~
        +rooms() vector~Room~
        +events() vector~unique_ptr~Event~~
    }

    class DataManager {
        <<fonctions libres>>
        +saveVenue(Venue, string) void
        +loadVenue(string) unique_ptr~Venue~
        -makeEvent(string, vector~string~) unique_ptr~Event~
        -parseInt(string) int
        -parseDouble(string) double
        -splitLine(string) vector~string~
    }

    class Analytics {
        <<template>>
        +minimum(vector~T~) T
        +maximum(vector~T~) T
        +average(vector~T~) double
    }

    class Main {
        <<main.cpp>>
        -ADMIN_CODE string
        -readText(string) string
        -readInt(string) int
        -readDouble(string) double
        -addEvent(Venue) void
        -showStatistics(Venue) void
    }

    Event <|-- Concert
    Event <|-- Play
    Event <|-- Screening
    Event <|-- Workshop
    Venue *-- Room
    Venue *-- Event
    DataManager ..> Venue
    DataManager ..> Event
    Main ..> Venue
    Main ..> DataManager
    Main ..> Analytics
```

# Cas d'utilisation

- Visiteur : consulter le programme (complet ou à une date), rechercher un événement par titre, consulter les salles et réserver des places.
- Directeur : consulter les recettes, les statistiques (prix, durées, réservations) et les salles les plus fréquentées.
- Administrateur : s'authentifier avec le code, ajouter une salle et programmer un événement.
- Application : charger `programme.txt` au démarrage, sauvegarder à la demande et à la sortie (sauf si le fichier chargé était invalide).