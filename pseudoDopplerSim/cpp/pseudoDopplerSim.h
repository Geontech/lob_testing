/* pseudoDopplerSim.h
 * Header for 4 Antenna Pseudo-doppler DF Simulation RedHawk Component
 * Brandon Luquette
 * (c) 2013 Geon Technologies, LLC
 * 13 Dec 2013
 */
#ifndef PSEUDODOPPLERSIM_IMPL_H
#define PSEUDODOPPLERSIM_IMPL_H

#include "pseudoDopplerSim_base.h"

class pseudoDopplerSim_i;

class pseudoDopplerSim_i : public pseudoDopplerSim_base
{
    ENABLE_LOGGING
    public:
        pseudoDopplerSim_i(const char *uuid, const char *label);
        ~pseudoDopplerSim_i();
        int serviceFunction();
    private:
        BULKIO::StreamSRI _sri;
        std::string _stream_id;
        double _initialPhase1;
        double _initialPhase2;
        double _initialPhase3;
        double _initialPhase4;
        double _lobRad;
        int _whichAntennas;
        int _antennaPackets;
        int _numSampsToDwell;
        int _numComplexSamples;

        bool sri_changed;
        float start_phase;
		void propertyChanged(const std::string& id);

		//Declare vectors for antennas
	    std::vector<float> antennaOne;
	    std::vector<float> antennaTwo;
	    std::vector<float> antennaThree;
	    std::vector<float> antennaFour;

	    long _switch_pattern;
	    unsigned long _size;
};

#endif
