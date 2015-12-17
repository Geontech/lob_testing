#include "lobCalc_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

lobCalc_base::lobCalc_base(const char *uuid, const char *label) :
    Resource_impl(uuid, label),
    serviceThread(0)
{
    construct();
}

void lobCalc_base::construct()
{
    Resource_impl::_started = false;
    loadProperties();
    serviceThread = 0;
    
    PortableServer::ObjectId_var oid;
    dataFloat_In = new bulkio::InFloatPort("dataFloat_In");
    oid = ossie::corba::RootPOA()->activate_object(dataFloat_In);

    registerInPort(dataFloat_In);
}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void lobCalc_base::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
}

void lobCalc_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    if (serviceThread == 0) {
        dataFloat_In->unblock();
        serviceThread = new ProcessThread<lobCalc_base>(this, 0.1);
        serviceThread->start();
    }
    
    if (!Resource_impl::started()) {
    	Resource_impl::start();
    }
}

void lobCalc_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    // release the child thread (if it exists)
    if (serviceThread != 0) {
        dataFloat_In->block();
        if (!serviceThread->release(2)) {
            throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
        }
        serviceThread = 0;
    }
    
    if (Resource_impl::started()) {
    	Resource_impl::stop();
    }
}

CORBA::Object_ptr lobCalc_base::getPort(const char* _id) throw (CORBA::SystemException, CF::PortSupplier::UnknownPort)
{

    std::map<std::string, Port_Provides_base_impl *>::iterator p_in = inPorts.find(std::string(_id));
    if (p_in != inPorts.end()) {
        if (!strcmp(_id,"dataFloat_In")) {
            bulkio::InFloatPort *ptr = dynamic_cast<bulkio::InFloatPort *>(p_in->second);
            if (ptr) {
                return ptr->_this();
            }
        }
    }

    std::map<std::string, CF::Port_var>::iterator p_out = outPorts_var.find(std::string(_id));
    if (p_out != outPorts_var.end()) {
        return CF::Port::_duplicate(p_out->second);
    }

    throw (CF::PortSupplier::UnknownPort());
}

void lobCalc_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
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

    delete(dataFloat_In);

    Resource_impl::releaseObject();
}

void lobCalc_base::loadProperties()
{
    addProperty(LOB_angle,
                0,
                "LOB_angle",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(LOB_valid,
                false,
                "LOB_valid",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(Threshold,
                0,
                "Threshold",
                "",
                "readwrite",
                "",
                "external",
                "configure");

}
