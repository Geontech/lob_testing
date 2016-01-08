function [pos, mag] = peakfind(tsig, tx);
% This fuction will find the positions and magnitudes of the local
% maxima or peaks in vector SIG.
%
%    [pos, mag] = peakfind(sig, tx)
%
% vector TX is the time index for SIG, and peak locations are NOT
% interpolated so it will return the last sample point before a
% negative decent begins .
%  
% The output values POS are the positions of the local maximum in terms
% of the axis vector TX, and MAG are the values of the peaks.  POS and MAG
% will always include the endpoints of the array. 
%
%   Written by Kevin Donohue 2/2002  donohue@engr.uky.edu

%  Get size of input vector
[r,c] = size(tsig);
%  Compute gradient
dsig = gradient(tsig);
%  Start counter for local maxima
count = 1;
%  Initialize with first endpoint
mag(count) = tsig(1);
pos(count) = tx(1);

%  Loop to find places where gradient changes sign
%  and record them as local maxima
for k=2:length(dsig)
    %  If gradient change sign from positive to negative
    if(dsig(k) < 0 & dsig(k-1) >= 0)
        count = count +1;  %   Increment peak counter
        [dum, idum] = max(tsig((k-1):k)); % Find the larger of the values where this
                                          %  Gradient change occured
        mag(count) = dum(1);              % Assign to magnitude value
        pos(count) = tx(k+idum-2);        % Assigne to position value
    end
end

%  If the last end point was not identified as a peak,
%  include it anyway
if pos(count) ~= tx(length(tx))
    pos(count+1) = tx(length(tx));
    mag(count+1) = tsig(length(tx));
end
