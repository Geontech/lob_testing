#include <iostream>
#include "ossie/ossieSupport.h"

#include "sensorIngest.h"


int main(int argc, char* argv[])
{
    sensorIngest_i* sensorIngest_servant;
    Resource_impl::start_component(sensorIngest_servant, argc, argv);
    return 0;
}
