

#ifndef SDDS_STATUS_H_
#define SDDS_STATUS_H_

 #include <iostream>
 
namespace sdds
{

    class Status
    {

        char* m_description;
        int m_code;

    public:

        Status(const char* a_description = nullptr, int a_code = 0);
        Status& operator=(const Status& a_source);
        Status(const Status& a_source);
        ~Status();

        Status& operator=(const char* a_description);
        Status& operator=(const int a_code);
        
        operator bool() const;
        operator const char* () const;
        operator int() const;

        Status& clear();

        friend std::ostream& operator<<(std::ostream& a_load, const Status& a_status);

    };

    std::ostream& operator<<(std::ostream& a_load, const Status& a_status);

    }

#endif  // SDDS_STATUS_H 

