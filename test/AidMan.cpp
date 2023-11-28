#define _CRT_SECURE_NO_WARNINGS

#include "AidMan.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <algorithm>

#include "Menu.h"

namespace sdds
{
    AidMan::AidMan() {
        m_productCount = 0;
        m_filename = nullptr;
    }

    AidMan::AidMan(const char* filename)  {
        m_productCount = 0;
        if (filename != nullptr) {
            m_filename = new char[strlen(filename) + 1];
            strcpy(m_filename, filename);
        }
    }

    AidMan::~AidMan() {
        deallocate();
    }

 	void AidMan::run()
	{
        static bool isFirstExecution = true;
        int choice = -1;
		do
		{
            if (!isFirstExecution) {
                cout << endl;
            }
            cout << "Aid Management System" << endl;
            Date currentDate;
            cout << "Date: " << currentDate << endl;

            cout << "Data file: " << ((m_filename != nullptr) ? m_filename : "No file") << endl;
            cout << "---------------------------------" << endl;
            Menu menu("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database");
            
            choice = menu.run(1);
            if (choice != 0) {
                cout << endl;
            }
            
            if (choice != 0 && m_filename == nullptr && choice != 7) {
                choice = 7;
            }

            switch (choice) {
            case 1: {
                cout << "****List Items****" << endl;
                int count = list(nullptr);

                cout << "Enter row number to display details or <ENTER> to continue:" << endl;
                cout << "> ";
                cin.ignore();
                char input[0x100] = { 0 };
                cin.getline(input, 0x100);

                if (*input != 0) {
                    int row = atoi(input);
                    if (row > 0 && row <= count) {
                        // Display details for the selected item
                        m_products[row - 1]->linear(false);
                        m_products[row - 1]->display(cout);
                    }
                    else {
                        cout << "Invalid row number." << endl;
                    }
                }
                break;
            }
            case 2:
                add();
                break;
            case 3:
                removeItem();
                break;
            case 4:
                updateQuantity();
                break;
            case 5:
                sortItems();
                break;
            case 6:
                shipItems();
                break;
            case 7:
                load();
                break;
            case 0:
                cout << "Exiting Program!" << endl;
                save();
                return;
            default:
                cout << endl;
                cout << "Invalid choice. Please try again." << endl;
            }

            isFirstExecution = false;
        } while (choice != 0);
       
    }

    void AidMan::shipItems() {
        cout << "****Ship Items****" << endl;

        ofstream shipOrderedFile("shippingOrder.txt");
        Date currentDate;
        shipOrderedFile << "Shipping Order, Date: " << currentDate << endl;
        shipOrderedFile << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry\n";
        shipOrderedFile << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
        
        int shippedCount = 0;
        for (int i = 0; i < m_productCount; ++i) {
            iProduct* product = m_products[i];
            if (product->qty() == product->qtyNeeded()) {
                product->linear(true);
                shipOrderedFile.fill(' ');
                shipOrderedFile << std::setw(4) << (++shippedCount) << " | " <<  *product << endl;
                remove(i);
                i--;
            }
        }
        shipOrderedFile << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

        cout << "Shipping Order for " << shippedCount << " times saved!" << endl;
    }

    void AidMan::sortItems()
    {
        cout << "****Sort****" << endl;

        for (int i = 0; i < m_productCount - 1; ++i) {
            for (int j = i + 1; j < m_productCount; ++j) {
                if ((m_products[i]->qtyNeeded() - m_products[i]->qty()) < (m_products[j]->qtyNeeded() - m_products[j]->qty())) {
                    iProduct* t = m_products[i];
                    m_products[i] = m_products[j];
                    m_products[j] = t;
                }
            }
        }

        cout << "Sort completed!" << endl;
    }

    void AidMan::updateQuantity()
    {
        cout << "****Update Quantity****" << endl;
        cout << "Item description: ";
        cin.ignore();  // Ignore any previous newline character in the input buffer
        char description[1000] = { 0 };
        cin.getline(description, 1000);

        int matchedCount = list(description);
        if (matchedCount == 0) {
            cout << "No matches found!" << endl;
            return;
        }

        cout << "Enter SKU: ";
        int sku;
        cin >> sku;
        int idx = search(sku);
        if (idx == -1) {
            cout << "SKU not found!" << endl;
            return;
        }

        Menu menu("Add\tReduce");
        int choice = menu.run(0);
        if (choice == 0) {
            cout << "Aborted!" << endl;
            return;
        }

        // Add
        if (choice == 1) {
            if (m_products[idx]->qtyNeeded() <= m_products[idx]->qty()) {
                cout << "Quantity Needed already fulfilled!" << endl;
                return;
            }
            else {
                int qty = ut.getint(1, m_products[idx]->qtyNeeded(), "Quantity to add: ", nullptr);
                *m_products[idx] += qty;
                cout << qty << " items added!" << endl;
            }
        }
        // Reduce
        if (choice == 2) {
            if (m_products[idx]->qty() == 0) {
                cout << "Quantity on hand is zero!" << endl;
                return;
            }
            else {
                int qty = ut.getint(1, m_products[idx]->qty(), "Quantity to reduce: ", nullptr);
                *m_products[idx] -= qty;
                cout << qty << " items removed!" << endl;
            }
        }
    }

    void AidMan::remove(int idx) {
        if (idx >= m_productCount) {
            return;
        }

        delete m_products[idx];
        m_productCount--;
        for (int i = idx; i < m_productCount; ++i) {
            m_products[i] = m_products[i + 1];
        }        
    }

    void AidMan::removeItem()
    {
        cout << "****Remove Item****" << endl;
        cout << "Item description: ";
        cin.ignore();  
        char description[1000] = { 0 };
        cin.getline(description, 1000);

        int matchedCount = list(description);
        if (matchedCount == 0) {
            cout << "No matching items found." << endl;
            return;
        }

        cout << "Enter SKU: ";
        int sku;
        cin >> sku;

        int idx = search(sku);
        if (idx == -1) {
            cout << "SKU not found!" << endl;
            return;
        }
        cout << "Following item will be removed: " << endl;
        m_products[idx]->linear(false);
        cout << *m_products[idx] << endl;

        cout << "Are you sure?" << endl;
        Menu menu("Yes!");
        int choice = menu.run(0);
        if (choice == 1) {
            remove(idx);
            cout << "Item removed!" << endl;
        }
        else {
            cout << "Aborted!" << endl;
        }
    }

    void AidMan::add() {
        // if number of iProduct Items is not less than sdds_max_num_items, 
        // it will print: "Database full!"
        if (m_productCount >= sdds_max_num_items) {
            cout << "Database full!" << endl;
            return;
        }

        cout << "****Add Item****" << endl;
        Menu menu("Perishable\tNon-Perishable");
        int choice = menu.run(2);

        Item* newProduct = nullptr;
        int sku = -1;

        // Based on the user's entry a Perishable or Non-Perishable Item is allocated in an iProduct pointer.
        // If the user chooses to exit, the message "Aborted\n" is printed.
        switch (choice) {
        case 1: {
            newProduct = new Perishable;
            break;
        }
        case 2: {
            newProduct = new Item;
            break;
        }
        case 0:
            cout << "Aborted!" << endl;
            return;
        };

        sku = newProduct->readSku(cin);
        if (search(sku) != -1) {
            cout << "Sku: " << sku << " is already in the system, try updating quantity instead." << endl;
            delete newProduct;
            return;
        }

        // If the read iProduct is in a good state, 
        // it is added to the next available element of the iProduct Pointers array and 
        // number of iProduct Items is added by one, 
        // otherwise, the allocated Item is displayed and then deleted.
        cin >> *newProduct;
        if (*newProduct) {
            m_products[m_productCount++] = newProduct;
        }
        else {
            cout << *newProduct << endl;
            delete newProduct;
            newProduct = nullptr;
        }
    }
    
    int AidMan::search(int sku) const {
        // Loops through all the iProduct Pointers elements and 
        // if the SKU is a match it will return the index, otherwise it will return -1
        for (int i = 0; i < m_productCount; ++i) {
            if ((*m_products[i]) == sku) {
                return i;
            }
        }
        return -1;
    }

    int AidMan::list(const char* sub_desc/* = nullptr*/) {
        
        cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry\n";
        cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
            
        int printedCount = 0;
        if (sub_desc == nullptr) {
            // if sub_desc is null, print all the items in a linear format.
            for (int i = 0; i < m_productCount; ++i) {
                m_products[i]->linear(true);
                cout.fill(' ');
                cout << std::setw(4) << i + 1 << " | ";
                m_products[i]->display(cout);
                cout << endl;
            }
            printedCount = m_productCount;
        }
        else {
            // if sub_desc is not null, print only the items containing the sub_desc in their description.
            for (int i = 0; i < m_productCount; ++i) {
                if ((*m_products[i]) == sub_desc) {
                    m_products[i]->linear(true);
                    cout.fill(' ');
                    cout << std::setw(4) << i + 1 << " | ";
                    m_products[i]->display(cout);
                    cout << endl;
                    printedCount++;
                }
            }
        }

        cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

        if (printedCount == 0) {
            // if no items are listed, print "The list is emtpy!" and go to new line
            cout << "The list is emtpy!" << endl;
        }
        
        // returns the number of iProducts printed 
        return printedCount;
    }

    bool AidMan::load() {
        save();
        deallocate();

        cout << "****New/Open Aid Database****" << endl;
        cout << "Enter file name: ";

        char filename[0x100];
        cin >> filename;

        m_filename = new char[strlen(filename) + 1];
        strcpy(m_filename, filename);

        ifstream file(m_filename);
        if (!file) {
            cout << "Failed to open " << m_filename << " for reading!" << endl;
            cout << "Would you like to create a new data file?" << endl;
            Menu newData("Yes!");
            int choice = newData.run(true);
            if (choice == 1) {
                ofstream newFile(m_filename);
                newFile.close();
            }
        } else {
            
            while (!file.eof())
            {
                Item* newItem = nullptr;
                int firstChar = file.peek();
                if (firstChar >= '1' && firstChar < '4') {
                    newItem = new Perishable();
                } else if (firstChar >= '4') {
                    newItem = new Item();
                } else {
                    file.setstate(std::ios::failbit);
                }

                if (newItem != nullptr) {
                    newItem->load(file);
                    if (*newItem) {
                        m_products[m_productCount++] = newItem;
                        if (m_productCount >= sdds_max_num_items) {
                            cout << "Database full!" << endl;
                            break;
                        }
                    }
                    else {
                        delete newItem;
                    }
                }
            }

            cout << m_productCount << " records loaded!" << endl;
            file.close();
        }
        return true;
    }

    void AidMan::save() {
        if (m_filename == nullptr)
            return;

        ofstream ofs(m_filename);
        if (!ofs)
            return;

        for (int i = 0; i < m_productCount; ++i)
        {
            m_products[i]->save(ofs);
            ofs << endl;
        }
    }

    void AidMan::deallocate() {
        for (int i = 0; i < m_productCount; i++) {
            delete m_products[i];
            m_products[i] = nullptr;
        }
        m_productCount = 0;
        delete[] m_filename;
        m_filename = nullptr;
    }
}

