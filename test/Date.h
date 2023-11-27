

#ifndef SDDS_DATE_H_
#define SDDS_DATE_H_

#include "Utils.h"
#include "Status.h"

 #include <iostream>
 
namespace sdds
{

    class Date
    {

        Status m_state;
        int m_day;
        int m_month;
        int m_year;
        bool m_format;
        const int m_maxYear = 2030;

        int unique()const;
        bool validate();


    public:

        Date();
        Date(int year,int month,int day);
        Date& operator=(const Date& RO);

        bool operator>=(const Date& RO)const;
        bool operator<=(const Date& RO)const;
        bool operator>(const Date& RO)const;
        bool operator<(const Date& RO)const;
        bool operator!=(const Date& RO)const;
        bool operator==(const Date& RO) const;
        
        const Status& state() const;
        Date& formatted(bool a_format);
        operator bool() const;

        std::ostream& write(std::ostream& ostr)const;
        std::istream& read(std::istream& istr);

    };

        std::istream& operator>>(std::istream& is, Date& date);
        std::ostream& operator<<(std::ostream& os, const Date& date);

}
#endif  // SDDS_DATE_H 

