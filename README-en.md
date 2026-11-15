# Cultural Venue Management Application

## Building

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

> If the `build` folder already exists with a different configuration, delete it before running again.

Run the executable **from the project root**: `programme.txt` is read and saved in the current directory.

## Project structure

```text
.
├── CMakeLists.txt
├── programme.txt     data loaded and saved by the program
├── include/          .hpp files
├── src/              .cpp files (including main.cpp)
└── tests/            Google Test unit tests
```

## Administration

The administrator code for this demo version is `CULT`.
It is defined in `main.cpp`.

## Event types

- Concert: artists and sound-check duration.
- Play: interval duration.
- Screening: rating.
- Workshop: instructor.

## `programme.txt` file format

One line per object, `;` separator.

```text
ROOM;id;name;capacity
CONCERT;title;date;time;duration;room;capacity;price;bookings;artists;soundcheck
PLAY;title;date;time;duration;room;capacity;price;bookings;interval
SCREENING;title;date;time;duration;room;capacity;price;bookings;rating
WORKSHOP;title;date;time;duration;room;capacity;price;bookings;instructor
```

Text fields must not contain the `;` character.
The date uses the `YYYY-MM-DD` format and the time uses the `HH:MM` format.

## Features

- Programme display and search by title.
- Programme display for a given date.
- Scheduling with conflict checking within the same room.
- Booking with capacity checking.
- Takings calculation and generic statistics (prices, durations, bookings, busiest rooms).
- Administrative addition of rooms and events.
- Saving and reloading the programme.
- Read errors report the file and the line number.

## Unit tests (Google Test)

Disabled by default: the normal build does not depend on Google Test.
Google Test is downloaded automatically by CMake (internet connection required the first time).

The tests are in the `tests/` folder:

- `test_event.cpp`: event validation (date, time, duration, capacity...) and seat booking
- `test_venue.cpp`: rooms, scheduling, time conflicts, bookings, statistics and displays
- `test_analytics.cpp`: minimum, maximum and average (generic functions)
- `test_persistence.cpp`: saving and loading the file, error handling (file and line)

### Windows (MinGW / MSYS2)

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -G "MinGW Makefiles" -DBUILD_TESTS=ON
cmake --build build --target unit_tests
cd build
ctest --output-on-failure
cd ..
```

### Linux / macOS

```bash
rm -rf build
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build --target unit_tests
cd build && ctest --output-on-failure && cd ..
```

Expected result: `100% tests passed`.

`Remove-Item` / `rm -rf build` avoids conflicts if `build` already exists with a different configuration
(for example a different generator). On Windows, do not forget `-G "MinGW Makefiles"`,
otherwise CMake looks for `nmake` and fails.

To add a test file, list it in `add_executable(unit_tests ...)` in `CMakeLists.txt`.

## Documentation
- `UML-en.md`: class diagram and use cases
- `DECISIONS-en.md`: design decisions and outcome of adding a new event type