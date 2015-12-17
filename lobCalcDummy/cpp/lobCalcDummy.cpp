#include "lobCalcDummy.h"

PREPARE_LOGGING(lobCalcDummy_i)

lobCalcDummy_i::lobCalcDummy_i(const char *uuid, const char *label) :
    lobCalcDummy_base(uuid, label)
{
	_stream_id = "DFStream";
	count = 0;
}

lobCalcDummy_i::~lobCalcDummy_i()
{
}

int lobCalcDummy_i::serviceFunction()
{
		LOG_DEBUG(lobCalcDummy_i, "serviceFunction() example log message");

	    bulkio::InFloatPort::dataTransfer *tmp = dataFloat_In->getPacket(bulkio::Const::BLOCKING);
	            if (not tmp) { // No data is available
	                return NOOP;
	            }

	            //Counter to control change of LOB
	            if (count < packetsBeforeChange){
	            	count++;
	            }
	            else{
	            	count = 0;
	            }

	            //Increment LOB by 1 each packet
	            if(count ==0){
	            	if(lob == 90){
	            		lob = 0;
	            	}
	            	else{
	            		lob = lob + 1;
	            	}
	            }

	            //Increment switch pattern
				if(count ==0){
					if(switchPattern_prop == 3){
						switchPattern_prop = 0;
					}
					else{
						switchPattern_prop = switchPattern_prop + 1;
					}
				}

	            //Valid is high for the second half of packets
	            if (count < packetsBeforeChange/2){
	            	valid = false;
	            }
	            else{
	            	valid = true;
	            }

	            //Extract Stream ID from incoming packet
	            streamID_prop = tmp->SRI.streamID;

	            delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
	            return NORMAL;
}
