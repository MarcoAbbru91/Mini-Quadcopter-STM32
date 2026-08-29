%% Model parameters %%

M = 0.07; % Mass = 70g
g = 9.81; % Gravity force
Drag = -0.1; % Aerodynamic drag gain / translational drag

L = 0.07; % Distance center-propeller = 7cm
a = L/sqrt(2); % Because of the 'x' configuration, since the motors lie on the diagonal

Ix = 6*10^(-5); % Moment of inertia. Parameter used inside the 6DOF block
Iy = 6*10^(-5); % Moment of inertia. Parameter used inside the 6DOF block
Iz = 1.2*10^(-4); % Moment of inertia. Parameter used inside the 6DOF block

Km = 3*10^(-9); % Propeller reaction torque which causes yaw (Prev 10^(-6))
Kf = 3*10^(-7); % Converts motor speed into thrust (Prev 10^(-5))

Hover_FeedForward = sqrt((M*g)/(4*Kf)); % Hover feedforward scaling term


%% Model Simulation %%

% Sample time / Scheduler (1ms)
Ts = 0.001;
% Simulation time
T = 50;
% Simulation steps
t = 0:Ts:T;   % 30 seconds simulation

% Desired altitude (in meters)
Throttle_Desired_Altitude = timeseries(single(-3 * ones(size(t))), t);  % 1 meter


% Desired Roll (in radians)
%Desired_Roll_angle = timeseries(single((30*pi/180) * ones(size(t))), t);  % rotate by 30°   (only for tuning: timeseries(single((30*pi/180) * ones(size(t))), t)

%Roll_pulse = single(zeros(size(t)));               % 0° everywhere by default
%Roll_pulse(t >= 12 & t <= 22) = single(20*pi/180);   % 20° only from t=12s to t=22s
%Desired_Roll_angle = timeseries(Roll_pulse, t);

Roll_pulse = single(zeros(size(t)));
% ramp up 12→14
idx_up = t >= 12 & t <= 14;
Roll_pulse(idx_up) = single((t(idx_up)-12)/(14-12)) * single(20*pi/180);
% hold 14→20
idx_hold = t > 14 & t <= 20;
Roll_pulse(idx_hold) = single(20*pi/180);
% ramp down 20→22
idx_down = t > 20 & t <= 22;
Roll_pulse(idx_down) = single((22-t(idx_down))/(22-20)) * single(20*pi/180);

Desired_Roll_angle = timeseries(Roll_pulse, t);


% Desired Pitch (in radians)
%Desired_Pitch_angle = timeseries(single(zeros(size(t))), t);  % no rotation

%Pitch_pulse = single(zeros(size(t)));               % 0° everywhere by default
%Pitch_pulse(t >= 12 & t <= 22) = single(20*pi/180);   % 20° only from t=12s to t=22s
%Desired_Pitch_angle = timeseries(Pitch_pulse, t);

Pitch_pulse = single(zeros(size(t)));
% ramp up 12→14
idx_up = t >= 12 & t <= 14;
Pitch_pulse(idx_up) = single((t(idx_up)-12)/(14-12)) * single(20*pi/180);
% hold 14→20
idx_hold = t > 14 & t <= 20;
Pitch_pulse(idx_hold) = single(20*pi/180);
% ramp down 20→22
idx_down = t > 20 & t <= 22;
Pitch_pulse(idx_down) = single((22-t(idx_down))/(22-20)) * single(20*pi/180);

Desired_Pitch_angle = timeseries(Pitch_pulse, t);


% Desired Yaw (in radians)
%timeseries(single((30*pi/180) * ones(size(t))), t);

%Yaw_pulse = single(zeros(size(t)));               % 0° everywhere by default
%Yaw_pulse(t >= 12 & t <= 22) = single(20*pi/180);   % 20° only from t=12s to t=22s
%Desired_Yaw_angle = timeseries(Yaw_pulse, t);

Yaw_pulse = single(zeros(size(t)));
% ramp up 12→14
idx_up = t >= 12 & t <= 14;
Yaw_pulse(idx_up) = single((t(idx_up)-12)/(14-12)) * single(20*pi/180);
% hold 14→20
idx_hold = t > 14 & t <= 20;
Yaw_pulse(idx_hold) = single(20*pi/180);
% ramp down 20→22
idx_down = t > 20 & t <= 22;
Yaw_pulse(idx_down) = single((22-t(idx_down))/(22-20)) * single(20*pi/180);

Desired_Yaw_angle = timeseries(Yaw_pulse, t);


% PID controller's values for Roll
P_Roll = 30; % 30 when tested singularly Proportional term
D_Roll = 15; % 15 Derivative term
% I term = 0

% PID controller's values for Pitch
P_Pitch = 30; % 30 Proportional term
D_Pitch = 15; % 15 Derivative term
% I term = 0

% PID controller's values for Yaw
P_Yaw = 220; % 220  180 Proportional term
D_Yaw = 70; % 70 Derivative term
% I term = 0

% PID controller's values for thrust/altitude
P_Thrust = 110; % Proportional term
D_Thrust = 105; % Derivative term
% I term = 0

% AHRS estimator coefficient
alfa_coeff = 0.98; % High alfa means more gyro reliance, but allows slower correction of long-term gyro drift from the accelerometer sensor

% Start simulation
simOut = sim("Quadcopter_Model.slx")


%% After simulation %%

Ve = simOut.Ve;
Xe = simOut.Xe;
rpy = simOut.rpy;
wb = simOut.wb;
% Extract time
t_out = rpy.Time;

% Extract signals
Ve_data  = Ve.Data;   % Velocity in the flat Earth reference frame
Xe_data  = Xe.Data;   % Position in the flat Earth reference frame
rpy_data = rpy.Data;  % Euler rotation angles for the x, y, z axes [roll, pitch, yaw]
wb_data  = wb.Data;   % Angular rates in body-fixed axes

% Split components
roll  = rpy_data(:,1);
pitch = rpy_data(:,2);
yaw   = rpy_data(:,3);

vx = Ve_data(:,1);
vy = Ve_data(:,2);
vz = Ve_data(:,3);

x = Xe_data(:,1);
y = Xe_data(:,2);
z = Xe_data(:,3);

p = wb_data(:,1);
q = wb_data(:,2);
r = wb_data(:,3);

%figure;
%plot(t_out, z, 'LineWidth',1.5); 
%hold on;
%plot(t_out, simOut.Pos_est.Data, '--', 'LineWidth',1.5);
%legend('real Pos Z','est Pos Z'); 
%grid on; 
%title('Real vs estimated altitude');
%grid on;

%figure;
%plot(t_out, vz, 'LineWidth',1.5); 
%hold on;
%plot(t_out, simOut.Vel_est.Data, '--', 'LineWidth',1.5);
%legend('real Vel Z','est Vel Z'); 
%grid on; 
%title('Real vs estimated velocity');
%grid on;

%figure;
%plot(t_out, z, 'LineWidth',1.5); 
%hold on;
%plot(t_out, simOut.measZpos.Data, 'LineWidth',1.5);
%legend('real Z','raw baro'); 
%grid on; 
%title('Real altitude vs raw barometer');

%a_true = gradient(vz, t_out);     % d(real vz)/dt
%figure; 
%plot(t_out, a_true, 'LineWidth',1.5); 
%hold on;
%plot(t_out, simOut.AccelZreal.Data, 'LineWidth',1.0);
%legend('true accel (d vz/dt)','Accel\_Z\_real'); 
%grid on;

%figure;
%plot(t_out, roll, 'LineWidth',1.5); 
%hold on;
%plot(t_out, simOut.Roll_est.Data, '--', 'LineWidth',1.5);
%legend('real Roll','est Roll'); 
%grid on; 
%title('Real vs estimated Roll');
%grid on;

figure;
plot(t_out, x, t_out, y, t_out, z);
legend('X', 'Y', 'Z');
title('Altitude');
xlabel('Time');
ylabel('Position [m]');
grid on;

figure;
plot(t_out, vx, t_out, vy, t_out, vz);
legend('Vx', 'Vy', 'Vz');
title('Velocity');
xlabel('Time');
ylabel('Velocity [m/s]');
grid on;

figure;
plot(t_out, roll, t_out, pitch, t_out, yaw);
legend('Roll','Pitch','Yaw');
title('Attitude (RPY)');
xlabel('Time');
ylabel('Angle [rad]');
grid on;

figure;
plot(t_out, p, t_out, q, t_out, r);
legend('Roll','Pitch','Yaw');
title('Angular rate');
xlabel('Time');
ylabel('Radians per second [rad/s]');
grid on;