% Script for calibrating resistor values.
% 
% @date 04/30/2022
clear;clc;
baud_rate = 38400;
device = serialport("COM7", baud_rate);
samples = 1000;

closed_buffer = zeros(samples, 5);
open_buffer = zeros(samples, 5);

% Align with incoming data
alignment_buffer = [0 0];
while ~isequal(alignment_buffer, [12*16+10 15*16+14])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(2) in];
    disp(alignment_buffer);
end
disp("Aligned.");
read(device, 9, "int16");

res = [0 0 0 0 0];

disp("Hold hand open...");
pause(2);
for i=1:samples
    buffer = read(device, 10, 'int16');
    res(1) = bitand(int16(buffer(8)), int16(hex2dec('00FF')), 'int16');
    
    res(2) = bitand(typecast(int16(buffer(8)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(2) = bitshift(uint16(res(2)), -8, 'uint16'); % shift upper bits to lower place
    
    res(3) = bitand(int16(buffer(9)), int16(hex2dec('00FF')), 'int16');
    
    res(4) = bitand(typecast(int16(buffer(9)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(4) = bitshift(uint16(res(4)), -8, 'uint16'); % shift upper bits to lower place
    
    res(5) = bitand(int16(buffer(10)), int16(hex2dec('00FF')), 'int16');
    
    closed_buffer(i, :) = res;
end
hand_open = mean(closed_buffer);
disp("Complete. Make a fist...");
pause(2);
for i=1:samples
    buffer = read(device, 10, 'int16');
    res(1) = bitand(int16(buffer(8)), int16(hex2dec('00FF')), 'int16');
    
    res(2) = bitand(typecast(int16(buffer(8)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(2) = bitshift(uint16(res(2)), -8, 'uint16'); % shift upper bits to lower place
    
    res(3) = bitand(int16(buffer(9)), int16(hex2dec('00FF')), 'int16');
    
    res(4) = bitand(typecast(int16(buffer(9)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(4) = bitshift(uint16(res(4)), -8, 'uint16'); % shift upper bits to lower place
    
    res(5) = bitand(int16(buffer(10)), int16(hex2dec('00FF')), 'int16');
    
    open_buffer(i, :) = res;
end
hand_closed = mean(open_buffer);

disp(hand_open);
disp(hand_closed);

save('res_cal.mat', 'hand_open', 'hand_closed');