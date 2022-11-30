#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

const string FILE_NAME = R"(C:\Git\pp-projekt\menu.txt)";
const string CONTINUE_CHAR = "X";
const string CONTINUE_CHAR_LOWER = string(1, (char) tolower(CONTINUE_CHAR.at(0)));
const string EXIT_CHAR = "Q";
const string EXIT_CHAR_LOWER = string(1, (char) tolower(EXIT_CHAR.at(0)));

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
};

vector<Dish> readMenu() {
    vector<Dish> menu;

    fstream menuFile;
    menuFile.open(FILE_NAME, ios::in);

    if (!menuFile.good()) {
        throw runtime_error("Nie można wczytać menu!");
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

    return menu;
}

string doubleToString(const double& d) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << d;
    return stream.str();
}

void printMenu(const vector<Dish>& menu, const Order& order) {
    cout << "Aktualna cena: " << doubleToString(order.value()) << endl;
    for (size_t i = 0; i < menu.size(); i++) {
        Dish item = menu[i];
        cout << "[" << i + 1 << "] " << item.name << " (" << item.ingredients << "), " << doubleToString(item.value) << endl;
    }
    cout << "[" << CONTINUE_CHAR << "] Kontynuuj" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdź bez zamówienia" << endl;
}

int main() {
    vector<Dish> menu = readMenu();
    Order order;

    string wybor;
    while (true) {
        printMenu(menu, order);
        cin >> wybor;

        if (wybor == CONTINUE_CHAR) {
            if (menu.empty()) {
                cout << "Nic nie wybrales!" << endl;
                continue;
            }

        } else if (wybor == EXIT_CHAR) {
            cout << "Zapraszamy ponownie!" << endl;
            return 0;
        }
    }

    return 0;
}
