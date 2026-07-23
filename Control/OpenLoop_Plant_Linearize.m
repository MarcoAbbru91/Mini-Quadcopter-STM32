% Linearize quadcopter OPEN-LOOP (!) plant around hover
% After obtaining the Transfer Functions for roll, pitch, yaw and
% altitude, and consequently their poles. Goal is to finally compute the
% gains for the respective PID controllers.

% Prior steps to be done on the Simulink model: make the plant open-loop,
% i.e. disconnect the roll, pitch, yaw, thrust connections from the
% controller (connect at most a constant block to them). 
% However, keep drag and gravity connected: Drag contributes the real pole 
% (the eventual (s+a) term) in G_z = k/(s(s+a)), and Gravity sets the hover 
% operating point (thrust trims against it)


% Clean-up
clear;
clc;
close all;

% Load parameters
run("Quadcopter_Commands.m");
model = "Quadcopter_Model";
load_system("Quadcopter_Model");


% Linearization input/output points
% 'linio' always references a block's output port (the signal leaving the block)

% Plant inputs:
% port 1 = Drag
% port 2 = Gravity
% port 3 = Thrust
% port 4 = Roll
% port 5 = Pitch
% port 6 = Yaw
io(1) = linio(sprintf('%s/Zero-Order\nHold9', model), 1, 'openinput');  % Thrust -> plant in3  (breaks the loop + injects) (sprintf is needed because of the 'new line character'
io(2) = linio('Quadcopter_Model/Constant2', 1, 'input'); % Roll  -> plant in4 (here the input to the plant is the constant block)
io(3) = linio('Quadcopter_Model/Constant',  1, 'input'); % Pitch -> plant in5 (here the input to the plant is the constant block)
io(4) = linio('Quadcopter_Model/Constant1', 1, 'input'); % Yaw   -> plant in6 (here the input to the plant is the constant block)

% Plant outputs:
% port 1 = Ve
% port 2 = Xe
% port 3 = rpy
% port 6 = wb
io(5) = linio("Quadcopter_Model/Quadcopter Plant", 1,"output");  % Ve
io(6) = linio("Quadcopter_Model/Quadcopter Plant", 2,"output");  % Xe
io(7) = linio("Quadcopter_Model/Quadcopter Plant", 3,"output");  % rpy
io(8) = linio("Quadcopter_Model/Quadcopter Plant", 6,"output");  % wb


% Get operating point at initial hover condition
op = operpoint(model);

% Linearize
opt = linearizeOptions('SampleTime', 0, 'RateConversionMethod', 'tustin');   % 0 = continuous/s-domain (with discretized plant I was getting two damped complex poles for the G(z) TF). 
                                                                             % "tustin" needed to avoid error "could not be linearized due to a discrete pole at z = 0 that is being converted to the continuous domain using the 'zoh' rate conversion method"
linsys = linearize(model, io, op, opt);

% Extract state-space matrices
[A,B,C,D] = ssdata(linsys);

% =========================================================
% Display dimensions
% ==========================================================

disp("State-Space matrix dimensions");

disp("Size of A:");
disp(size(A));

disp("Size of B:");
disp(size(B));

disp("Size of C:");
disp(size(C));

disp("Size of D:");
disp(size(D));


% =========================================================
% Eigenvalue analysis
% ==========================================================

eigA = eig(A);
disp(eigA);

figure;
plot(real(eigA), imag(eigA), 'x', 'LineWidth',2);
grid on;

xlabel('Real Axis');
ylabel('Imaginary Axis');

title('Eigenvalues of Linearized Quadcopter');


% =========================================================
% Convert to transfer functions
% ==========================================================

G = tf(linsys);


% =========================================================
% CHANNELS
%
% INPUTS:
%   1 = Thrust
%   2 = Roll
%   3 = Pitch
%   4 = Yaw
%
% OUTPUTS:
%
% Ve:
%   1 = Vx
%   2 = Vy
%   3 = Vz
%
% Xe:
%   4 = X
%   5 = Y
%   6 = Z
%
% rpy:
%   7 = Roll
%   8 = Pitch
%   9 = Yaw
%
% wb:
%   10 = p
%   11 = q
%   12 = r
%

% =========================================================
% Transfer functions
% =========================================================

% Roll dynamics (Roll output / Roll input)
G_roll = G(7,2);

% Pitch dynamics (Pitch output / Pitch input)
G_pitch = G(8,3);

% Yaw dynamics (Yaw output / Yaw input)
G_yaw = G(9,4);

% Altitude dynamics (Z output / Thrust input
G_z = G(6,1);


disp("Roll Transfer Function");
G_roll

disp("Pitch Transfer Function");
G_pitch

disp("Yaw Transfer Function");
G_yaw

disp("Altitude Transfer Function");
G_z

% =========================================================
% Step responses
% ==========================================================

figure;
step(G_roll);
grid on;
title('Roll Dynamics');

figure;
step(G_pitch);
grid on;
title('Pitch Dynamics');

figure;
step(G_yaw);
grid on;
title('Yaw Dynamics');

figure;
step(G_z);
grid on;
title('Altitude Dynamics');

% =========================================================
% Bode plots
% ==========================================================

figure;
bode(G_roll);
grid on;
title('Roll Bode Plot');

figure;
bode(G_pitch);
grid on;
title('Pitch Bode Plot');

figure;
bode(G_yaw);
grid on;
title('Yaw Bode Plot');

figure;
bode(G_z);
grid on;
title('Altitude Bode Plot');

% =========================================================
% Pole-zero maps
% ==========================================================

figure;
pzmap(G_roll);
grid on;
title('Roll Pole-Zero Map');

figure;
pzmap(G_pitch);
grid on;
title('Pitch Pole-Zero Map');

figure;
pzmap(G_yaw);
grid on;
title('Yaw Pole-Zero Map');

figure;
pzmap(G_z);
grid on;
title('Altitude Pole-Zero Map');