function plot_cfft (data,min_freq,max_freq,NFFT,avgs,title_string)

%PLOT_CFFT   Plots averaged complex FFT spectrum
%   plot_cfft (data, min_freq, max_freq, NFFT, avgs, title_string) generates
%   and displays an averaged FFT plot with frequency axis labeling. It
%   plots the entire NFFT points and reorders the data so the negative
%   frequencies are plotted first. Used for the decimated chip line
%   specrtal plot.
%     
%   Input values are:
%
%   data - vector data to be plotted
%
%   min_freq - the low end of the frequency scale to be plotted. 
%              Typically -1/2 sample rate
%
%   max_freq - the upper end of the frequency scale to be plotted. 
%              Typically -1/2 sample rate
%
%   NFFT - FFT size (should be power of 2)
%
%   avgs - Number of FFTs to average for the display plot
%
%   title_string - Title string for the figure
%
%   Limitations: 
%     - Need to call figure() before calling this function if plot is to be
%       generated in a new window each time
%     - Arbitrary amplitude scale used. Need to scale spectral plot 
%       based on input amplitude
%     - No windowing of overlap is currently performed
%
%
%    Jim McCloskey, Geon Technologies
%   Original Version: 5 Dec 2013

spec_data = zeros(1, NFFT);
start = 1;
stop = NFFT;

for n = 1:avgs
        
    spec_data = spec_data + abs(fft(data(start:stop),NFFT)/NFFT);

    start = start + NFFT;
    stop = stop + NFFT;
end

spec_data = 10*log(spec_data ./ avgs);
max_db = max(spec_data);
min_db = min(spec_data);

% Crude attempt to scale amplitude axis for spectral plots
if (max_db < 0.0) 
    max_db = 0.0; 
end
if (min_db > -50.0) 
    min_db = -50.0;
end

% Reorder the spectral data such that the negative frequencies are at the
% beginning of the array

spec_data = [spec_data(NFFT/2+1:NFFT) spec_data(1:NFFT/2)];


% generate vector for x axix with evenly spaced values from 
% min_freq to max_freq
f = linspace(min_freq,max_freq,NFFT);
%figure();

% Plot the entire complex spectrum 
plot(f, spec_data(1:NFFT));

axis([min_freq max_freq min_db max_db]);
grid on;
title(title_string);
xlabel('Frequency (Hz)');
ylabel('dB');





