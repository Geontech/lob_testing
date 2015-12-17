#include <iostream>
#include "ossie/ossieSupport.h"

#include "lobCalcDummy.h"


int main(int argc, char* argv[])
{
    lobCalcDummy_i* lobCalcDummy_servant;
    Resource_impl::start_component(lobCalcDummy_servant, argc, argv);
    return 0;
}
