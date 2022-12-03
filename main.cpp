#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "classes.cpp"
#include "consts.cpp"
#include "utils.cpp"

using namespace std;

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

void printExit() {
    cout << "[" << EXIT_CHAR << "] Wyjdź bez zamówienia" << endl;
}

string getDishString(const Dish &dish) {
    stringstream ss;
    ss << dish.name << " (" << dish.ingredients << "), " << doubleToString(dish.value);
    return ss.str();
}

string getDishStringWithAmount(const Dish &dish, int amount) {
    stringstream ss;
    ss << dish.name << ", " << amount << " x " << doubleToString(dish.value) << ", łącznie: " << doubleToString(dish.value * (double) amount);
    return ss.str();
}

string getSelectionString(const Selection &selection) {
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
    printExit();
}

void printSelectionForDelete(const Order &order) {
    printTotalCount(order);
    cout << "Twoje zamówienie:" << endl;
    for (auto i = 0; i < order.selections.size(); i++) {
        cout << "[" << i + 1 << "] " << getSelectionString(order.selections[i]) << endl;
    }
    cout << "[" << BACK_CHAR << "] Powrót" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdź bez zamówienia" << endl;
}

void processSummary(const Order &order) {
    cout << "PODSUMOWANIE ZAMóWIENIA WKRóTCE!" << endl;
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
            cout << "Czy na pewno chcesz usunąć:" << endl;
            auto selection = order.selections[index];
            cout << getSelectionString(selection) << endl;

            cout << "Potwierdź wpisując T (tak) lub N (nie): " << endl;
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

        cout << "Podaj prawidłową opcję lub numer pozycji!" << endl;
    }
}

void processMenuSelection() {
    auto menu = readMenu();
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
            processSummary(order);
            return;
        }

        if (choice == DELETE_CHAR || choice == DELETE_CHAR_LOWER) {
            if (order.empty()) {
                cout << "Nic nie wybrałeś!" << endl;
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
                cout << "Nieprawidłową wartość!" << endl;
                continue;
            }

            cout << "Czy na pewno chcesz dodać:" << endl;
            auto dish = menu[index];
            cout << getDishStringWithAmount(dish, amount) << endl;

            cout << "Potwierdź wpisując T (tak) lub N (nie): " << endl;
            string zatwierdzenie;
            cin >> zatwierdzenie;

            if (zatwierdzenie == YES_CHAR || zatwierdzenie == YES_CHAR_LOWER) {
                order.add(dish, amount);
                cout << "Dodano!" << endl;
                continue;
            }

            if (zatwierdzenie == NO_CHAR || zatwierdzenie == NO_CHAR_LOWER) {
                cout << "Nie dodaję!" << endl;
                continue;
            }

            cout << "Nie podałeś prawidłowej opcji!" << endl;
            continue;
        }

        cout << "Podaj prawidłową opcję lub numer dania!" << endl;
    }
}

void addUTF8Support() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

int main() {
    addUTF8Support();

    cout << "PODANIE WSTEPNYCH DANYCH WKROTCE! ęąśźćż" << endl;
    // TODO Podanie wstepnych danych
    processMenuSelection();
    return 0;
}