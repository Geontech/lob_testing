#ifndef SENSORINGEST_IMPL_H
#define SENSORINGEST_IMPL_H

#include "sensorIngest_base.h"

class sensorIngest_i;

class sensorIngest_i : public sensorIngest_base
{
    ENABLE_LOGGING
    public:
        sensorIngest_i(const char *uuid, const char *label);
        ~sensorIngest_i();
        int serviceFunction();

    private:
        bool _isNewStream;
        int _displayCnt;
};

#endif
