%Something is wrong with this. There doesn't seem to be any input from the
%accelerometer used anywhere.


%Read data file
s = importdata('temp.txt');


%Constants/stuff to deal with later
var_angle = 0.05;
var_gyro_resid = 0.15;

x_accel = s(:,1);
y_accel = s(:,2);
z_accel = s(:,3);
pitch_v = s(:,4);
roll_v = s(:,5);
yaw_v = s(:,6);

%Time scale
delta_time = 0.2;

%Angles and velocities from accelerometer
pitch = atan2(sqrt((y_accel.^2)+(z_accel.^2)),x_accel);
pitch_v = diff(pitch)/delta_time;
%beta = atan2(sqrt((x_accel.^2)+(z_accel.^2)),y_accel);
%beta_dot = diff(beta)/delta_time;
%gamma = atan2(z_accel,sqrt((x_accel.^2)+(y_accel.^2)));
%gamma_dot = diff(gamma)/delta_time;

F = [1,-delta_time;0,1];
B = [delta_time;0];
Q = [var_angle,0;0,var_gyro_resid];
H = [1,0];
R = 0.3;
z = pitch;

x = [0;0];  %Initial angular position and gyro drift
P = [0,0;0,0]; %If initial position not certain, set diags to large number

out = [];

for i = 1:(length(pitch)-1)

    
end

xlswrite('excelout.xlsx',out)
