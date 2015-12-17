/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "lobCalc.h"

PREPARE_LOGGING(lobCalc_i)

lobCalc_i::lobCalc_i(const char *uuid, const char *label) :
    lobCalc_base(uuid, label)
{
	_12valid = false;
	_23valid = false;
	_34valid = false;
	_41valid = false;
}

lobCalc_i::~lobCalc_i()
{
}


int lobCalc_i::serviceFunction()
{
    LOG_DEBUG(lobCalc_i, "serviceFunction() example log message");
    bulkio::InFloatPort::dataTransfer *tmp = dataFloat_In->getPacket(bulkio::Const::BLOCKING);
	if (not tmp) { // No data is available
		return NOOP;
	}

	//Search for switch pattern keyword in incoming packets
    std::string swpKeyword="switch_pattern";
	long antennaSwitchState;
	for(int b=0;b<tmp->SRI.keywords.length();b++){
		if("switch_pattern"==(std::string)tmp->SRI.keywords[b].id){
			tmp->SRI.keywords[b].value>>=antennaSwitchState;
		}
	}//Could add a check here for not being able to find the switch_pattern keyword

    //Check if antennaSwitchState is > 0
	if (antennaSwitchState == 0){
		//If 0, set valid variables false and valid property false for component
		std::cout << "DF mode off" << std::endl;
		_12valid = false;
		_23valid = false;
		_34valid = false;
		_41valid = false;
		_antennaOneAvg = 0;
		_antennaTwoAvg = 0;
		_antennaThreeAvg = 0;
		_antennaFourAvg = 0;
		delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
		return NORMAL;
	}
	else{
		//Detect peaks
	    //Generate peak packet
		std::vector<float> peaks;
		std::vector<int> indexes;
		float currentSample;
		float currentSamplePlusOne;
		float currentSampleMinusOne;
		float peakAverage, peakSum;

		peaks.clear();
		indexes.clear();

        for (unsigned int i=1; i<tmp->dataBuffer.size()-1; i++) {
    		currentSample = (float)tmp->dataBuffer[i];
    		currentSamplePlusOne = (float)tmp->dataBuffer[i+1];
    		currentSampleMinusOne = (float)tmp->dataBuffer[i-1];
        	//If sample is greater than previous sample and greater than next sample, it's a peak
            if(currentSample > currentSampleMinusOne && currentSample > currentSamplePlusOne){
            	//if > threshold, save peak and index
            	if(currentSample>Threshold){
            		peaks.push_back(currentSample);
            		indexes.push_back(i);
            	}
            }
        }
        //if peaks > 0
        if(!peaks.empty()){
            //Average peak values
        	peakAverage=0;
        	peakSum = 0;
            for(int a=0;a<peaks.size();a++){
                    peakSum+=peaks[a];
            }
            peakAverage = peakSum/peaks.size();
        }

        //Set valid variable true for this pair of antennas
        switch (antennaSwitchState)
        {
        case 1 :
        	_12valid = true;
        	std::cout << "Peak 1 average: " << peakAverage << std::endl;
        	_antennaOneAvg = peakAverage;
            break;
        case 2 :
        	_23valid = true;
        	std::cout << "Peak 2 average: " << peakAverage << std::endl;
        	_antennaTwoAvg = peakAverage;
            break;
        case 3 :
        	_34valid = true;
        	std::cout << "Peak 3 average: " << peakAverage << std::endl;
        	_antennaThreeAvg = peakAverage;
            break;
        default:
        	_41valid = true;
        	std::cout << "Peak 4 average: " << peakAverage << std::endl;
        	_antennaFourAvg = peakAverage;
        	break;
        }

        //Check if all valid variables are true
        if(_12valid && _23valid && _34valid && _41valid){
        	//Set valid property true for component
        	LOB_valid = true;
        }else{
        	LOB_valid = false;
        }


		//Calculate and update LOB
        LOB_angle = 30;



        delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
		return NORMAL;
	}
}
