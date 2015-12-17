/* pseudoDopplerSim.cpp
 * Source for 4 Antenna Pseudo-doppler DF Simulation RedHawk Component
 * Brandon Luquette
 * (c) 2013 Geon Technologies, LLC
 * 13 Dec 2013
 */

#include "pseudoDopplerSim.h"
//#define USE_MATH_DEFINES
#include <cmath>
#include <string>
#include "Waveform.h"

PREPARE_LOGGING(pseudoDopplerSim_i)


void pseudoDopplerSim_i::propertyChanged(const std::string& id)
{
		sri_changed = true;
}

pseudoDopplerSim_i::pseudoDopplerSim_i(const char *uuid, const char *label) :
    pseudoDopplerSim_base(uuid, label)
{

	//Set SRI parameters
	_stream_id = "DFStream";
	_sri = bulkio::sri::create(_stream_id);
	_sri.xdelta = 1.0/sampleFreq;
	_sri.mode = 1; // complex
    _size = _sri.keywords.length();
    _sri.keywords.length(_size + 1);
    _sri.keywords[_size].id = CORBA::string_dup("switch_pattern");


	//Calculate number of samples for each antenna
    _numSampsToDwell = int(sampleFreq * antennaDwell);
    _numComplexSamples = 2*_numSampsToDwell;

	//Calculate LOB in radians
	_lobRad=lobDeg*M_PI/180;

	//Calculate initial phase for each antenna
	_initialPhase1 = 0.0;
	_initialPhase2 = -M_PI/2*sin(_lobRad)/(2*M_PI);
	_initialPhase3 = -M_PI/2*(sin(_lobRad)+cos(_lobRad))/(2*M_PI);
	_initialPhase4 = -M_PI/2*cos(_lobRad)/(2*M_PI);

	//Set up PropertyChangeListeners
	setPropertyChangeListener("amplitude", this, &pseudoDopplerSim_i::propertyChanged);
	setPropertyChangeListener("carrierFreq", this, &pseudoDopplerSim_i::propertyChanged);
	setPropertyChangeListener("sampleFreq", this, &pseudoDopplerSim_i::propertyChanged);
	setPropertyChangeListener("lobDeg", this, &pseudoDopplerSim_i::propertyChanged);
	setPropertyChangeListener("antennaDwell", this, &pseudoDopplerSim_i::propertyChanged);

	//Initialize counters for when to switch antennas
	_whichAntennas = 0;
	_antennaPackets = 0;

}

pseudoDopplerSim_i::~pseudoDopplerSim_i()
{
}

int pseudoDopplerSim_i::serviceFunction()
{

	LOG_DEBUG(pseudoDopplerSim_i, "serviceFunction() example log message");

	BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

    double deltaPhi = (carrierFreq/sampleFreq);

	std::vector<float> outputData;
	outputData.resize(_numComplexSamples*4);

    //Resize vectors
    //The multiple of two is because the data is complex
    //For each antenna, generate a full rotation of samples
    antennaOne.resize(_numComplexSamples*4);
    antennaTwo.resize(_numComplexSamples*4);
    antennaThree.resize(_numComplexSamples*4);
    antennaFour.resize(_numComplexSamples*4);

    _initialPhase1 = Waveform::sincos(antennaOne,   amplitude, _initialPhase1, deltaPhi, _numSampsToDwell*4, -2);
	_initialPhase2 = Waveform::sincos(antennaTwo,   amplitude, _initialPhase2, deltaPhi, _numSampsToDwell*4, -2);
	_initialPhase3 = Waveform::sincos(antennaThree, amplitude, _initialPhase3, deltaPhi, _numSampsToDwell*4, -2);
	_initialPhase4 = Waveform::sincos(antennaFour,  amplitude, _initialPhase4, deltaPhi, _numSampsToDwell*4, -2);

	//Every 10 packets, switch between antenna patterns
	if(_antennaPackets > 10){
		_antennaPackets = 0;
		if(_whichAntennas > 3){
			_whichAntennas = 0;
		}
		else{
			_whichAntennas++;
		}
	}
	else{
		_antennaPackets++;
	}

	int beginIndex = 0;
	int endIndex   = _numComplexSamples;

	if(_whichAntennas == 0){
		_switch_pattern = 1;
		//Switch between antenna one and two
		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaOne[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaTwo[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaOne[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaTwo[i];
	}
	else if(_whichAntennas == 1){
		_switch_pattern = 2;
		//Switch between antenna two and three
		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaTwo[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaThree[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaTwo[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaThree[i];
	}
	else if(_whichAntennas == 2){
		_switch_pattern = 3;
		//Switch between antenna three and four
		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaThree[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaFour[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaThree[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaFour[i];
	}
	else{
		_switch_pattern = 4;
		//Switch between antenna four and one
		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaFour[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaOne[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaFour[i];

		beginIndex = endIndex;
		endIndex  = endIndex + _numComplexSamples;

		for (int i = beginIndex; i<endIndex; i++)
			outputData[i] = antennaOne[i];
	}

	//Update Switch Pattern Keyword
	_sri.keywords[_size].value <<= _switch_pattern;

	//Push packet
	out->pushSRI(_sri);
	out->pushPacket(outputData, tstamp, false, _stream_id);

	return NORMAL;

}
