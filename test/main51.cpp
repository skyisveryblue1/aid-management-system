// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>

#include "AidMan.h"
#include "Utils.h"

using namespace std;
using namespace sdds;

void copyfile(const char* to, const char* from);
void displayFile(const char* file);

int main()
{
    copyfile("data.dat", "data56.dat");
    ut.testMode();
    AidMan().run();
    displayFile("data.dat");
    cout << endl << "------------------------------------------------" << endl;
    displayFile("shippingOrder.txt");
    return 0;
}

void displayFile(const char* file) {
    ifstream in(file);
    char ch;
    cout << "File: " << file << endl;
    while (in.get(ch)) cout << ch;
}

void copyfile(const char* to, const char* from) {
    std::ifstream in(from);
    std::ofstream out(to);
    char ch;
    while (in.get(ch)) out.put(ch);
}
