#pragma once

using namespace std;

#include "Perishable.h"

namespace sdds
{
	class AidMan
	{
	public:
		AidMan();
		AidMan(const char* filename);
		~AidMan();

		void run();
	private:
		void shipItems();
		void sortItems();
		void updateQuantity();
		void removeItem();
		int list(const char* sub_desc = nullptr);
		void add();
		void remove(int idx);

		int search(int sku) const;

		bool load();
		void save();

		void deallocate();

	public:
		const char* version = "0.6";
		static const int sdds_max_num_items = 100;

	protected:
		iProduct* m_products[sdds_max_num_items];
		int m_productCount;
		char* m_filename;
	};

}