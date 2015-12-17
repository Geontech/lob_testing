#include "pseudoDopplerSim_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

pseudoDopplerSim_base::pseudoDopplerSim_base(const char *uuid, const char *label) :
    Resource_impl(uuid, label),
    serviceThread(0)
{
    construct();
}

void pseudoDopplerSim_base::construct()
{
    Resource_impl::_started = false;
    loadProperties();
    serviceThread = 0;
    
    PortableServer::ObjectId_var oid;
    out = new bulkio::OutFloatPort("out");
    oid = ossie::corba::RootPOA()->activate_object(out);

    registerOutPort(out, out->_this());
}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void pseudoDopplerSim_base::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
}

void pseudoDopplerSim_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    if (serviceThread == 0) {
        serviceThread = new ProcessThread<pseudoDopplerSim_base>(this, 0.1);
        serviceThread->start();
    }
    
    if (!Resource_impl::started()) {
    	Resource_impl::start();
    }
}

void pseudoDopplerSim_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    // release the child thread (if it exists)
    if (serviceThread != 0) {
        if (!serviceThread->release(2)) {
            throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
        }
        serviceThread = 0;
    }
    
    if (Resource_impl::started()) {
    	Resource_impl::stop();
    }
}

CORBA::Object_ptr pseudoDopplerSim_base::getPort(const char* _id) throw (CORBA::SystemException, CF::PortSupplier::UnknownPort)
{

    std::map<std::string, Port_Provides_base_impl *>::iterator p_in = inPorts.find(std::string(_id));
    if (p_in != inPorts.end()) {
    }

    std::map<std::string, CF::Port_var>::iterator p_out = outPorts_var.find(std::string(_id));
    if (p_out != outPorts_var.end()) {
        return CF::Port::_duplicate(p_out->second);
    }

    throw (CF::PortSupplier::UnknownPort());
}

void pseudoDopplerSim_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the component running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    // deactivate ports
    releaseInPorts();
    releaseOutPorts();

    delete(out);

    Resource_impl::releaseObject();
}

void pseudoDopplerSim_base::loadProperties()
{
    addProperty(amplitude,
                1.0,
                "amplitude",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(carrierFreq,
                35000.0,
                "carrierFreq",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(sampleFreq,
                250000.0,
                "sampleFreq",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(lobDeg,
                30.0,
                "lobDeg",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(antennaDwell,
                500e-6,
                "antennaDwell",
                "",
                "readwrite",
                "",
                "external",
                "configure");

}
