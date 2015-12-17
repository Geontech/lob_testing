/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "sensorIngest.h"
#include <iostream>
using namespace std;

PREPARE_LOGGING(sensorIngest_i)

sensorIngest_i::sensorIngest_i(const char *uuid, const char *label) :
    sensorIngest_base(uuid, label)
{
	_isNewStream = true;
	_displayCnt = 0;
}

sensorIngest_i::~sensorIngest_i()
{
}

/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);

	Time:
	    To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through the getPacket call (BULKIO only).
        The dataTransfer class is a port-specific class, so each port implementing the
        BULKIO interface will have its own type-specific dataTransfer.

        The argument to the getPacket function is a floating point number that specifies
        the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        Each received dataTransfer is owned by serviceFunction and *MUST* be
        explicitly deallocated.

        To send data using a BULKIO interface, a convenience interface has been added 
        that takes a std::vector as the data input

        NOTE: If you have a BULKIO dataSDDS port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // this example assumes that the component has two ports:
            //  A provides (input) port of type bulkio::InShortPort called short_in
            //  A uses (output) port of type bulkio::OutFloatPort called float_out
            // The mapping between the port and the class is found
            // in the component base class header file

            bulkio::InShortPort::dataTransfer *tmp = short_in->getPacket(bulkio::Const::BLOCKING);
            if (not tmp) { // No databulkio::InShortPort::dataTransfer *tmp = short_in->getPacket(bulkio::Const::BLOCKING);
            if (not tmp) { // No data is available
                return NOOP;
            }

            std::vector<float> outputData;
            outputData.resize(tmp->dataBuffer.size());
            for (unsigned int i=0; i<tmp->dataBuffer.size(); i++) {
                outputData[i] = (float)tmp->dataBuffer[i];
            }

            // NOTE: You must make at least one valid pushSRI call
            if (tmp->sriChanged) {
                float_out->pushSRI(tmp->SRI);
            }
            float_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);
             is available
                return NOOP;
            }

            std::vector<float> outputData;
            outputData.resize(tmp->dataBuffer.size());
            for (unsigned int i=0; i<tmp->dataBuffer.size(); i++) {
                outputData[i] = (float)tmp->dataBuffer[i];
            }

            // NOTE: You must make at least one valid pushSRI call
            if (tmp->sriChanged) {
                float_out->pushSRI(tmp->SRI);
            }
            float_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);

            delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the std::vector passed from/to BulkIO can be typecast to/from
        std::vector< std::complex<dataType> >.  For example, for short data:

            bulkio::InShortPort::dataTransfer *tmp = myInput->getPacket(bulkio::Const::BLOCKING);
            std::vector<std::complex<short> >* intermediate = (std::vector<std::complex<short> >*) &(tmp->dataBuffer);
            // do work here
            std::vector<short>* output = (std::vector<short>*) intermediate;
            myOutput->pushPacket(*output, tmp->T, tmp->EOS, tmp->streamID);

        Interactions with non-BULKIO ports are left up to the component developer's discretion

    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given a generated name of the form
        "prop_n", where "n" is the ordinal number of the property in the PRF file.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (sensorIngest_base).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        cout << ""
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            ifshort (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
                dataOut[i] = dataIn[i];
            }
            
        A callback method can be associated with a property so that the method is
        called each time the property value changes.  This is done by calling 
        setPropertyChangeListener(<property name>, this, &sensorIngest_i::<callback method>)
        in the constructor.
            
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            
        //Add to sensorIngest.cpp
        sensorIngest_i::sensorIngest_i(const char *uuid, const char *label) :
            sensorIngest_base(uuid, label)
        {
            setPropertyChangeListener("scaleValue", this, &sensorIngest_i::scaleChanged);
        }

        void sensorIngest_i::scaleChanged(const std::string& id){
            std::cout << "scaleChanged scaleValue " << scaleValue << std::endl;
        }
            
        //Add to sensorIngest.h
        void scaleChanged(const std::string&);
        
        
************************************************************************************************/
int sensorIngest_i::serviceFunction()
{
    LOG_DEBUG(sensorIngest_i, "serviceFunction() example log message");
    
    float sum = 0.0;
    float dc_offset = 0.0;
    unsigned int n = 0;


    bulkio::InShortPort::dataTransfer *tmp = dataShort_in->getPacket(bulkio::Const::BLOCKING);
		if (not tmp) { // No data is available
			//cout << "NOOP" << endl;
			return NOOP;
		}

//cout << "BufferSize: " << tmp->dataBuffer.size() << " EOS " << tmp->EOS << endl;

		std::vector<float> outputData;

		n = tmp->dataBuffer.size();
		outputData.resize(n);

		for (unsigned int i=0; i<n; i++) {
			sum = sum + (float)tmp->dataBuffer[i];
		}

		dc_offset = sum / (float) n;

		for (unsigned int i=0; i<n; i++) {
			outputData[i] = (float)tmp->dataBuffer[i] - dc_offset;
		}

		unsigned long keySize = tmp->SRI.keywords.length();
		//LOG_INFO(sensorIngest_i,  " Size " << keySize);
		// Display Keywords in SRI

// Test for SRI Keywords
			for(unsigned long i=0; i< keySize; i++){
			//	if(strcmp(tmp->SRI.keywords[i].id, id)){
				CORBA::Long value;
				tmp->SRI.keywords[i].value >>= value;
			//	cout << "Kwd " << tmp->SRI.keywords[i].id << " Val " << value << " BufSize " << tmp->dataBuffer.size() << " Size " << keySize << endl;
			}

		//LOG_INFO(sensorIngest_i, "Sample Period" << tmp->SRI.xdelta);


		// NOTE: You must make at least one valid pushSRI call

		//if (tmp->sriChanged) {

		//Search for switch pattern keyword in incoming packets
/*	    std::string swpKeyword = "switch_pattern";
		long antennaSwitchState = 0;
		for(int b=0; b<tmp->SRI.keywords.length(); b++)
		{
			if(swpKeyword == (std::string)tmp->SRI.keywords[b].id)
			{
				tmp->SRI.keywords[b].value >>= antennaSwitchState;
				cout << "Keyword Len: " << tmp->SRI.keywords.length() << " Switch State: " << antennaSwitchState << endl;
			}
		}*/

		tmp->SRI.mode = 1; // RTL outputs complex data - JIM
			dataFloat_out->pushSRI(tmp->SRI);
		//}

			dataFloat_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);

        delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
        return NORMAL;

}
