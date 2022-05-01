clear;clc;

lat = 21.3252 * pi / 180; % latitude in radians
long = -158.027 * pi / 180; % longitude in radians
altitude = 500; % altitude in [ units ]
Xgeodetic0 = [lat long altitude]; % latitude, longitude, altitude

baud_rate = 38400;
device = serialport("COM7", baud_rate);

% Set up Kalman filter
FUSE = imufilter("SampleRate", 50);

config = tunerconfig('imufilter');
load('imu_cal.mat');
tune(FUSE, sensor_data_table, ground_truth, config);
gyro_offset = mean(gyro_data);
acc_offset = mean(acc_data) - [0 0 9.81];

% Set up resistors
load('res_cal');

% Format calibration data to be brought into Simulink
acc_offset = [0 acc_offset];
gyro_offset = [0 gyro_offset];
hand_closed = [0 hand_closed];
hand_open = [0 hand_open];

% % Align with incoming data
% alignment_buffer = [0 0];
% while ~isequal(alignment_buffer, [12*16+10 15*16+14])
%     in = read(device, 1, "uint8");
%     alignment_buffer = [alignment_buffer(2) in];
%     disp(alignment_buffer);
% end
% disp("Aligned.");
% read(device, 9, "int16");
% 
% ang = zeros(10, 3);
% norm_res = [0 0 0 0 0];
% 
% time = 0;
% % Uses global variables: update, update_period, acc_buffer, and gyro_buffer
% while 1
%     buffer = read(device, 100, 'int16');
%     buffer = reshape(buffer, [10,10])';
%     gyro = buffer(:,5:7) * (2000 * pi / ((2^15) * 180)) - gyro_offset; % rad/s
%     acc = buffer(:,2:4) * 2 * 9.81 / (2^15) - acc_offset; % g-forces
%     q = FUSE(acc, gyro);
%     ang = [(time:0.01:time+0.09)' eulerd(q, "XYZ", 'frame')];
%     assignin('base', 'ang', ang)
%     
%     res_buffer = buffer(1, :);
%     res = [0 0 0 0 0];
%     res(1) = bitand(int16(res_buffer(8)), int16(hex2dec('00FF')), 'int16');
%     res(2) = bitand(typecast(int16(res_buffer(8)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
%     res(2) = bitshift(uint16(res_buffer(2)), -8, 'uint16'); % shift upper bits to lower place
%     res(3) = bitand(int16(res_buffer(9)), int16(hex2dec('00FF')), 'int16');
%     res(4) = bitand(typecast(int16(res_buffer(9)), "uint16"), uint16(hex2dec('FF00')), 'uint16');
%     res(4) = bitshift(uint16(res(4)), -8, 'uint16'); % shift upper bits to lower place
%     res(5) = bitand(int16(res_buffer(10)), int16(hex2dec('00FF')), 'int16');
%     
%     for res_index=1:5
%         norm_res(res_index) = (res(res_index) - hand_open(res_index)) * 200 / (hand_closed(res_index) - hand_open(res_index));
%     end
%     
%     input_buffer = buffer(1, :);
%     inputs = bitand(int16(input_buffer(10)), int16(hex2dec('FF00')), 'int16');
%     inputs = bitshift(int16(inputs), -8, 'int16'); % shift upper bits to lower place
%     
%     fprintf("Orientation: %f, %f, %f | Res: %f, %f, %f, %f, %f | Input: %d\n",...
%     ang(1, 2), ang(1, 3), ang(1, 4), norm_res(1), norm_res(2), norm_res(3), norm_res(4), norm_res(5), inputs);
%     time = time + 0.01;
% end