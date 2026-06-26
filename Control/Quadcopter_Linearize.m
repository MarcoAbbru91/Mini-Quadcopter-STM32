% Linearize quadcopter plant around hover

% Clean-up
clear;
clc;
close all;

% Load parameters
run("Quadcopter_Commands.m");
model = "Quadcopter_Model";
load_system("Quadcopter_Model");


% Linearization input/output points

% Plant inputs:
% port 1 = Drag
% port 2 = Gravity
% port 3 = Thrust
% port 4 = Roll
% port 5 = Pitch
% port 6 = Yaw
io(1) = linio("Quadcopter_Model/Thrust", 1,"input");   % Thrust
io(2) = linio("Quadcopter_Model/Roll",   1,"input");   % Roll
io(3) = linio("Quadcopter_Model/Pitch",  1,"input");   % Pitch
io(4) = linio("Quadcopter_Model/Yaw",    1,"input");   % Yaw

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
linsys = linearize(model,io,op);

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