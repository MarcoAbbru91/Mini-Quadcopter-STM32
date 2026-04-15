% Sample time / Scheduler (5ms)
Ts = 0.005;
% Simulation time
t = 0:Ts:20;   % 20 seconds simulation

% Desired altitude (Throttle_Desired_Altitude)
Throttle_Desired_Altitude = timeseries(-15 * ones(size(t)), t);  % 15 meter
% Desired X position
Desired_X_Pos = timeseries(10 * ones(size(t)), t);  % hold at 10
% Desired Y position
Desired_Y_Pos = timeseries(5 * ones(size(t)), t);  % hold at 5
% Yaw command (in radians)
Yaw_Command = timeseries(zeros(size(t)), t);  % no rotation
simOut = sim("Quadcopter_Model.slx")


%% ===== After simulation =====

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

figure;
plot(t_out, vx, t_out, vy, t_out, vz);
legend('Velocity on three axis');
title('Velocity');
xlabel('Velocity [m/s]');
ylabel('Time');
grid on;

figure;
plot(t_out, x, t_out, y, t_out, z);
legend('Position on three axis');
title('Altitude');
xlabel('Position [m]');
ylabel('Time');
grid on;

figure;
plot(t_out, roll, t_out, pitch, t_out, yaw);
legend('Roll','Pitch','Yaw');
title('Attitude (RPY)');
xlabel('Time [s]');
ylabel('Angle');
grid on;

figure;
plot(t_out, p, t_out, q, t_out, r);
legend('Angular rates in body axes');
title('Angular rate');
xlabel('Radians per second [rad/s]');
ylabel('Time');
grid on;

%figure;
%subplot(3,1,1); plot(t_out, roll);  title('Roll');
%subplot(3,1,2); plot(t_out, pitch); title('Pitch');
%subplot(3,1,3); plot(t_out, yaw);   title('Yaw');


% t = [0 5 10 20];

% Throttle_Desired_Altitude = timeseries([0 1 1 1], t);  % step to 1m at t=5s
% Desired_X_Pos = timeseries([0 0 2 2], t);  % move to x=2 at t=10s
% Desired_Y_Pos = timeseries([0 0 -1 -1], t);  % move to y=-1 at t=10s
% Yaw_Command = timeseries([0 0 pi/4 pi/4], t);  % rotate 45° at t=10s