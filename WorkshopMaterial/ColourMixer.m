% This script mixes colours.
% You have three primary colours as CIE coordinates with their relative intensity values.
% You want to set a particular colour as a CIE coordinate by mixing these three primary colours.
% Is the colour displayable?
% Proportionately how much do you need to drive your primaries?

clear all;
clc;


% These are my primaries, as measured
red_x = 0.71;
red_y = 0.29;
red_intensity = 0.9;

% During normal use, the x values of the green and blue primaries should be different. 
green_x = 0.13;
green_y = 0.77;
green_intensity = 0.8;

blue_x = 0.12;
blue_y = 0.06;
blue_intensity = 1;

% I want this colour.
requested_colour_x = 0.45;
requested_colour_y = 0.41;

%% Is the colour displayable?
% We can check this by verifying that the requested colour is inside the colour gamut, i.e. by verifying that on the X-Y plane, our point is within the
% triangle. Basically, the interior function. See this: http://mathworld.wolfram.com/TriangleInterior.html

value_a = ( (requested_colour_x * blue_y - requested_colour_y * blue_x) - (red_x * blue_y - red_y * blue_x) ) / (green_x * blue_y - green_y * blue_x);
value_b = ( (requested_colour_x * green_y - requested_colour_y * green_x) - (red_x * green_y - red_y * green_x) ) / (green_x * blue_y - green_y * blue_x);

colour_is_not_displayable = true;

% colour is displayable, if:
if( (value_a > 0) && (value_b > 0) )
    %... both values are greater than 0. But this is not all:
    if( (value_a + value_b) > 1 )
        % The sum of the two values has to be greater than one too.
        colour_is_not_displayable = false; % Only adjust this semaphore if the colour is displayable.
    end
end

% For Matlab users, this looks weird, but I did this so it's similar to what's in the C-code
if(colour_is_not_displayable)
    fprintf("Requested colour is: x=%.2f, y=%.2f\n", requested_colour_x, requested_colour_y)
    error("This colour can't be displayed, as it's outside the colour gamut!")
end


%% Proportionately how much do you need to drive your primaries?
% Now that we know we are within the colour gamut, we can calculate the distance from the requested colour's coordinate to the primaries.
% These are:

red_distance = sqrt( (red_x - requested_colour_x)^2 + (red_y - requested_colour_y)^2 );
green_distance = sqrt( (green_x - requested_colour_x)^2 + (green_y - requested_colour_y)^2 );
blue_distance = sqrt( (blue_x - requested_colour_x)^2 + (blue_y - requested_colour_y)^2 );

% These distances are INVERSELY PROPORTIONAL to the intensity of the primary colours.
% So the closer the requested colour to the primary is, the more you need of it.
% But this is not enough, because these are just the distances and not the proportions of intensity.
% We need to find the point where the line stating from a pimary colour's coordinates that goes across the requested colour's coordinates hit the opposite side
% of the triangle: We need to make two lines, and calculate the intersection. See this: http://zonalandeducation.com/mmts/intersections/intersectionOfTwoLines1/intersectionOfTwoLines1.html

% These are the lines between the requested colour and the primaries
red_line_slope = (requested_colour_y - red_y) / (requested_colour_x - red_x);
red_line_intercept = requested_colour_y - red_line_slope * requested_colour_x;

green_line_slope = (requested_colour_y - green_y) / (requested_colour_x - green_x);
green_line_intercept = requested_colour_y - green_line_slope * requested_colour_x;
infinite_green_slope = false;

% The green slope can be infinite! This happens when the colour in question is directly below the green primary. This just means that we can calculate the
% intersection point much easier.
if( (requested_colour_x - green_x) == 0)
    infinite_green_slope = true;
end

blue_line_slope = (requested_colour_y - blue_y) / (requested_colour_x - blue_x);
blue_line_intercept = requested_colour_y - blue_line_slope * requested_colour_x;

% And now we calculate the line equations for the colour gamut too
blue_green_line_slope = (blue_y - green_y) / (blue_x - green_x);
blue_green_line_intercept = blue_y - blue_green_line_slope * blue_x;

red_blue_line_slope = (red_y - blue_y) / (red_x - blue_x);
red_blue_line_intercept = red_y - red_blue_line_slope * red_x;

green_red_line_slope = (green_y - red_y) / (green_x - red_x);
green_red_line_intercept = green_y - green_red_line_slope * green_x;


% Now we can calculate the intersection points.

% Red line - blue-green line intersection
red_intersection_x = (blue_green_line_intercept - red_line_intercept) / (red_line_slope - blue_green_line_slope);
red_intersection_y = red_line_slope * red_intersection_x + red_line_intercept;

% Green line - red-blue line intersection. This is the one that CAN get simple
if(infinite_green_slope)
    % If we have infinite slope, we are going down. So the X coordinate is alread solved!
    green_intersection_x = green_x;
else
    green_intersection_x = (red_blue_line_intercept - green_line_intercept) / (green_line_slope - red_blue_line_slope);
end
green_intersection_y = green_line_slope * green_intersection_x + green_line_intercept;

% Blue line - green-red line intersection
blue_intersection_x = (green_red_line_intercept - blue_line_intercept) / (blue_line_slope - green_red_line_slope);
blue_intersection_y = blue_line_slope * blue_intersection_x + blue_line_intercept;


% And now we can calculate the lengths.

red_line_length = sqrt( (red_x - red_intersection_x)^2 + (red_y - red_intersection_y)^2 );
green_line_length = sqrt( (green_x - green_intersection_x)^2 + (green_y - green_intersection_y)^2 );
blue_line_length = sqrt( (blue_x - blue_intersection_x)^2 + (blue_y - blue_intersection_y)^2 );


% We can now take the intensities into account.
% An intensity of 0.2 means that it would have to be five times as luminous to produce as much light output as intensity of 1.

red_drive = (1 - (red_distance / red_line_length)) / red_intensity;
green_drive = (1 - (green_distance / green_line_length)) / green_intensity;
blue_drive = (1 - (blue_distance / blue_line_length)) / blue_intensity;


% and we are STILL not done, because we ideally should maximise the drive on the highest colour channel.

max_drive_value = max([red_drive, green_drive, blue_drive]);

% Scale the values to something a microcontroller can understand: from 0-1 to 0-255.

red_pwm_value = round(red_drive / max_drive_value * 255)
green_pwm_value = round(green_drive / max_drive_value * 255)
blue_pwm_value = round(blue_drive / max_drive_value * 255)








