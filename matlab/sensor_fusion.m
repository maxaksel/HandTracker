% Script for debugging orientation tracking.
% 
% @date 04/28/2022
clear;clc;
baud_rate = 38400;
imu_sample_rate = 40;
device = serialport("COM7", baud_rate);

imu_filter = imufilter('SampleRate', imu_sample_rate);

% Align with incoming data
alignment_buffer = [0 0];
while ~isequal(alignment_buffer, [12*16+10 15*16+14])
    in = read(device, 1, "uint8");
    alignment_buffer = [alignment_buffer(2) in];
    disp(alignment_buffer);
end

disp("Aligned.");

% Read data from glove
angles = [0 0 0];
update = 0;
update_period = 250;
acc_buffer = zeros(250, 3);
gyro_buffer = zeros(250, 3);
while 1
    if update == 250
        [orientation, omega] = FUSE(acc_buffer, gyro_buffer);
        angles = angles + eulerd(orientation, 'ZYX', 'frame');
        fprintf("X angle: %f, Y angle: %f, Z angle: %f\n", angles(3), angles(2), angles(1));
        update = 0;
    end
    
    buffer = read(device, 10, 'int16');
    gyro = buffer(5:7) * 2000 / (2^16); % DPS
    acc = buffer(2:4) * 4 / (2^16); % g-forces
    acc_buffer(update) = gyro';
    gyro_buffer(update) = acc';
    update = update + 1;
end
