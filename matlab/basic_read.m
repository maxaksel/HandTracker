% Script for debugging serial input from HandTracker
% 
% @date 04/22/2022

device = serialport("COM3", 115200);

% Align with incoming data
alignment_buffer = [0 0 0 0];
while ~isequal(alignment_buffer, [0xC, 0xA, 0xF, 0xE])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(1:3) in];
end

disp("Aligned.");

% Read data from glove
while 1
    buffer = read(device, 7, "int16");
    gyro = buffer(2:4) * 2000; % DPS
    acc = buffer(5:7) * 4; % g-forces
end