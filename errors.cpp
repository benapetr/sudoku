#include "errors.h"
#include "global.h"



QString Errors::ToString(int error)
{
    switch(error)
    {
        case E_ALREADY_USED:
            return "Value is already used";
        case E_NOT_EMPTY:
            return "This item already has a value";
        case E_INVALID_COL:
            return "Invalid column";
        case E_INVALID_ROW:
            return "Invalid row";
        case E_INVALID_VALUE:
            return "Invalid value";
        case E_READ_ONLY:
            return "This field is read only, you can only change it in editor mode";
        case E_NO_SOLUTIONS:
            return "Unable to find a solution";
    }
    return "Unknown error (" + QString::number(error) + ")";
}


