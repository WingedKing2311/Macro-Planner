# MacroTrack-CPP

A console-based nutrition tracker built in **C++** with **SQLite** for persistent storage. The application allows users to manage food items, calculate daily nutritional intake, rank foods by nutritional efficiency, and generate protein-focused recommendations under dietary constraints.

---

## Features

- Add food items
- View stored foods
- Edit existing foods
- Delete foods
- Calculate daily intake
- Protein optimization using a Greedy Algorithm
- Food rankings
  - Protein per Calorie
  - Fiber per Calorie
- SQLite database for persistent storage

---

## Technologies Used

- C++
- SQLite3
- STL
  - Vector
  - Sorting
  - Lambda Functions

---

## Algorithms Used

### Greedy Algorithm

Used for recommending foods that maximize protein intake while respecting:

- Protein requirement
- Calorie budget
- User-defined maximum quantity for each food

Foods are ranked using:

Protein per Calorie

and selected greedily until the constraints are met.

---

### Sorting

Foods can be ranked based on:

- Protein per Calorie
- Fiber per Calorie

using `std::sort()` with custom comparators.

---

## Database Schema

Table: `Items`

| Column | Description |
|---------|-------------|
| id | Primary Key |
| name | Food Name |
| protein | Protein (per 100g) |
| calories | Calories (per 100g) |
| fiber | Fiber (per 100g) |

---

## Screenshots

*(Add screenshots here after running the application.)*

---

## Future Improvements

- Daily meal history
- Search foods by name
- Protein goal tracking
- Cost-based optimization
- Meal templates
- CSV import/export

---

## Build

Compile using g++:

```bash
g++ main.cpp sqlite3.c -o MacroTracker
```

Run:

```bash
./MacroTracker
```

On Windows:

```bash
MacroTracker.exe
```

---

## Author

Marcus Abraham T
Information Technology
Madras Institute of Technology
