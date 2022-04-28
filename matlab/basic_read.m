% Script for debugging serial input from HandTracker
% 
% @date 04/22/2022

baud_rate = 115200;
device = serialport("COM3", baud_rate);

% Align with incoming data
alignment_buffer = [0 0 0 0];
while ~isequal(alignment_buffer, [0xC, 0xA, 0xF, 0xE])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(1:3) in];
end

disp("Aligned.");

% Read data from glove
while 1
    buffer = read(device, 10, 'int16');
    gyro = buffer(2:4) * 2000; % DPS
    acc = buffer(5:7) * 4; % g-forces
    res = [0 0 0 0 0];
    res(1) = bitand(buffer(8), hex2dec('FF00'), 'uint16');
    res(1) = bitshift(res(1), -8, 'uint16'); % shift upper bits to lower place
    res(2) = bitand(buffer(8), hex2dec('00FF'), 'uint16');
    res(3) = bitand(buffer(9), hex2dec('FF00'), 'uint16');
    res(3) = bitshift(res(3), -8, 'uint16'); % shift upper bits to lower place
    res(4) = bitand(buffer(9), hex2dec('00FF'), 'uint16');
    res(5) = bitand(buffer(10), hex2dec('FF00'), 'uint16');
    res(5) = bitshift(res(5), -8, 'uint16'); % shift upper bits to lower place
    inputs = bitand(buffer(10), hex2dec('00FF'), 'uint16');
    
    fprintf("Gyro: %f, %f, %f | Acc: %f, %f, %f | Res: %d, %d, %d, %d, %d | Input: %d\n",...
    gyro(1), gyro(2), gyro(3), acc(1), acc(2), acc(3), res(1), res(2), res(3), res(4), res(5), inputs);
end
