#pragma once
#ifndef classes_cpp
#define classes_cpp

#include <string>
#include <vector>

using namespace std;

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
        return this->dish.value * (double)this->amount;
    }
};

class Order {
public:
    vector<Selection> selections = vector<Selection>{};
    int status;
    string address;
    string name;
    int stolik;

    double value() const {
        double value = 0;
        for (const Selection& selection : this->selections) {
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

    void remove(int index) {
        vector<Selection> newSelections;

        for (int i = 0; i < this->selections.size(); i++) {
            if (i == index) {
                continue;
            }

            auto item = this->selections[i];
            newSelections.push_back(item);
        }

        this->selections = newSelections;
    }

    bool empty() const {
        return this->selections.empty();
    }

    int getMaxMinutes() const {
        int minutes = 0;
        for (const auto& selection : this->selections) {
            if (selection.dish.minutes > minutes) {
                minutes = selection.dish.minutes;
            }
        }
        return minutes;
    }
};

#endif