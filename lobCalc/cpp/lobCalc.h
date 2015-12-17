#ifndef LOBCALC_IMPL_H
#define LOBCALC_IMPL_H

#include "lobCalc_base.h"

class lobCalc_i;

class lobCalc_i : public lobCalc_base
{
    ENABLE_LOGGING
    public:
        lobCalc_i(const char *uuid, const char *label);
        ~lobCalc_i();
        int serviceFunction();
    private:
        bool _12valid;
        bool _23valid;
        bool _34valid;
        bool _41valid;
        float _antennaOneAvg;
        float _antennaTwoAvg;
        float _antennaThreeAvg;
        float _antennaFourAvg;
};

#endif
