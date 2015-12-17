#include "lobCalcDummy_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

lobCalcDummy_base::lobCalcDummy_base(const char *uuid, const char *label) :
    Resource_impl(uuid, label),
    serviceThread(0)
{
    construct();
}

void lobCalcDummy_base::construct()
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
void lobCalcDummy_base::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
}

void lobCalcDummy_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    boost::mutex::scoped_lock lock(serviceThreadLock);
    if (serviceThread == 0) {
        dataFloat_In->unblock();
        serviceThread = new ProcessThread<lobCalcDummy_base>(this, 0.1);
        serviceThread->start();
    }
    
    if (!Resource_impl::started()) {
    	Resource_impl::start();
    }
}

void lobCalcDummy_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
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

CORBA::Object_ptr lobCalcDummy_base::getPort(const char* _id) throw (CORBA::SystemException, CF::PortSupplier::UnknownPort)
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

void lobCalcDummy_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
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

void lobCalcDummy_base::loadProperties()
{
    addProperty(lob,
                0,
                "lob",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(valid,
                true,
                "valid",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(packetsBeforeChange,
                10,
                "packetsBeforeChange",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(switchPattern_prop,
                0,
                "switchPattern_prop",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(streamID_prop,
                "streamID_prop",
                "",
                "readwrite",
                "",
                "external",
                "configure");

}
