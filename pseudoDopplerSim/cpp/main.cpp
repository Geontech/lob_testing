#include <iostream>
#include "ossie/ossieSupport.h"

#include "pseudoDopplerSim.h"


int main(int argc, char* argv[])
{
    pseudoDopplerSim_i* pseudoDopplerSim_servant;
    Resource_impl::start_component(pseudoDopplerSim_servant, argc, argv);
    return 0;
}
