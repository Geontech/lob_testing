function out = FMDemod(cmplxBBandIn)

% FM Demod with complex baseband input. 

size = length(cmplxBBandIn);
out = zeros(1,length(size));

phDly = 0;

%Loop through entire input file 
for count = 1:size
   s = cmplxBBandIn(count);
      
 % FM Demodulation via arctan % 
    ph = atan2(imag(s),real(s));

   %Unwrap Ph discontinuities
   tmp = unwrap([phDly ph]);
   ph = tmp(2);

   %calc radial freq
   w = ph - phDly;
   %save ph for next time through 
   phDly = ph;

   % Scale w by 32767/pi. Originally intended to represent the
   % radial frequency as a 16 bit value but not really needed here.
   % Note that the plots assume this scaling is there, otherwise they
   % Dont look nice.
   w=w*10430;

   count = count + 1; 
   out(count) = w;

end 
