# MacroTrack-CPP

A console-based nutrition tracker built in **C++** with **SQLite** for persistent storage. The application allows users to manage food items, calculate daily nutritional intake, rank foods by nutritional efficiency, and generate protein-focused recommendations under dietary constraints (For the Gym Bros).

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

<img width="434" height="274" alt="Screenshot 2026-06-28 134414" src="https://github.com/user-attachments/assets/a2827408-429a-48d5-8b37-732e6ce88082" />
<img width="418" height="211" alt="Screenshot 2026-06-28 134429" src="https://github.com/user-attachments/assets/32065c27-d55b-478a-917e-07923000bb27" />
<img width="277" height="61" alt="Screenshot 2026-06-28 134532" src="https://github.com/user-attachments/assets/f0146b01-7d3e-47dd-b3cd-8fbdaed369a1" />
<img width="542" height="412" alt="image" src="https://github.com/user-attachments/assets/444cc862-c41d-4075-8776-a109f37ed2ec" />

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
