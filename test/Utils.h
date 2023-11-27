

#ifndef SDDS_UTILS_H
#define SDDS_UTILS_H

    namespace sdds
    {
        const int get_day = 9;
        const int get_month = 12;
        const int get_year = 2023;

        class Utils
    {
        bool m_testMode = false;

   public:

       void getSystemDate(int* a_year = nullptr, int* a_month = nullptr, int* a_day = nullptr);
       int monthDays(int a_month, int a_year)const;
       void testMode(bool a_testmode = true);
       void alocpy(char*& destination, const char* source);
       int getint(const char* prompt = nullptr);
       int getint(int min, int max, const char* prompt = nullptr, const char* errMes = nullptr);
       double getdouble(double min, double max, const char* prompt, const char* errMes);

   };

   extern Utils ut;

}

#endif // SDDS_UTILS_H 

