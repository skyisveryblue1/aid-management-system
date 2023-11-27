

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include "Date.h"
#include "Utils.h"


namespace sdds
{

    int Date::unique() const
    {

        return m_year * 372 + m_month * 31 + m_day;
    }


    bool Date::validate()
    {

        bool valid = false;

        if (m_year < 2023 || m_year > m_maxYear)
        {
            m_state = Status("ERR#1: Invalid year in date", 1);
        }
        else if (m_month < 1 || m_month > 12)
        {
            m_state = Status("ERR#2: Invalid month in date", 2);
        }

        else if (m_day < 1 || m_day > ut.monthDays(m_month, m_year))
        {
            m_state = Status("ERR#3: Invalid day in date", 3);
        }
        else
        {
            valid = true;
        }
        return valid;
    }

    Date::Date()
    {
        ut.getSystemDate(&m_year, &m_month, &m_day);
        m_format = true;
    }

    Date::Date(int a_year, int a_month, int a_day)
    {
        m_day = a_day;
        m_month = a_month;
        m_year = a_year;
        m_format = true;
        
        validate();
    }
    

    Date& Date::operator=(const Date& RO)
    {
        if (this == &RO)
        {
            return *this;
        }
        m_day = RO.m_day;
        m_month = RO.m_month;
        m_year = RO.m_year;

        return *this;

    }


    bool Date::operator>=(const Date& RO)const
    {
        return this->unique() >= RO.unique();
    }

    bool Date::operator<=(const Date& RO)const
    {
        return this->unique() <= RO.unique();
    }

    bool Date::operator>(const Date& RO)const
    {
        return this->unique() > RO.unique();
    }

    bool Date::operator<(const Date& RO)const
    {
        return this->unique() < RO.unique();
    }

    bool Date::operator!=(const Date& RO)const
    {
        return this->unique() != RO.unique();
    }

    bool Date::operator==(const Date& RO)const
    {
        return this->unique() == RO.unique();
    }


    const Status& Date::state() const
    {
        return m_state;
    }

    Date& Date::formatted(bool a_format)
    {
        m_format = a_format;
        return *this;
    }

    Date::operator bool() const
    {
        return m_state;
    }


    std::ostream& Date::write(std::ostream& ostr) const
    {

    if (m_format)
    {
    ostr << m_year << '/'
    << std::setw(2) << std::setfill('0') << m_month << '/'
    << std::setw(2) << std::setfill('0') << m_day;
    }
    else
    {
    ostr << std::setw(2) << std::setfill('0') << (m_year % 100)
    << std::setw(2) << std::setfill('0') << m_month
    << std::setw(2) << std::setfill('0') << m_day;
    }
    return ostr;
    }



    std::istream& Date::read(std::istream& istr)
    {
        int value;
        istr >> value;

        if (!istr)
        {
            m_state = Status("Invalid date value", 0);
            return istr;
        }

        if (value > 9999)
        {
            m_day = value % 100;
            m_month = int((value % 10000) / 100);
            m_year = 2000 + int(value / 10000);
        }
        else
        {
            ut.getSystemDate(&m_year, nullptr, nullptr);
            m_day = value % 100;
            m_month = value / 100;
        }

        if (!validate())
        {
            istr.setstate(std::ios::failbit);
        }

        return istr;

    }


    std::istream& operator>>(std::istream& is, Date& date)
    {
        return date.read(is);
    }

    std::ostream& operator<<(std::ostream& ostr, const Date& a_date)
    {
        return a_date.write(ostr);
    }


} 

