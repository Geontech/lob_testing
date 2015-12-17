#include <iostream>
#include "ossie/ossieSupport.h"

#include "lobCalc.h"


int main(int argc, char* argv[])
{
    lobCalc_i* lobCalc_servant;
    Resource_impl::start_component(lobCalc_servant, argc, argv);
    return 0;
}
