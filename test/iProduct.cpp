

#include "iProduct.h"

namespace sdds
{

    std::istream& operator>>(std::istream& istr, iProduct& a_iP)
    {
        return a_iP.read(istr);
    }

    std::ostream& operator<<(std::ostream& ostr, const iProduct& a_iP)
    {
        return a_iP.display(ostr);
    }

}
