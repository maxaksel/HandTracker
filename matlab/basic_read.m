% Script for debugging serial input from HandTracker
% 
% @date 04/22/2022
clear;clc;
baud_rate = 38400;
device = serialport("COM7", baud_rate);

% Align with incoming data
alignment_buffer = [0 0];
while ~isequal(alignment_buffer, [12*16+10 15*16+14])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(2) in];
    disp(alignment_buffer);
end

disp("Aligned.");

% Read data from glove
while 1
    buffer = read(device, 10, 'int16');
    % disp(buffer);
    gyro = buffer(5:7) * 2000 / (2^16); % DPS
    acc = buffer(2:4) * 4 / (2^16); % g-forces
    % disp(buffer);
    % disp(buffer);
    res = [0 0 0 0 0];
    res(1) = bitand(int16(buffer(8)), int16(hex2dec('FF00')), 'int16');
    res(1) = bitshift(res(1), -8, 'int16'); % shift upper bits to lower place
    res(2) = bitand(int16(buffer(8)), int16(hex2dec('00FF')), 'int16');
    res(3) = bitand(int16(buffer(9)), int16(hex2dec('FF00')), 'int16');
    res(3) = bitshift(res(3), -8, 'int16'); % shift upper bits to lower place
    res(4) = bitand(int16(buffer(9)), int16(hex2dec('00FF')), 'int16');
    res(5) = bitand(int16(buffer(10)), int16(hex2dec('FF00')), 'int16');
    res(5) = bitshift(res(5), -8, 'int16'); % shift upper bits to lower place
    inputs = bitand(int16(buffer(10)), int16(hex2dec('00FF')), 'int16');
    
    disp(acc);
    % fprintf("Gyro: %f, %f, %f | Acc: %f, %f, %f | Res: %d, %d, %d, %d, %d | Input: %d\n",...
    % gyro(1), gyro(2), gyro(3), acc(1), acc(2), acc(3), res(1), res(2), res(3), res(4), res(5), inputs);
end
