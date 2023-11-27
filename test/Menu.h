

#ifndef SDDS_MENU_H
#define SDDS_MENU_H

namespace sdds
{
    class Menu
    {
        char* menuContent;
        unsigned int countOptions() const;
        
    public:
        Menu(const char* text);
        ~Menu();
        Menu(const Menu&) = delete;
        Menu& operator=(const Menu&) = delete; 
        unsigned run() const;
    };

}

#endif //SDDS_MENU_H


