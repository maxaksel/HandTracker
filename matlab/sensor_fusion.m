% Script for debugging orientation tracking.
% 
% @date 04/29/2022
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
read(device, 12, "int16");
FUSE = imufilter("SampleRate", 50, "DecimationFactor", 10);


angles = [0 0 0];

% Uses global variables: update, update_period, acc_buffer, and gyro_buffer
while 1
    tic
    buffer = read(device, 130, 'int16');
    buffer = reshape(buffer, [13, 10])';
    gyro = buffer(:, 5:7) * (2000 * (pi) / ((2^15)*180)); % rad/s
    acc = buffer(:,2:4) * 2 * 9.81 / (2^15); % g-forces
    q = FUSE(acc, gyro);
    ang = eulerd(q, "XYZ", 'frame');
    
    % disp(toc);
    disp(ang);
    % disp(acc);
    % disp(gyro);
    % disp(buffer);
end
