
#ifndef SDDS_PERISHABLE_H
#define SDDS_PERISHABLE_H

#include "Date.h"
#include "Item.h"

namespace sdds
{
    class Perishable : public Item
    {
        char* m_handlingInstructions;
        Date m_expiryDate;

    public:

        Perishable();
        Perishable(const Perishable& RO);
        Perishable& operator=(const Perishable& RO);
        ~Perishable();

        const Date& expiry() const;

        virtual int readSku(std::istream& istr) override;
        virtual std::ofstream& save(std::ofstream& ofstr) const override;
        virtual std::ifstream& load(std::ifstream& ifstr) override;
        virtual std::ostream& display(std::ostream& ostr) const override;
        virtual std::istream& read(std::istream& istr) override;

    };

}

#endif // SDDS_PERISHABLE_H

