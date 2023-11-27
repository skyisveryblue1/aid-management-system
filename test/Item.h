

#ifndef SDDS_ITEM_H
#define SDDS_ITEM_H

#include "iProduct.h"
#include "Status.h"

#define MAX_DESC_LEN 10000

namespace sdds
{
   class Item : public iProduct
    {

        double m_price;
        int m_qty;
        int m_qtyNeeded;
        char* m_description;
        bool m_linear;

   protected:
        Status m_state;
        int m_sku;
        bool linear() const;

   public:

        Item();
        Item& operator=(const Item& RO);
        Item(const Item& other);
       ~Item();

   
        virtual int operator-=(int a_qty) override;
        virtual int operator+=(int a_qty) override;
        virtual void linear(bool isLinear) override;

        virtual int qtyNeeded() const override;
        virtual int qty() const override;
        virtual operator double() const override;
        virtual operator bool() const override;

        void clear();

        virtual bool operator==(int sku) const override;
        virtual bool operator==(const char* description) const override;
        virtual std::ofstream& save(std::ofstream& ofstr) const override;
        virtual std::ifstream& load(std::ifstream& ifstr) override;
        virtual std::ostream& display(std::ostream& ostr) const override;
        virtual std::istream& read(std::istream& istr) override;

        virtual int readSku(std::istream& istr) override;

   };

}

#endif // SDDS_ITEM_H 

