function out = complexCarrier(numSamps,Fs,Fc,phaseOffset)

% Fs = sample rate
% Fc = carrier offset
% phaseOffset = initial phase offset in radians

output_i = zeros(numSamps,1); 
output_q = zeros(numSamps,1);
phase   = phaseOffset;

for i=1:numSamps
    
      %calculate phase advance
      phase = phase + (2*pi*Fc)/Fs;
      
      %modulate unity amplitude signal 
      output_i(i) = cos(phase);
      output_q(i) = sin(phase);
      
 end 

 out = output_i + j*output_q;