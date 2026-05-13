%% Model parameters %%

M = 0.07; % Mass = 70g
g = 9.81; % Gravity force
Drag = -0.1; % Aerodynamic drag gain / translational drag

L = 0.07; % Distance center-propeller = 7cm
a = L/sqrt(2); % Because of the 'x' configuration, since the motors lie on the diagonal

Ix = 2*10^(-4); % Moment of inertia. Parameter used inside the 6DOF block
Iy = 2*10^(-4); % Moment of inertia. Parameter used inside the 6DOF block
Iz = 4*10^(-4); % Moment of inertia. Parameter used inside the 6DOF block

Km = 10^(-6); % Propeller reaction torque which causes yaw
Kf = 10^(-5); % Converts motor speed into thrust

Hover_FeedForw_Coef = 245250; % Hover feedforward scaling coefficient (approximated as g/(4*Kf) )


%% Model Simulation %%

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