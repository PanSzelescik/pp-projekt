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
#include <ctime>
#include <conio.h>

#pragma warning(disable : 4996)

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
    cout << "\nAktualna cena: " << doubleToString(order.value()) << endl;
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
    ss << dish.name << ", " << amount << " x " << doubleToString(dish.value) << ", łącznie: "
       << doubleToString(dish.value * (double) amount);
    return ss.str();
}

string getSelectionString(const Selection &selection) {
    return getDishStringWithAmount(selection.dish, selection.amount);
}

void printMenu(const vector<Dish> &menu, const Order &order) {
    printTotalCount(order);
    cout << "Menu:" << endl;
    for (int i = 0; i < menu.size(); i++) {
        cout << "[" << i + 1 << "] " << getDishString(menu[i]) << endl;
    }
    cout << "[" << DELETE_CHAR << "] Usuwanie dodanych pozycji" << endl;
    cout << "[" << CONTINUE_CHAR << "] Kontynuuj" << endl;
    printExit();
}

void printSelectionForDelete(const Order &order) {
    printTotalCount(order);
    cout << "Twoje zamówienie:" << endl;
    for (int i = 0; i < order.selections.size(); i++) {
        cout << "[" << i + 1 << "] " << getSelectionString(order.selections[i]) << endl;
    }
    cout << "[" << BACK_CHAR << "] Powrót" << endl;
    cout << "[" << EXIT_CHAR << "] Wyjdź bez zamówienia" << endl;
}

void processSummary(const Order &order) {
    fstream receiptFile;
    receiptFile.open(FILE_NAME_RECEIPT, fstream::in | fstream::out | fstream::trunc);
    if (order.status == 1) {
        receiptFile << "Stolik: " << order.stolik << "\n";
    }
    receiptFile << "Czas oczekiwania: " << order.getMaxMinutes() << " minut\n";
    if (order.status == 2) {
        receiptFile << "Zamówienie zostanie dostarczone na godzinę: " << order.godzinaDostawy << "\n";
    }
    receiptFile << "Łączny koszt: " << doubleToString(order.value()) << "\n";
    receiptFile << "Zamówienie:\n\n";
    for (int i = 0; i < order.selections.size(); i++) {
        receiptFile << "[" << i + 1 << "] " << getSelectionString(order.selections[i]) << endl;
    }
    receiptFile.close();
    cout << "\nPrognozowany czas oczekiwania to: " << order.getMaxMinutes() << " minut" << endl;
    if (order.status == 2) {
        cout << "Zamówienie zostanie dostarczone na godzinę: " << order.godzinaDostawy << endl;
    }
    cout << "Paragon został zapisany" << endl;
    cout << "Wciśnij dowolny klawisz, aby zakończyć" << endl;
    getch();
}

void processDeleteSelection(Order &order) {
    while (true) {
        printSelectionForDelete(order);
        string choice;
        cin >> choice;

        if (choice == EXIT_CHAR || choice == EXIT_CHAR_LOWER) {
            cout << "\nZapraszamy ponownie!" << endl;
            return;
        }

        if (choice == BACK_CHAR || choice == BACK_CHAR_LOWER) {
            break;
        }

        int index = stringToIndex(choice);
        if (index >= 0 && index < order.selections.size()) {
            cout << "\nCzy na pewno chcesz usunąć:" << endl;
            Selection selection = order.selections[index];
            cout << getSelectionString(selection) << endl;

            cout << "Potwierdź wpisując T (tak) lub N (nie): " << endl;
            string zatwierdzenie;
            cin >> zatwierdzenie;

            if (zatwierdzenie == YES_CHAR || zatwierdzenie == YES_CHAR_LOWER) {
                cout << "\nUsuwam!" << endl;
                order.remove(index);
                continue;
            }

            if (zatwierdzenie == NO_CHAR || zatwierdzenie == NO_CHAR_LOWER) {
                cout << "\nNie usuwam!" << endl;
                continue;
            }
        }

        cout << "\nPodaj prawidłową opcję lub numer pozycji!" << endl;
    }
}

void processMenuSelection(Order &order) {
    vector<Dish> menu = readMenu();

    while (true) {
        printMenu(menu, order);
        string choice;
        cin >> choice;

        if (choice == EXIT_CHAR || choice == EXIT_CHAR_LOWER) {
            cout << "\nZapraszamy ponownie!" << endl;
            return;
        }

        if (choice == CONTINUE_CHAR || choice == CONTINUE_CHAR_LOWER) {
            if (order.empty()) {
                cout << "\nNic nie wybrałeś!" << endl;
                continue;
            }

            processSummary(order);
            return;
        }

        if (choice == DELETE_CHAR || choice == DELETE_CHAR_LOWER) {
            if (order.empty()) {
                cout << "\nNic nie wybrałeś!" << endl;
                continue;
            }

            processDeleteSelection(order);
            continue;
        }

        int index = stringToIndex(choice);
        if (index >= 0 && index < menu.size()) {
            cout << "\nIle chcesz porcji?" << endl;
            string amountString;
            cin >> amountString;

            int amount = safeStringToInt(amountString);
            if (amount < 1) {
                cout << "\nNieprawidłowa wartość!" << endl;
                continue;
            }

            cout << "\nCzy na pewno chcesz dodać:" << endl;
            Dish dish = menu[index];
            cout << getDishStringWithAmount(dish, amount) << endl;

            cout << "Potwierdź wpisując T (tak) lub N (nie): " << endl;
            string zatwierdzenie;
            cin >> zatwierdzenie;

            if (zatwierdzenie == YES_CHAR || zatwierdzenie == YES_CHAR_LOWER) {
                order.add(dish, amount);
                cout << "\nDodano!" << endl;
                continue;
            }

            if (zatwierdzenie == NO_CHAR || zatwierdzenie == NO_CHAR_LOWER) {
                cout << "\nNie dodaję!" << endl;
                continue;
            }

            cout << "\nNie podałeś prawidłowej opcji!" << endl;
            continue;
        }

        cout << "\nPodaj prawidłową opcję lub numer dania!" << endl;
    }
}

void addUTF8Support() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

int main() {
    addUTF8Support();

    string name, adres;
    int status, stolik, godzinaDostawy;

    time_t currentTime = time(nullptr);
    tm *localTime = localtime(&currentTime);

    cout << "Witamy w gospodzie \"Pod Głupim Osłem\"!" << endl;
    cout << "Adres: Bagienna 5, 21-370 Zasiedmiogorogród" << endl;
    cout << "Właściciel: Shrek Kerhs" << endl;
    cout << "Tel.: +48 732 054 143" << endl;
    cout << "Email: 1s8xhb@gmail.com" << endl;
    cout << "Godziny otwarcia: od poniedziałku do soboty, 10:00 - 22:00" << endl;

    if (localTime->tm_wday >= 1 && localTime->tm_wday <= 6) {
        if (localTime->tm_hour >= 10 && localTime->tm_hour < 22) {
            cout << "\nPodaj swoje imię: ";
            cin >> name;
            cout << "\nWitaj " << name << endl;

            cout << "\nCzy danie ma być na miejscu czy na dowóz?" << endl;
            cout << "[1] Na miejscu" << endl;
            cout << "[2] Na dowóz" << endl;
            cin >> status;

            Order order;
            order.name = name;
            order.status = status;

            while (true) {
                if (status == 1) {
                    cout << "\nWybierz numer stolika [1-15]" << endl;
                    cin >> stolik;
                    if (stolik >= 1 && stolik <= 15) {
                        cout << "\nWybrano stolik numer " << stolik << endl;
                        order.stolik = stolik;
                        break;
                    } else {
                        cout << "\nWprowadzono niepoprawny numer stolika!" << endl;
                        continue;
                    }
                }

                if (status == 2) {
                    cout << "\nPodaj adres na który mamy dostarczyć posiłek" << endl;
                    getline(cin >> ws, adres);
                    order.address = adres;
                    cout << "\nTwój adres to:" << endl;
                    cout << adres << endl;
                    while (true) {
                        cout << "\nWprowadź godzinę dostawy: ";
                        cin >> godzinaDostawy;
                        order.godzinaDostawy = godzinaDostawy;
                        if (godzinaDostawy >= 10 && godzinaDostawy <= 22) {
                            cout << "\nGodzina dostawy: " << godzinaDostawy << endl;
                            break;
                        } else {
                            cout << "\nNiestety, gosopoda jest otwarta tylko w godzinach 10:00 - 22:00." << endl;
                            continue;
                        }
                    }
                    break;
                }

                cout << "\nWybrałeś złą opcje!" << endl;
                cout << "\nCzy danie ma być na miejscu czy na dowóz?" << endl;
                cout << "[1] Na miejscu" << endl;
                cout << "[2] Na dowóz" << endl;
                cin >> status;
                order.status = status;
            }
            processMenuSelection(order);
        } else {
            cout << "\nGospoda jest zamknięta, przyjdź gdy gospoda będzie otwarta!" << endl;
        }
    } else {
        cout << "\nGospoda jest zamknięta, przyjdź gdy gospoda będzie otwarta!" << endl;
    }

    return 0;
}
