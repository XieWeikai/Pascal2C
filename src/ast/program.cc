#include <string>
#include <sstream>

#include "program.h"

namespace pascal2C::ast
{
    using string = ::std::string;
    using stringstream = ::std::stringstream;

    const string IdList::ToString(const int &level) const
    {
        std::stringstream str_s;
        IndentOutput(str_s, level);
        str_s << "IdList: ";

        for (int i = 0; i < id_list_.size(); i++)
        {
            IndentOutput(str_s, level);
            str_s << "expr " << i + 1 << ":\n"
                  << params_[i]->ToString(level + 1);
        }
        return str_s.str();
    }
}
