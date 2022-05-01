% Script for calibrating IMU used for orientation tracking.
% 
% @date 04/30/2022

clear;clc;
baud_rate = 38400;
device = serialport("COM7", baud_rate);

FUSE = imufilter("SampleRate", 50);
config = tunerconfig('imufilter');
samples = 5000;
zero_quat = quaternion([0, 0, 0], 'eulerd', 'XYZ', 'frame');
ground_truth = table(repelem(zero_quat, samples)');
gyro_data = zeros(samples, 3);
acc_data = zeros(samples, 3);

disp("Calibrating IMU...");

% Align with incoming data
alignment_buffer = [0 0];
while ~isequal(alignment_buffer, [12*16+10 15*16+14])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(2) in];
    disp(alignment_buffer);
end
read(device, 12, "int16");
disp("Aligned.");

for i=1:samples
    buffer = read(device, 13, 'int16');
    gyro = buffer(5:7) * (2000 / (2^15)); % deg/s
    acc = buffer(2:4) * 2 * 9.81 / (2^15); % m/s^2 
    
    gyro_data(i, :) = gyro * pi/180; % rad/s
    acc_data(i, :) = acc; % m/s^2
end

sensor_data_table = table(acc_data, gyro_data, 'VariableNames', {'Accelerometer', 'Gyroscope'});

tune(FUSE, sensor_data_table, ground_truth, config);
