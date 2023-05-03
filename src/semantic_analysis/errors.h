#ifndef ERRORS_H
#define ERRORS_H
#include <string>
namespace saERRORS{
    enum ERROR_TYPE{
        NO_ERROR,
        NOT_FOUND,
        FOUND_BUT_NOT_MATCH,
        ITEM_ERROR,
        ITEM_EXIST
    };
    std::string toString(ERROR_TYPE x);
}
#endif