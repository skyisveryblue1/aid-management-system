#pragma once

#include <vector>

using namespace std;

#include "Perishable.h"

namespace sdds
{
	class AidMan
	{
	public:
		void run();

	private:
		void shipItems();
		void sortItems();
		void updateQuantity();
		void removeItem();
		void listItems(std::vector<iProduct*>& items, bool onlyShowList = false);
		void newOpenDatabase();
		void addItem();

		void save();

	protected:
		std::string m_filename;
		std::vector<iProduct*> m_items;
	};

}