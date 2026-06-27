#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "sqlite3.h"

using namespace std;

sqlite3 *db;

struct Food
{
    int id;
    string name;
    float protein;
    float calories;
    float fiber;
};

struct FoodOpt
{
    int id;
    string name;
    double protein;
    double calories;
    double availableQty;
};

struct RankFood
{
    int id;
    string name;
    double ratio;
};

void createTable();
void addEntry();
void viewEntries();
void deleteEntry();
void calculateDay();
void editEntry();
void proteinOptimization();
void foodRankings();

int main()
{
    int rc = sqlite3_open("macro_tracker.db", &db);

    if (rc)
    {
        cerr << "Cannot open database: "
             << sqlite3_errmsg(db) << endl;
        return 1;
    }

    cout << "Database Connected!\n";
    cout << fixed << setprecision(2);

    createTable();

    int choice;

    do
    {
        cout << "\n========== Macro Tracker ==========\n";
        cout << "1. Add Entry\n";
        cout << "2. View Entries\n";
        cout << "3. Delete Entry\n";
        cout << "4. Calculate Day\n";
        cout << "5. Edit Entry\n";
        cout << "6. Protein Optimization\n";
        cout << "7. Food Rankings\n";
        cout << "8. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addEntry();
            break;

        case 2:
            viewEntries();
            break;

        case 3:
            deleteEntry();
            break;

        case 4:
            calculateDay();
            break;

        case 5:
            editEntry();
            break;

        case 6:
            proteinOptimization();
            break;

        case 7:
            foodRankings();
            break;

        case 8:
            cout << "\nDatabase saved successfully.\n";
            cout << "Thank you for using Macro Tracker!\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (choice != 8);

    sqlite3_close(db);

    return 0;
}

void createTable()
{
    string sql =
        "CREATE TABLE IF NOT EXISTS Items("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "protein REAL,"
        "calories REAL,"
        "fiber REAL);";

    char *errMsg = nullptr;

    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        cerr << "Error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void addEntry()
{
    Food e;

    cin.ignore();

    cout << "Food Name: ";
    getline(cin, e.name);

    cout << "Protein: ";
    cin >> e.protein;

    cout << "Calories: ";
    cin >> e.calories;

    cout << "Fiber: ";
    cin >> e.fiber;

    string sql =
        "INSERT INTO Items(name,protein,calories,fiber)"
        " VALUES(?,?,?,?);";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, e.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, e.protein);
    sqlite3_bind_double(stmt, 3, e.calories);
    sqlite3_bind_double(stmt, 4, e.fiber);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "Entry Added!\n";
    else
        cout << "Failed to Add Entry.\n";

    sqlite3_finalize(stmt);
}

void viewEntries()
{
    bool found = false;

    string sql =
        "SELECT id,name,protein,calories,fiber "
        "FROM Items;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    cout << "\n===== Entries =====\n";

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;
        cout << "ID: "
             << sqlite3_column_int(stmt, 0)
             << "\nName: "
             << sqlite3_column_text(stmt, 1)
             << "\nProtein : "
             << sqlite3_column_double(stmt, 2)
             << " g/100g"
             << "\nCalories: "
             << sqlite3_column_double(stmt, 3)
             << " kcal/100g"
             << "\nFiber   : "
             << sqlite3_column_double(stmt, 4)
             << " g/100g"
             << "\n-------------------\n";
    }

    if (!found)
        cout << "No food items found.\n";

    sqlite3_finalize(stmt);
}

void deleteEntry()
{
    int id;

    cout << "Enter ID to delete: ";
    cin >> id;

    string sql = "DELETE FROM Items WHERE id = ?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db))
            cout << "Entry Deleted!\n";
        else
            cout << "ID not found!\n";
    }
    else
        cout << "Failed to Delete Entry.\n";

    sqlite3_finalize(stmt);
}

void calculateDay()
{
    double totalProtein = 0;
    double totalCalories = 0;
    double totalFiber = 0;

    while (true)
    {
        int id;
        double qty;

        cout << "Enter Food ID (-1 to finish): ";
        cin >> id;

        if (id == -1)
            break;

        cout << "Enter Quantity (g): ";
        cin >> qty;

        string sql =
            "SELECT protein, calories, fiber "
            "FROM Items "
            "WHERE id = ?;";

        sqlite3_stmt *stmt;

        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            double protein = sqlite3_column_double(stmt, 0);
            double calories = sqlite3_column_double(stmt, 1);
            double fiber = sqlite3_column_double(stmt, 2);

            totalProtein += protein * qty / 100.0;
            totalCalories += calories * qty / 100.0;
            totalFiber += fiber * qty / 100.0;
        }
        else
        {
            cout << "Food ID not found!\n";
        }

        sqlite3_finalize(stmt);
    }

    cout << "\n===== Daily Totals =====\n";
    cout << "Protein : " << totalProtein << " g\n";
    cout << "Calories: " << totalCalories << " kcal\n";
    cout << "Fiber   : " << totalFiber << " g\n";
}

void editEntry()
{
    int id;

    viewEntries();

    cout << "\nEnter ID to edit: ";
    cin >> id;

    cin.ignore();

    string name;
    double protein, calories, fiber;

    cout << "New Name: ";
    getline(cin, name);

    cout << "New Protein (per 100g): ";
    cin >> protein;

    cout << "New Calories (per 100g): ";
    cin >> calories;

    cout << "New Fiber (per 100g): ";
    cin >> fiber;

    string sql =
        "UPDATE Items "
        "SET name=?, protein=?, calories=?, fiber=? "
        "WHERE id=?;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, protein);
    sqlite3_bind_double(stmt, 3, calories);
    sqlite3_bind_double(stmt, 4, fiber);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
            cout << "Entry Updated!\n";
        else
            cout << "ID not found!\n";
    }
    else
    {
        cout << "Update Failed!\n";
    }

    sqlite3_finalize(stmt);
}

void proteinOptimization()
{
    double ProteinRequirement, calorieBudget;

    cout << "Enter protein requirement (g): ";
    cin >> ProteinRequirement;

    cout << "Enter calorie budget (kcal): ";
    cin >> calorieBudget;

    vector<FoodOpt> foods;

    viewEntries();

    while (true)
    {
        int id;

        cout << "\nFood ID (-1 to finish): ";
        cin >> id;

        if (id == -1)
            break;

        double qty;

        cout << "Maximum Quantity You Are Willing To Eat (g): ";
        cin >> qty;

        string sql =
            "SELECT name, protein, calories "
            "FROM Items "
            "WHERE id=?;";

        sqlite3_stmt *stmt;

        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            FoodOpt f;

            f.id = id;
            f.name = (const char *)sqlite3_column_text(stmt, 0);
            f.protein = sqlite3_column_double(stmt, 1);
            f.calories = sqlite3_column_double(stmt, 2);
            f.availableQty = qty;

            foods.push_back(f);
        }
        else
        {
            cout << "Invalid ID!\n";
        }

        sqlite3_finalize(stmt);
    }

    if (foods.empty())
    {
        cout << "No foods selected.\n";
        return;
    }

    sort(foods.begin(), foods.end(),
         [](const FoodOpt &a, const FoodOpt &b)
         {
             return (a.protein / a.calories) > (b.protein / b.calories);
         });

    double totalProtein = 0;
    double totalCalories = 0;

    cout << "\n===== Recommended Intake =====\n";

    for (auto &f : foods)
    {
        double proteinNeeded =
            ProteinRequirement - totalProtein;

        double qtyByProtein =
            proteinNeeded * 100.0 / f.protein;

        double qtyByCalories =
            calorieBudget * 100.0 / f.calories;

        double take =
            min({f.availableQty,
                 qtyByProtein,
                 qtyByCalories});

        if (take <= 0)
            continue;

        double usedCalories =
            f.calories * take / 100.0;

        double gainedProtein =
            f.protein * take / 100.0;

        calorieBudget -= usedCalories;

        totalCalories += usedCalories;
        totalProtein += gainedProtein;

        cout << f.name
             << " : "
             << take << " g"
             << " | Protein: " << gainedProtein << " g"
             << " | Calories: " << usedCalories << " kcal\n";

        if (calorieBudget <= 0.01 || totalProtein >= ProteinRequirement)
            break;
    }

    cout << "\nProtein : "
         << totalProtein
         << " g";

    cout << "\nCalories: "
         << totalCalories
         << " kcal";

    cout << "\nRemaining Budget: "
         << calorieBudget
         << " kcal\n";

    cout << "\nRemaining Protein Requirement: "
         << max(0.0, ProteinRequirement - totalProtein)
         << " g\n";

    if (totalProtein < ProteinRequirement)
    {
        cout << "\nProtein goal could not be reached "
             << "with the selected foods.\n";
    }
    else
    {
        cout << "\nProtein goal achieved!\n";
    }
}

void foodRankings()
{
    int choice;

    cout << "\n===== Food Rankings =====\n";
    cout << "1. Protein per Calorie\n";
    cout << "2. Fiber per Calorie\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice != 1 && choice != 2)
    {
        cout << "Invalid choice!\n";
        return;
    }

    string sql =
        "SELECT id,name,protein,calories,fiber "
        "FROM Items "
        "WHERE calories > 0;";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    vector<RankFood> foods;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        RankFood f;

        f.id = sqlite3_column_int(stmt, 0);
        f.name = (const char *)sqlite3_column_text(stmt, 1);

        double protein = sqlite3_column_double(stmt, 2);
        double calories = sqlite3_column_double(stmt, 3);
        double fiber = sqlite3_column_double(stmt, 4);

        if (choice == 1)
            f.ratio = protein / calories;
        else
            f.ratio = fiber / calories;

        foods.push_back(f);
    }

    sqlite3_finalize(stmt);

    if (foods.empty())
    {
        cout << "No foods available.\n";
        return;
    }

    sort(foods.begin(), foods.end(),
         [](const RankFood &a, const RankFood &b)
         {
             return a.ratio > b.ratio;
         });

    cout << "\n";

    if (choice == 1)
        cout << "===== Protein per Calorie =====\n";
    else
        cout << "===== Fiber per Calorie =====\n";

    for (size_t i = 0; i < foods.size(); i++)
    {
        cout << i + 1
             << ". "
             << foods[i].name
             << " ("
             << foods[i].ratio * 100
             << " per 100 kcal)"
             << endl;
    }
}