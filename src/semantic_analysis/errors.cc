#include <string>
#include "errors.h"
namespace saERRORS{
    std::string toString(ERROR_TYPE x)
    {
        switch(x)
        {
            case NO_ERROR:
                return "No error";
            case NOT_FOUND:
                return "Not found";
            case FOUND_BUT_NOT_MATCH:
                return "Found but not match";
            case ITEM_ERROR:
                return "Item error";
            case ITEM_EXIST:
                return "Item exist";
        }
        return "error";
    }
}