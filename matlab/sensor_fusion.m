% Script for debugging orientation tracking.
% 
% @date 04/29/2022
clear;clc;
baud_rate = 38400;
imu_sample_rate = 50; % 50 Hz
device = serialport("COM7", baud_rate);

FUSE = imufilter('SampleRate', imu_sample_rate);

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
update = 0;
update_period = 10;
acc_buffer = zeros(update_period, 3);
gyro_buffer = zeros(update_period, 3);

% update_timer = timer('ExecutionMode', 'fixedRate', 'Period', 1/imu_sample_rate, 'TimerFcn', @update_orientation);
% start(update_timer);

% Uses global variables: update, update_period, acc_buffer, and gyro_buffer
while 1
    tic
    % Read data from glove
    if update == update_period
        [orientation, ~] = FUSE(acc_buffer, gyro_buffer);
        angles = eulerd(orientation, 'ZYX', 'frame');
        angles = mod(angles, 360);
        fprintf("X angle: %f, Y angle: %f, Z angle: %f\n", angles(3), angles(2), angles(1));
        update = 0;
        % disp(angles);
    end

    buffer = read(device, 10, 'int16');
    gyro = buffer(5:7) * (2000 / (2^15)) * (pi / 180); % rad/s
    acc = buffer(2:4) * 2 * 9.81 / (2^15); % g-forces
    acc_buffer(update + 1, :) = gyro;
    gyro_buffer(update + 1, :) = acc;
    update = update + 1;
    pause(1/50 - toc)
end
