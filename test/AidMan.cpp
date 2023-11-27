#include "AidMan.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

#include "Menu.h"

namespace sdds
{
	void AidMan::run()
	{
        int choice;
     
		do
		{
            ifstream ifs;
            ifs.open(m_filename);

            std::time_t t = std::time(0);
            std::tm* now = std::localtime(&t);

			cout << endl << "Aid Management System" << endl;
			cout << "Date: " << std::put_time(now, "%Y/%m/%d") << endl;
			cout << "Data file: " << (ifs.is_open() ? m_filename : "No file") << endl;
            cout << "---------------------------------" << endl;
            Menu menu("List Items\tAdd Item\tRemove Item\tUpdate Quantity\tSort\tShip Items\tNew/Open Aid Database");
            
            choice = menu.run();
            cout << endl;
            
            if (choice != 0 && choice != 7 && m_items.size() == 0)
            {
                newOpenDatabase();
                continue;
            }

            switch (choice) {
            case 1:
                listItems(m_items);
                break;
            case 2:
                addItem();
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
                newOpenDatabase();
                break;
            case 0:
                cout << "Exiting program!" << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 0);
	}

    void AidMan::shipItems()
    {
        cout << "****Ship Items****" << endl;

        vector<iProduct*> shipedOrderItems;
        m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&](const iProduct* item) {
            bool erase = (item->qty() == item->qtyNeeded());
            if (erase)
            {
                shipedOrderItems.push_back((iProduct*)item);
            }
            return erase;
        }), m_items.end());

        ofstream shipOrderedFile("shippingOrder.txt");
        shipOrderedFile << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry\n";
        shipOrderedFile << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;
        for (size_t i = 0; i < shipedOrderItems.size(); ++i)
        {
            shipedOrderItems[i]->linear(true);
            shipOrderedFile.fill(' ');
            shipOrderedFile << std::setw(4) << i + 1 << " | ";
            shipedOrderItems[i]->display(shipOrderedFile);
            shipOrderedFile << endl;
        }

        cout << "Shipping Order for " << shipedOrderItems.size() << " times saved!" << endl;
        save();
    }

    void AidMan::sortItems()
    {
        cout << "****Sort****" << endl;

        std::sort(m_items.begin(), m_items.end(), [](const iProduct* a, const iProduct* b) {
            return (a->qtyNeeded() - a->qty()) > (b->qtyNeeded() - b->qty());
        });

        cout << "Sort completed!" << endl;
        save();
    }

    void AidMan::updateQuantity()
    {
        cout << "****Update Quantity****" << endl;
        cout << "Item description: ";
        cin.ignore();  // Ignore any previous newline character in the input buffer
        char description[1000] = { 0 };
        cin.getline(description, 1000);

        vector<iProduct*> matchingItems;
        for (const auto& item : m_items) {
            if (*item == description) {
                matchingItems.push_back(item);
            }
        }

        if (matchingItems.empty()) {
            cout << "No matching items found." << endl;
            return;
        }

        listItems(matchingItems, true);

        cout << "Enter SKU: ";
        int sku;
        cin >> sku;

        auto it = find_if(matchingItems.begin(), matchingItems.end(), [sku](const iProduct* item) {
            return *item == sku;
        });

        if (it != matchingItems.end())
        {
            cout << "1- Add" << endl;
            cout << "2- Reduce" << endl;
            cout << "0- Exit" << endl;
            cout << "> ";

            int confirmation;
            cin >> confirmation;
            if (confirmation == 0)
            {
                cout << "Aborted!" << endl;
            }

            it = find_if(m_items.begin(), m_items.end(), [sku](const iProduct* item) {
                return *item == sku;
            });

            if (confirmation == 1 && (*it)->qtyNeeded() <= (*it)->qty())
            {
                cout << "Quantity Needed already fulfilled!" << endl;
                return;
            }
            
            int quantity;
            cout << "Quantity to " << (confirmation == 1 ? "add: " : "reduce: ");
            cin >> quantity;

            if (confirmation == 1 && quantity <= (*it)->qtyNeeded() - (*it)->qty()) {
                *(*it) += quantity;
                cout << quantity << " items added!" << endl;
                save();
            }
            else if (confirmation == 2 && quantity <= (*it)->qty()) {
                *(*it) -= quantity;
                cout << quantity << " items removed!" << endl;
                save();
            }
            else {
                cout << "Value out of range [1<=val<=" << 
                    (confirmation == 1 ? (*it)->qtyNeeded() - (*it)->qty() : (*it)->qty()) << "]" << endl;
            }
        }
    }

    void AidMan::removeItem()
    {
        cout << "****Remove Item****" << endl;
        cout << "Item description: ";
        cin.ignore();  // Ignore any previous newline character in the input buffer
        char description[1000] = { 0 };
        cin.getline(description, 1000);

        vector<iProduct*> matchingItems;
        for (const auto& item : m_items) {
            if (*item == description) {
                matchingItems.push_back(item);
            }
        }

        if (matchingItems.empty()) {
            cout << "No matching items found." << endl;
            return;
        }

        listItems(matchingItems, true);

        cout << "Enter SKU: ";
        int sku;
        cin >> sku;

        auto it = find_if(matchingItems.begin(), matchingItems.end(), [sku](const iProduct* item) {
            return *item == sku;
        });

        if (it != matchingItems.end())
        {
            cout << "Following item will be removed:" << endl;
            (*it)->linear(false);
            (*it)->display(cout);

            cout << "Are you sure?" << endl;
            cout << "1- Yes!" << endl;
            cout << "0- Exit" << endl;
            cout << "> ";

            int confirmation;
            cin >> confirmation;
            if (confirmation == 1)
            {
                it = find_if(m_items.begin(), m_items.end(), [sku](const iProduct* item) {
                    return *item == sku;
                });
                m_items.erase(it);
                cout << "Item removed!" << endl;
                save();
            }
            else if (confirmation == 0)
            {
                
            }
        }

    }

    void AidMan::addItem()
    {
        cout << "****Add Item****" << endl;
        Menu menu("Perishable\tNon-Perishable");

        int choice = menu.run();
        cout << endl;

        Item* new_item = nullptr;
        int sku = -1;
       
        switch (choice) {
        case 1:
        {
            new_item = new Perishable;
            sku = new_item->readSku(cin);
            cin >> *new_item;
            if (!*new_item)
            {
                delete new_item;
                new_item = nullptr;
            }

            break;
        }
        case 2:
        {
            new_item = new Item;
            sku = new_item->readSku(cin);
            cin >> *new_item;
            if (!*new_item)
            {
                delete new_item;
                new_item = nullptr;
            }

            break;
        }
        case 0:
            cout << "Exiting program!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        };

        if (new_item)
        {
            for (iProduct* item : m_items)
            {
                if (*item == sku)
                {
                    cout << "Sku:" << sku << " is already in the system, try updating quantity instead." << endl;
                    delete new_item;
                    return;
                }
            }
            m_items.push_back(new_item);
            save();
            cout << endl;
        }
        
    }

    void AidMan::listItems(std::vector<iProduct*>& items, bool onlyShowList/* = false*/)
    {
        cout << "****List Items****" << endl;
        std::cout << " ROW |  SKU  | Description                         | Have | Need |  Price  | Expiry\n";
        cout << "-----+-------+-------------------------------------+------+------+---------+-----------" << endl;

        for (size_t i = 0; i < items.size(); ++i) 
        {
            items[i]->linear(true);
            cout.fill(' ');
            cout << std::setw(4) << i + 1 << " | ";
            items[i]->display(cout);
            cout << endl;
        }

        if (onlyShowList)
            return;

        cout << "Enter row number to display details or <ENTER> to continue:" << endl;
        cout << "> ";
        cin.ignore();
        char input[0x100] = { 0 };
        cin.getline(input, 0x100);

        if (*input != 0) {
            int row = atoi(input);
            if (row > 0 && row <= static_cast<int>(items.size())) {
                // Display details for the selected item
                items[row - 1]->linear(false);
                items[row - 1]->display(cout);
            }
            else {
                cout << "Invalid row number." << endl;
            }
        }
    }

    void AidMan::newOpenDatabase() {
        cout << "****New/Open Aid Database****" << endl;
        cout << "Enter file name: ";
        cin >> m_filename;

        ifstream file(m_filename);

        if (file.is_open()) {

            m_items.clear();  // Clear existing items before loading new ones

            int numRecords = 0;
            while (!file.eof())
            {
                Item* newItem = new Perishable;
                streampos currentPosition = file.tellg();
                ios::iostate currentStatus = file.rdstate();

                newItem->load(file);
                if (!(*newItem))
                {
                    file.clear();
                    file.seekg(currentPosition);
                    file.setstate(currentStatus);
                    delete newItem;
                    newItem = new Item;
                    newItem->load(file);                    
                }
                if (!file)
                    break;
                m_items.push_back(newItem);
                numRecords++;
            }

            cout << numRecords << " records loaded!" << endl;
            file.close();
        }
        else {
            cout << "Unable to open file: " << m_filename << endl;
        }
    }

    void AidMan::save()
    {
        ofstream ofs(m_filename);
        if (!ofs)
        {
            return;
        }

        for (iProduct* item : m_items)
        {
            item->save(ofs);
            ofs << endl;
        }
    }
}

