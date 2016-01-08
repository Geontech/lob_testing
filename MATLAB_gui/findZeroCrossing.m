function index = findZeroCrossing(data)

% Returns the index of the first positive going zero crossing in the data.
% Returns 0 if no index is found

size = length(data);
zeroCrossingFound = false;

prevData = data(1);

%Loop through entire input file 
for i = 2:size

    if (prevData < 0) && (data(i) >= 0)
        index = i;
        zeroCrossingFound = true;
        break;
    else
        prevData = data(i);
    end
end 

if ~zeroCrossingFound
    index = 0;
end
