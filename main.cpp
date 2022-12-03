#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

static const string FILE_NAME = "menu.txt";

static const string CONTINUE_CHAR = "X";
static const string CONTINUE_CHAR_LOWER = string(1, (char) tolower(CONTINUE_CHAR.at(0)));
static const string EXIT_CHAR = "Q";
static const string EXIT_CHAR_LOWER = string(1, (char) tolower(EXIT_CHAR.at(0)));
static const string DELETE_CHAR = "D";
static const string DELETE_CHAR_LOWER = string(1, (char) tolower(DELETE_CHAR.at(0)));

class Dish {
public:
    string name;
    string ingredients;
    double value{};
    int minutes{};
};

class Selection {
public:
    Dish dish;
    int amount{};

    double value() const {
        return this->dish.value * this->amount;
    }
};

class Order {
public:
    vector<Selection> selections = vector<Selection>{};

    double value() const {
        double value = 0;
        for (const auto &selection: this->selections) {
            value += selection.value();
        }
        return value;
    }

    void add(Dish dish, int amount) {
        Selection selection;
        selection.amount = amount;
        selection.dish = dish;
        this->selections.push_back(selection);
    }

    int getMaxMinutes() const {
        int minutes = 0;
        for (const auto &selection: this->selections) {
            if (selection.dish.minutes > minutes) {
                minutes = selection.dish.minutes;
            }
        }
        return minutes;
    }
};

string doubleToString(const double &d) {
    stringstream stream;
    stream << fixed << setprecision(2) << d;
    return stream.str();
}

int indexOfMenu(const string &choice) {
    try {
        int number = std::stoi(choice);
        return number - 1;
    }
    catch (invalid_argument) {
        return -1;
    }
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
        } else {
            break;
        }
    }

    menuFile.close();

    return menu;
}

void printMenu(const vector<Dish> &menu, const Order &order) {
    cout << "Aktualna cena: " << doubleToString(order.value()) << endl;
    cout << "Menu:" << endl;
    for (size_t i = 0; i < menu.size(); i++) {
        Dish item = menu[i];
        cout << "[" << i + 1 << "] " << item.name << " (" << item.ingredients << "), " << doubleToString(item.value)
             << endl;
    }
    cout << "[" << DELETE_CHAR << "] Usuwanie dodanych pozycji" << endl;
    cout << "[" << CONTINUE_CHAR << "] Kontynuuj" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdz bez zamowienia" << endl;
}

int processMenuSelection() {
    vector<Dish> menu = readMenu();
    Order order;

    string choice;
    while (true) {
        printMenu(menu, order);
        cin >> choice;

        if (choice == CONTINUE_CHAR || choice == CONTINUE_CHAR_LOWER) {
            if (menu.empty()) {
                cout << "Nic nie wybrales!" << endl;
                continue;
            }
            // TODO Dalszy proces zamowienia
            break;
        } else if (choice == EXIT_CHAR || choice == EXIT_CHAR_LOWER) {
            cout << "Zapraszamy ponownie!" << endl;
            return 0;
        } else if (choice == DELETE_CHAR || choice == DELETE_CHAR_LOWER) {
            if (menu.empty()) {
                cout << "Nic nie wybrales!" << endl;
                continue;
            }
        } else {
            int index = indexOfMenu(choice);
            if (index >= 0 && index < menu.size()) {
                cout << "Ile chcesz porcji?" << endl;
                int amount;
                cin >> amount;
                order.add(menu[index], amount);
                cout << "Dodano!" << endl;
            } else {
                cout << "Podaj prawidlowy numer dania!" << endl;
                continue;
            }
        }
    }

    return 0;
}

int main() {
    return processMenuSelection();
}