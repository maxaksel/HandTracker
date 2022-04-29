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
read(device, 9, "int16");

angles = [0 0 0];

% Uses global variables: update, update_period, acc_buffer, and gyro_buffer
while 1
    tic
    buffer = read(device, 10, 'int16');
    gyro = buffer(5:7) * (2000 / (2^15)); % deg/s
    acc = buffer(2:4) * 2 * 9.81 / (2^15); % g-forces
    acc(3) = acc(3) * -1; % transform to right-handed reference
    acc_norm = norm(acc);
    
    if acc_norm > 9.6 && acc_norm < 10.0
        angles(1) = atan(acc(1) / sqrt(acc(2)^2 + acc(3)^2)) * 180 / pi;
        angles(2) = atan(acc(2) / sqrt(acc(1)^2 + acc(3)^2)) * 180 / pi;
        angles(3) = atan(sqrt(acc(1)^2 + acc(2)^2) / acc(3)) * 180 / pi;
    else
        angles = angles + gyro * toc;
    end
    
    disp(angles);
end
