% Script for debugging orientation tracking.
% 
% @date 04/29/2022
clear;clc;
baud_rate = 38400;
device = serialport("COM7", baud_rate);

% Set up Kalman filter
FUSE = imufilter("SampleRate", 50);

config = tunerconfig('imufilter');
load('imu_cal.mat');
tune(FUSE, sensor_data_table, ground_truth, config);
gyro_offset = mean(gyro_data);
acc_offset = mean(acc_data) - [0 0 9.81];

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
    buffer = read(device, 100, 'int16');
    buffer = reshape(buffer, [10,10])';
    gyro = buffer(:,5:7) * (2000 * pi / ((2^15) * 180)) - gyro_offset; % rad/s
    acc = buffer(:,2:4) * 2 * 9.81 / (2^15) - acc_offset; % g-forces
    q = FUSE(acc, gyro);
    ang = eulerd(q, "XYZ", 'frame');
    disp(ang);
end
