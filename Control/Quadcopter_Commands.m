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

% Sample time / Scheduler (5ms)
Ts = 0.005;
% Simulation time
t = 0:Ts:30;   % 30 seconds simulation

% Desired altitude (Throttle_Desired_Altitude)
Throttle_Desired_Altitude = timeseries(-1 * ones(size(t)), t);  % 1 meter
% Desired X position
Desired_Roll = timeseries(zeros(size(t)), t);  % hold at 10   (only for tuning: timeseries((5*pi/180) * ones(size(t)), t))
% Desired Y position
Desired_Pitch = timeseries(zeros(size(t)), t);  % hold at 5
% Yaw command (in radians)
Desired_Yaw = timeseries(zeros(size(t)), t);  % no rotation
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

figure;
plot(t_out, vx, t_out, vy, t_out, vz);
legend('Vx', 'Vy', 'Vz');
title('Velocity');
xlabel('Time');
ylabel('Velocity [m/s]');
grid on;

figure;
plot(t_out, x, t_out, y, t_out, z);
legend('X', 'Y', 'Z');
title('Altitude');
xlabel('Time');
ylabel('Position [m]');
grid on;

figure;
plot(t_out, roll, t_out, pitch, t_out, yaw);
legend('Roll','Pitch','Yaw');
title('Attitude (RPY)');
xlabel('Time');
ylabel('Angle');
grid on;

figure;
plot(t_out, p, t_out, q, t_out, r);
legend('Roll','Pitch','Yaw');
title('Angular rate');
xlabel('Time');
ylabel('Radians per second [rad/s]');
grid on;