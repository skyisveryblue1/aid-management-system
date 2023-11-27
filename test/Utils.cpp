

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>
#include "Utils.h"

using namespace std;

namespace sdds
{

   Utils ut;

   void Utils::getSystemDate(int* a_year, int* a_month, int* a_day)
    {

       if (m_testMode)
       {
           if (a_day) *a_day = get_day;
           if (a_month) *a_month = get_month;
           if (a_year) *a_year = get_year;
       }

       else
       {
           time_t t = std::time(NULL);
           tm lt = *localtime(&t);
           if (a_day) *a_day = lt.tm_mday;
           if (a_month) *a_month = lt.tm_mon + 1;
           if (a_year) *a_year = lt.tm_year + 1900;
       }

   }

    int Utils::monthDays(int a_month, int a_year)const
    {

       int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };

       int month = (a_month >= 1 && a_month <= 12 ? a_month : 13) - 1;

       return days[month] + int((month == 1) * ((a_year % 4 == 0) && (a_year % 100 != 0)) || (a_year % 400 == 0));

   }

    void Utils::testMode(bool a_testmode)
    {
        m_testMode = a_testmode;
    }


   void Utils::alocpy(char*& destination, const char* source)
    {
       if (destination)
       {
           delete[] destination;
       }

       if (source)
       {
           destination = new char[strlen(source) + 1];
           strcpy(destination, source);
       }
       else
       {
           destination = nullptr;
       }

   }

   int Utils::getint(const char* prompt)
    {
       bool isValid;
       int value;
       do
       {
           isValid = true;
           if (prompt)
           {
               std::cout << prompt;
           }
           std::cin >> value;
           if (std::cin.fail())
           {
               std::cin.clear();
               std::cin.ignore(1000, '\n');
               std::cout << "Invalid Integer, retry: ";
               isValid = false;

           }

       } while (!isValid);

       return value;
   }


   int Utils::getint(int a_min, int a_max, const char* prompt, const char* errMes) {

       int value = 0;
       bool flag = true;

       do
       {

           if (prompt && flag)
           {
               std::cout << prompt;
           }

           else if (!flag)
           {
               if (std::cin.fail())
               {
                   std::cout << "Invalid Integer, retry: ";
                   std::cin.clear();
                   std::cin.ignore(1000, '\n');
               }

               else if (value < a_min || value > a_max)
               {
                   std::cout << "Value out of range [" << a_min << "<=val<=" << a_max << "]: ";
               }

           }

           std::cin >> value;
           flag = false;

       } while (std::cin.fail() || value < a_min || value > a_max);

       return value;
   }



   double Utils::getdouble(double a_min, double a_max, const char* prompt, const char* errMes)
    {

       double value = 0;
       bool flag = true;

       do
       {
           if (prompt && flag)
           {
               std::cout << prompt;
           }

           else if (!flag)
           {
               if (std::cin.fail())
               {
                   std::cout << "Invalid number, retry: ";
                   std::cin.clear();
                   std::cin.ignore(10000, '\n');
               }

               else if (value < a_min || value > a_max)
               {
                   std::cout << "Value out of range [" << std::fixed << std::setprecision(2) << a_min << "<=val<=" << a_max << "]: ";
               }

           }

           std::cin >> value;
           flag = false;

       } while (std::cin.fail() || value < a_min || value > a_max);

       return value;

   }

} 

