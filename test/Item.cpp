

#define _CRT_SECURE_NO_WARNINGS

#include <iomanip>
#include <cstring>
#include "Utils.h"
#include "Item.h"


namespace sdds
{

    bool Item::linear() const
    {
        return m_linear;
    }

 

    Item::Item() : m_price(0.0), m_qty(0), m_qtyNeeded(0), m_description(nullptr), m_linear(false), m_sku(0)
    {
        m_state.clear();
    }

    Item& Item::operator=(const Item& RO)
    {
        if (this != &RO)
        {
            m_price = RO.m_price;
            m_qty = RO.m_qty;
            m_qtyNeeded = RO.m_qtyNeeded;
            m_linear = RO.m_linear;
            m_sku = RO.m_sku;
            m_state = RO.m_state;
            ut.alocpy(m_description, RO.m_description);

        }

        return *this;

    }

    Item::Item(const Item& RO) : m_description(nullptr)
    {
        operator=(RO);
    }

    Item::~Item()
    {
        delete[] m_description;
    }


    int Item::operator-=(int a_qty)
    {
        if (m_qty >= a_qty)
        {
            m_qty -= a_qty;
        }
        else
        {
            m_state = "Not enough quantity";
        }
        return m_qty;
    }

    int Item::operator+=(int a_qty)
    {
        m_qty += a_qty;
        return m_qty;
    }

    void Item::linear(bool isLinear)
    {
        m_linear = isLinear;
    }

    int Item::qtyNeeded() const
    {
        return m_qtyNeeded;
    }

    int Item::qty() const
    {
        return m_qty;
    }

    Item::operator double() const
    {
        return m_price;
    }

    Item::operator bool() const
    {
        return m_state && m_sku;
    }

    void Item::clear()
    {
        m_state.clear();
    }

    bool Item::operator==(int a_sku) const
    {
        return m_sku == a_sku;
    }

    bool Item::operator==(const char* a_description) const
    {
        return m_description && a_description && std::strstr(m_description, a_description) != nullptr;
    }

    std::ofstream& Item::save(std::ofstream& ofstr) const
    {
        if (m_state)
        {
            ofstr << m_sku << '\t' << m_description << '\t' << m_qty << '\t'

                << m_qtyNeeded << '\t' << std::fixed << std::setprecision(2) << m_price;
        }
        return ofstr;
    }

    std::ifstream& Item::load(std::ifstream& ifstr)
    {
        char buffer[MAX_DESC_LEN];
        int sku;
        int qty;
        int qtyNeeded;
        double price;
        ifstr >> sku;
        ifstr.ignore();
        ifstr.getline(buffer, MAX_DESC_LEN, '\t');
        ifstr >> qty;
        ifstr.ignore();
        ifstr >> qtyNeeded;
        ifstr.ignore();
        ifstr >> price;
        ifstr.ignore();
        
        if (ifstr)
        {
            m_sku = sku;
            m_qty = qty;
            m_qtyNeeded = qtyNeeded;
            m_price = price;
            delete[] m_description;
            m_description = new char[strlen(buffer) + 1];
            strcpy(m_description, buffer);
        }
        else
        {
            m_state = "Input file stream read failed!";
        }
        return ifstr;
    }

 
    std::ostream& Item::display(std::ostream& ostr) const
    {

        if (!m_state)
        {
            ostr << m_state;
        }
        else
        {
            if (m_linear)
            {
                ostr.fill(' ');
                ostr << std::setw(5) << m_sku << " | ";
                if (m_description)
                {
                    std::string truncatedDesc(m_description);
                    if (truncatedDesc.length() > 35)
                    {
                        truncatedDesc = truncatedDesc.substr(0, 35);
                    }
                    ostr << std::setw(35) << std::left << truncatedDesc << " | ";
                }
                else
                {
                    ostr << std::setw(35) << std::left << " " << " | ";
                }
                ostr << std::setw(4) << std::right << m_qty << " | "

                    << std::setw(4) << std::right << m_qtyNeeded << " | "

                    << std::fixed << std::setprecision(2) << std::setw(7) << std::right << m_price << " |";
            }
            else
            {
                ostr << "AMA Item:" << std::endl

                    << m_sku << ": " << (m_description ? m_description : "") << std::endl

                    << "Quantity Needed: " << m_qtyNeeded << std::endl

                    << "Quantity Available: " << m_qty << std::endl

                    << "Unit Price: $" << std::fixed << std::setprecision(2) << m_price << std::endl

                    << "Needed Purchase Fund: $" << std::fixed << std::setprecision(2) << (m_qtyNeeded - m_qty) * m_price << std::endl;
            }
        }
        return ostr;
    }


    std::istream& Item::read(std::istream& istr)
    {
        char buffer[MAX_DESC_LEN];
        std::cout << "AMA Item:" << std::endl;
        std::cout << "SKU: " << m_sku << std::endl;
        std::cout << "Description: ";
        istr.ignore(10000, '\n');
        istr.getline(buffer, MAX_DESC_LEN);
        ut.alocpy(m_description, buffer);
        m_qtyNeeded = ut.getint(1, 9999, "Quantity Needed: ", "Value out of range [1<=val<=9999]: ");
        m_qty = ut.getint(0, m_qtyNeeded, "Quantity On Hand: ", "Value out of range [0<=val<=qtyNeeded]: ");
        m_price = ut.getdouble(0.00, 9999.00, "Unit Price: $", "Value out of range [0.00<=val<=9999.00]: ");

        if (istr.fail())
        {
            m_state = "Console entry failed!";
            istr.clear();
            istr.ignore(10000, '\n');
        }
        return istr;
    }

 
    int Item::readSku(std::istream& istr)
    {
        m_sku = ut.getint(40000, 99999, "SKU: ", "Value out of range [40000<=val<=99999]: ");

        return m_sku;

    }

}
