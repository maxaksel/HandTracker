% Script for debugging orientation tracking.
% 
% @date 04/29/2022
clc
clear device;
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
load('res_cal.mat');


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
    res = buffer(1, 8:12);
    
    res_buffer = buffer(1, :);
    res = [0 0 0 0 0];                       
    res(1) = bitand(int16(res_buffer(8)), int16(hex2dec('00FF')), 'int16');
    res(2) = bitand(typecast(int16(res_buffer(8)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(2) = bitshift(uint16(res(2)), -8, 'uint16'); % shift upper bits to lower place
    res(3) = bitand(int16(res_buffer(9)), int16(hex2dec('00FF')), 'int16');
    res(4) = bitand(typecast(int16(res_buffer(9)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
    res(4) = bitshift(uint16(res(4)), -8, 'uint16'); % shift upper bits to lower place
    res(5) = bitand(int16(res_buffer(10)), int16(hex2dec('00FF')), 'int16');
    
    norm_res = [0 0 0 0 0];
    for res_index=1:5
        norm_res(res_index) = (res(res_index) - hand_open(res_index)) * 200 / (hand_closed(res_index) - hand_open(res_index));
    end
    
    fprintf("Orientation: (%f, %f, %f)    Fingers: %f, %f, %f, %f, %f\n", ang(1), ang(2), ang(3), norm_res(1), norm_res(2), norm_res(3), norm_res(4), norm_res(5));
    % disp(toc);
    % disp(ang);
    % disp(acc);
    % disp(gyro);
    % disp(buffer);
end
