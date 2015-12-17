#ifndef LOBCALCDUMMY_IMPL_H
#define LOBCALCDUMMY_IMPL_H

#include "lobCalcDummy_base.h"

class lobCalcDummy_i;

class lobCalcDummy_i : public lobCalcDummy_base
{
    ENABLE_LOGGING
    public:
        lobCalcDummy_i(const char *uuid, const char *label);
        ~lobCalcDummy_i();
        int serviceFunction();
    private:
        std::string _stream_id;
        short count;
};

#endif
