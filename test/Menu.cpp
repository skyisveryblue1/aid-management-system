#define _CRT_SECURE_NO_WARNINGS

#include "Menu.h"
#include <iostream>
#include <cstring>

namespace sdds
{
    Menu::Menu(const char* text) : menuContent(nullptr)
    {
      if (text)
      {
          int tabCount = 0;
          for (const char* t = text; *t; ++t)
          {
              if (*t == '\t') ++tabCount;
          }
          
          if (tabCount < 15)
          {
              menuContent = new char[std::strlen(text) + 1];
              std::strcpy(menuContent, text);
          }
      }
  }


    Menu::~Menu()
    {
        delete[] menuContent;
    }


    unsigned int Menu::countOptions() const
    {
        unsigned int options = 0;
        if (menuContent)
        {
            for (const char* temp = menuContent; *temp; ++temp)
            {
                if (*temp == '\t') ++options;
            }
            ++options;
        }
        return options;
    }


    unsigned Menu::run(int isRunContext) const
    {
       if (!menuContent || countOptions() > 15)
       {
           std::cout << "Invalid Menu!" << std::endl;
            return 0;
        }
        unsigned int option;
        unsigned int maxOption = countOptions();

        if (!menuContent || maxOption == 0)
        {
            return 0;
        }

        const char* token = menuContent;
        for (unsigned int i = 0; i < maxOption; i++)
        {
            std::cout << (i + 1) << "- ";
            while (*token && *token != '\t')
            {
                std::cout << *token++;
            }
            if (*token) token++; 
            std::cout << std::endl;
        }


        if (isRunContext == 1) {
            std::cout << "---------------------------------" << std::endl;
        }
        else if (isRunContext == 2) {
            std::cout << "-----------------" << std::endl;
        }
        else {
        }
        std::cout << "0- Exit" << std::endl << "> ";

        while (true)
        {
            std::cin >> option;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid Integer, retry: ";
            }
            else if (option > maxOption)
            {
                std::cout << "Value out of range [0<=val<=" << maxOption << "]: ";
            }
            else
            {
                break;
            }
        }
        return option;
    }
}


