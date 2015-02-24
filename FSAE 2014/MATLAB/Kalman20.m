s = importdata('temp.txt');

var_angle = 0.001;
var_gyro_resid = 0.003;

x_accel = s(:,1);
y_accel = s(:,2);
z_accel = s(:,3);
pitch_v_gyro = s(:,4);

delta_time = 0.05;
pitch = atan2(-x_accel, sqrt(y_accel.^2 + z_accel.^2));
pitch_v_accel = diff(pitch)./delta_time;

F = [1,-delta_time;0,1];
B = [delta_time;0];
Q = [var_angle,0;0,var_gyro_resid];
H = [1,0];
R = 0.03;
z = pitch; %stupid

x = [0;0];
uncertainty = 10000000000;
P = uncertainty*eye(2);

out = [];

for i = 1:(length(x_accel)-1)
    
    %Prediction
    x = (F*x) + (B*pitch_v_gyro(i));
    %x = F*x + [pitch_v(i)*delta_time;0];

    P = (F*P*(F.')) + Q;
    
    %Update
    y = pitch(i) - (H*x);
    S = (H*P*(H.')) + R;
    K = (P*(H.'))/S;
    x = x + (K*y);
    P = (eye(2)-(K*H))*P;
    
    out = [out;x.']; %try to fix
    
end

xlswrite('exceloutPosition.xlsx',out);
    