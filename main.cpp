#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "classes.cpp"

using namespace std;

static const string FILE_NAME = "menu.txt";

string toLowercase(string s) {
    return string(1, (char) tolower(s.at(0)));
}

static const string CONTINUE_CHAR = "X";
static const string CONTINUE_CHAR_LOWER = toLowercase(CONTINUE_CHAR);
static const string EXIT_CHAR = "Q";
static const string EXIT_CHAR_LOWER = toLowercase(EXIT_CHAR);
static const string DELETE_CHAR = "D";
static const string DELETE_CHAR_LOWER = toLowercase(DELETE_CHAR);
static const string BACK_CHAR = "P";
static const string BACK_CHAR_LOWER = toLowercase(BACK_CHAR);
static const string YES_CHAR = "T";
static const string YES_CHAR_LOWER = toLowercase(YES_CHAR);
static const string NO_CHAR = "N";
static const string NO_CHAR_LOWER = toLowercase(NO_CHAR);

string doubleToString(const double &d) {
    stringstream stream;
    stream << fixed << setprecision(2) << d;
    return stream.str();
}

int safeStringToInt(const string &str) {
    try {
        return stoi(str);
    } catch (invalid_argument) {
        return -1;
    }
}

int stringToIndex(const string &choice) {
    auto number = safeStringToInt(choice);
    return number == -1 ? number : number - 1;
}

vector<Dish> readMenu() {
    vector<Dish> menu;

    fstream menuFile;
    menuFile.open(FILE_NAME, ios::in);

    if (!menuFile.good()) {
        throw runtime_error("Nie mozna wczytac menu!");
    }

    string line;
    while (!menuFile.eof()) {
        getline(menuFile, line);
        if (line.find(';') != string::npos) {
            Dish dish;

            stringstream ss(line);
            string item;

            getline(ss, dish.name, ';');

            getline(ss, dish.ingredients, ';');

            string value;
            getline(ss, value, ';');
            dish.value = stod(value);

            string minutes;
            getline(ss, minutes, ';');
            dish.minutes = stoi(minutes);

            menu.push_back(dish);
            continue;
        }
        break;
    }

    menuFile.close();

    return menu;
}

void printTotalCount(const Order &order) {
    cout << "Aktualna cena: " << doubleToString(order.value()) << endl;
}

string getDishString(const Dish& dish) {
    stringstream ss;
    ss << dish.name << " (" << dish.ingredients << "), " << doubleToString(dish.value);
    return ss.str();
}

string getDishStringWithAmount(const Dish& dish, int amount) {
    stringstream ss;
    ss << dish.name << ", " << amount << " x " << doubleToString(dish.value) << ", lacznie: " << doubleToString(dish.value * (double) amount);
    return ss.str();
}

string getSelectionString(const Selection& selection) {
    return getDishStringWithAmount(selection.dish, selection.amount);
}

void printMenu(const vector<Dish> &menu, const Order &order) {
    printTotalCount(order);
    cout << "Menu:" << endl;
    for (auto i = 0; i < menu.size(); i++) {
        cout << "[" << i + 1 << "] " << getDishString(menu[i]) << endl;
    }
    cout << "[" << DELETE_CHAR << "] Usuwanie dodanych pozycji" << endl;
    cout << "[" << CONTINUE_CHAR << "] Kontynuuj" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdz bez zamowienia" << endl;
}

void printSelectionForDelete(const Order &order) {
    printTotalCount(order);
    cout << "Menu:" << endl;
    for (auto i = 0; i < order.selections.size(); i++) {
        cout << "[" << i + 1 << "] " << getSelectionString(order.selections[i]) << endl;
    }
    cout << "[" << BACK_CHAR << "] Powrot" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdz bez zamowienia" << endl;
}

void processSummary(const Order &order) {
    cout << "PODSUMOWANIE ZAMOWIENIA WKROTCE!" << endl;
    // TODO Podsumowanie zamowienia
}

void processDeleteSelection(Order &order) {
    while (true) {
        printSelectionForDelete(order);
        string choice;
        cin >> choice;

        if (choice == EXIT_CHAR || choice == EXIT_CHAR_LOWER) {
            cout << "Zapraszamy ponownie!" << endl;
            return;
        }

        if (choice == BACK_CHAR || choice == BACK_CHAR_LOWER) {
            break;
        }

        auto index = stringToIndex(choice);
        if (index >= 0 && index < order.selections.size()) {
            cout << "Czy na pewno chcesz usunac:" << endl;
            auto selection = order.selections[index];
            cout << getSelectionString(selection) << endl;

            cout << "Potwierdz wpisujac T (tak) lub N (nie): " << endl;
            string zatwierdzenie;
            cin >> zatwierdzenie;

            if (zatwierdzenie == YES_CHAR || zatwierdzenie == YES_CHAR_LOWER) {
                order.remove(index);
                continue;
            }

            if (zatwierdzenie == NO_CHAR || zatwierdzenie == NO_CHAR_LOWER) {
                cout << "Nie usuwam!" << endl;
                continue;
            }
        }

        cout << "Podaj prawidlowa opcje lub numer pozycji!" << endl;
    }
}

void processMenuSelection() {
    vector<Dish> menu = readMenu();
    Order order;

    while (true) {
        printMenu(menu, order);
        string choice;
        cin >> choice;

        if (choice == EXIT_CHAR || choice == EXIT_CHAR_LOWER) {
            cout << "Zapraszamy ponownie!" << endl;
            return;
        }

        if (choice == CONTINUE_CHAR || choice == CONTINUE_CHAR_LOWER) {
            if (order.empty()) {
                cout << "Nic nie wybrales!" << endl;
                continue;
            }

            processSummary(order);
            return;
        }

        if (choice == DELETE_CHAR || choice == DELETE_CHAR_LOWER) {
            if (order.empty()) {
                cout << "Nic nie wybrales!" << endl;
                continue;
            }

            processDeleteSelection(order);
            continue;
        }

        auto index = stringToIndex(choice);
        if (index >= 0 && index < menu.size()) {
            cout << "Ile chcesz porcji?" << endl;
            string amountString;
            cin >> amountString;

            int amount = safeStringToInt(amountString);
            if (amount < 1) {
                cout << "Nieprawidlowa wartosc!" << endl;
                continue;
            }

            cout << "Czy na pewno chcesz dodac:" << endl;
            auto dish = menu[index];
            cout << getDishStringWithAmount(dish, amount) << endl;

            cout << "Potwierdz wpisujac T (tak) lub N (nie): " << endl;
            string zatwierdzenie;
            cin >> zatwierdzenie;

            if (zatwierdzenie == YES_CHAR || zatwierdzenie == YES_CHAR_LOWER) {
                order.add(dish, amount);
                cout << "Dodano!" << endl;
                continue;
            }

            if (zatwierdzenie == NO_CHAR || zatwierdzenie == NO_CHAR_LOWER) {
                cout << "Nie dodaje!" << endl;
                continue;
            }

            cout << "Nie podales prawidlowej opcji!" << endl;
            continue;
        }

        cout << "Podaj prawidlowa opcje lub numer dania!" << endl;
    }
}

int main() {
    cout << "PODANIE WSTEPNYCH DANYCH WKROTCE!" << endl;
    // TODO Podanie wstepnych danych
    processMenuSelection();
    return 0;
}