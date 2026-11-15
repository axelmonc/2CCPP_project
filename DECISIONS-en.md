# Design Decisions

## Responsibilities

- `Room` represents a room and its capacity.
- `Event` holds the information common to all events, validates its data in its constructor and defines the polymorphic operations.
- `Concert`, `Play`, `Screening` and `Workshop` hold the information specific to each type.
- `Venue` manages rooms, events, bookings and the director's queries. It also provides the data series (prices, durations, bookings) used by the statistics.
- `DataManager` is responsible for reading and writing the file.
- `Analytics.hpp` contains the generic minimum, maximum and average functions.
- `main.cpp` contains the menu, the safe input handling and the code-protected administration.

## Technical choices

1. Public inheritance represents the relationship "a concert is an event".
2. `Event` is abstract: only concrete events are created.
3. `Venue` owns the events through `std::unique_ptr<Event>`. The virtual destructor of `Event` allows correct destruction.
4. Rooms are stored by value in a `std::vector<Room>`: `Venue` owns them (composition).
5. Events are stored in a single polymorphic vector, which avoids modifying `Venue` for each new type.
6. The statistics use templates to work with `double` and `int`. `Analytics.hpp` does not depend on `Venue`: it works on `std::vector<T>` that `Venue` provides (`prices()`, `durations()`, `attendances()`).
7. Loading builds a new `Venue` object. In case of error, this temporary object is destroyed and no partially loaded programme is used.
8. The file states its type in the first field. Loading chooses the concrete class from this tag (`makeEvent` function, internal to `DataManager.cpp`: it lives in an anonymous namespace, like `parseInt`, `parseDouble` and `splitLine`).
9. Data is validated in the constructors (`Room`, `Event` and its child classes): an invalid object cannot exist. Rules that involve several objects are in `Venue::scheduleEvent`: the room exists, the event capacity is less than or equal to the room capacity, the title is unique per date, and there is no overlap within the same room.
10. The project is built as a static library `lieu_culturel_lib` (everything except `main.cpp`), linked to the `lieu_culturel` executable and to the tests. This avoids compiling the same files twice.

## Outcome of adding a new event type: Workshop

After the initial design (Concert, Play, Screening), we added a fourth type, `Workshop` (a workshop with an instructor). The result, measured in the git history:

| File | Change |
| --- | --- |
| `include/Workshop.hpp`, `src/Workshop.cpp` | created (44 lines in total) |
| `src/DataManager.cpp` | +8 lines: a `WORKSHOP` branch in `makeEvent` |
| `src/main.cpp` | a "4 Workshop" option in the menu and an input branch in `addEvent` |
| `CMakeLists.txt` | +1 line: `src/Workshop.cpp` in `add_library` |
| `Event`, `Venue`, `Room`, `Analytics.hpp` | **no changes** |

Concrete consequences:

- `Venue` only knows `Event`: scheduling, conflict detection, bookings, takings and statistics work unchanged for the new type.
- Saving works as soon as `Workshop::save` is written; only loading needs a branch, because it must choose the class from the tag.
- The point that still has to be modified by hand is `makeEvent` (and the `main.cpp` menu): adding a type is not completely automatic. A type registry (tag → creation function) would remove this step, but would be disproportionate for four types.

For a fifth type (for example `Exhibition`), the same recipe applies: create the two files, add them to `CMakeLists.txt`, add the branch in `makeEvent` and the choice in `main.cpp`.

## Known limitations of this educational version

- Text fields in the file cannot contain `;`.
- Date validation checks the format, the month and the number of days (leap years included), with no other calendar rule: a past date is accepted.
- Events that run past midnight are rejected.
- The administrator code `CULT` is a demo code to be replaced before any real use.
- Bookings are recorded as a number of seats, with no customer identity or seat numbers.
- `programme.txt` is read and written in the current directory: the program must be run from the project root.
- Search by title returns the first event found, even if the same title exists on several dates.
- `minimum`, `maximum` and `average` are not defined on an empty vector: `main.cpp` checks that there is data before calling them.