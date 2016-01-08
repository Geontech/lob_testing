function baseband = ddc (data, fs, fo, f_lpf)

% Inputs a carrier @ fo,  
% downconverts and filters to complex baseband signal. 
%
% Inputs:
% data - real array of signal samples with sample rate fs. 
%
% fs - sample rate in Hz
%
% fo - carrier frequency of the input signal in Hz
%
% f_lpf -  Used to set the baseband lowpass filter cut-off following the mixers
%


num_samples = length(data);
t = (0: 1/fs : (num_samples-1)/fs);

% multiply samples by complex LO tuned to the carrier frequency

fo_offset    = 0; %Offset for test and experimentation
phase_offset = 0; %Offset for test and experimentation
baseband = data .* (cos(2*pi*(fo+fo_offset)*t + phase_offset*pi) - ...
                  j*sin(2*pi*(fo+fo_offset)*t + phase_offset*pi));

% filter baseband I&Q data nominally to the chip rate fc
taps = 64; % # actual filter taps - 1
wn = (f_lpf)/(fs/2);
b = fir1(taps, wn);
baseband = filter(b,1,baseband);

% Note that we do not decimate the data at this point. Follow-on 
% processing such as feature detection will perform decimation as
% required
