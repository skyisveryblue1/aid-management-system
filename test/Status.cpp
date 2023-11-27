

#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <iostream>
#include "Utils.h"
#include "Status.h"


using namespace std;

namespace sdds
{

   Status::Status(const char* a_description, int a_code) : m_code(0)
    {
       if (a_description)
       {
           m_description = new char[strlen(a_description) + 1];
           strcpy(m_description, a_description);
       }
       else
       {
           m_description = nullptr;
       }
   }



    Status& Status::operator=(const Status& a_source)
    {
        if (this != &a_source)
        {
            delete[] m_description;
    
            if (a_source.m_description != nullptr)
            {
                m_description = new char[strlen(a_source.m_description) + 1];
                strcpy(m_description, a_source.m_description);
            }
            else
            {
                m_description = nullptr;
            }

            m_code = a_source.m_code;

        }

        return *this;

    }


   Status::Status(const Status& a_source)
    {
       if (a_source.m_description != nullptr)
       {
           m_description = new char[strlen(a_source.m_description) + 1];
           strcpy(m_description, a_source.m_description);
       }
       else
       {
           m_description = nullptr;
       }

       m_code = a_source.m_code;

   }


   Status::~Status()
    {
       delete[] m_description;
       m_description = nullptr;
   }

   Status& Status::operator=(const char* a_description)
    {
       delete[] m_description;
       if (a_description)
       {
           m_description = new char[strlen(a_description) + 1];
           strcpy(m_description, a_description);
       }
       else
       {
           m_description = nullptr;
       }
       return *this;
   }


    Status& Status::operator=(const int a_code)
     {
        m_code = a_code;
        return *this;
    }

    Status::operator bool() const
    {
        return m_description == nullptr;
    }

    Status::operator const char* () const
    {
        return m_description;
    }

    Status::operator int() const
    {
       return m_code;
    }


    Status& Status::clear()
    {
       delete[] m_description;
       m_description = nullptr;
       m_code = 0;
       return *this;
    }


    std::ostream& operator<<(std::ostream& a_load, const Status& a_status)
    {
       if (!a_status)
       {
           if (a_status.m_code != 0)
           {
               a_load << "ERR#" << a_status.m_code << ": ";
           }
           a_load << a_status.m_description;
       }
       return a_load;
   }

}


