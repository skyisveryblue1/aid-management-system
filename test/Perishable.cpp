

#include <cstring>
#include "Utils.h"
#include "Perishable.h"

namespace sdds
{
    Perishable::Perishable() : Item(), m_handlingInstructions(nullptr) {}

 
    Perishable::Perishable(const Perishable& RO) : Item(RO), m_handlingInstructions(nullptr)
    {
        *this = RO;
    }

 

    Perishable& Perishable::operator=(const Perishable& RO)
    {
        if (this != &RO)
        {
            Item::operator=(RO);
            ut.alocpy(m_handlingInstructions, RO.m_handlingInstructions);
            m_expiryDate = RO.m_expiryDate;

        }

        return *this;
    }

 

    Perishable::~Perishable()
    {
        delete[] m_handlingInstructions;
    }

 

    const Date& Perishable::expiry() const
    {
        return m_expiryDate;
    }


    int Perishable::readSku(std::istream& istr)
    {
        int sku = ut.getint(10000, 39999, "SKU: ", "Value out of range [10000<=val<=39999]: ");

        if (sku >= 10000 && sku <= 39999)
        {
            m_sku = sku;
        }

        return m_sku;

    }

 

    std::ofstream& Perishable::save(std::ofstream& ofstr) const
    {

        if (this)
        {
            Item::save(ofstr);
            ofstr << '\t';
            if (m_handlingInstructions && m_handlingInstructions[0] != '\0') {

                    ofstr << m_handlingInstructions;

                }

            ofstr << '\t';

            Date tempDate = m_expiryDate;

            tempDate.formatted(false);

            ofstr << tempDate;

        }

        return ofstr;

    }

 

    std::ifstream& Perishable::load(std::ifstream& ifstr)
    {

        Item::load(ifstr);

        char buffer[1000];
        ifstr.getline(buffer, 1000, '\t');
        ut.alocpy(m_handlingInstructions, buffer);
        ifstr >> m_expiryDate;
        ifstr.ignore(1000, '\n');
        if (!ifstr)
        {
            m_state = "Input file stream read (perishable) failed!";
        }
        return ifstr;

    }

 

    std::ostream& Perishable::display(std::ostream& ostr) const
    {

        if (!*this)
        {
            ostr << m_state;
        }

        else
        {
            if (linear())
            {
                Item::display(ostr);

                if (m_handlingInstructions && m_handlingInstructions[0] != '\0')
                {
                    ostr << '*';
                }
                else
                {
                    ostr << ' ';
                }
                ostr << m_expiryDate;

            }
            else
            {
                ostr << "Perishable ";
                Item::display(ostr);
                ostr << "Expiry date: ";
                ostr << m_expiryDate << std::endl;
                if (m_handlingInstructions && m_handlingInstructions[0] != '\0')
                {

                    ostr << "Handling Instructions: " << m_handlingInstructions << std::endl;

                }

            }

        }

        return ostr;

    }

 

    std::istream& Perishable::read(std::istream& istr) {

        if (Item::read(istr))
        {

            std::cout << "Expiry date (YYMMDD): ";
            istr >> m_expiryDate;
            istr.ignore(10000, '\n');
            std::cout << "Handling Instructions, ENTER to skip: ";

            if (istr.peek() != '\n')
            {
                char buffer[1000];
                istr.getline(buffer, 1000);
                ut.alocpy(m_handlingInstructions, buffer);
            }
            else
            {
                delete[] m_handlingInstructions;
                m_handlingInstructions = nullptr;
            }
            if (istr.fail())
            {
                m_state = "Perishable console date entry failed!";
            }

        }

        return istr;

    }

}
