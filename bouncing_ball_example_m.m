clear all
close all
clc

%% Create IDDATA
%
% To perform the identification we need to create iddata that will be used later.
% Our data will be created by a test model with g = 9.8 and k = 0.5

order = [1, 1, 2];
Ts = 1e-3;
parameters = [9.8; 0.75];
ics = [3; 0.2];

test_model = idnlhybrid(   ...
  'bouncing_ball_example_c', ... % Name of the source file of the model
  order,                     ... % The order of the model Ny, Nu, Nx
  parameters,                ... % The parameters for our model
  Ts,                        ... % Sampling time (integration step)
  'ics', ics,                ... % Initial Conditions
  'Path', '.',               ... % Position of the sources with respect to cwd
  'jumpLogic', 'C');             % The jump logic privileges the flow set


fprintf(1, 'REAL Model Initial state: % 2.4f, % 2.4f\n', ...
  test_model.InitialStates(3).Value, test_model.InitialStates(4).Value);
fprintf(1, 'REAL Model Parameters: % 2.4f, % 2.4f\n', ...
  test_model.Parameters(1).Value, test_model.Parameters(2).Value);

empty_input = zeros(int32(2/Ts), 1);
empty_iddata = iddata(empty_input, empty_input, Ts);

[y, ~, x] = sim(test_model, empty_iddata);
figure;
plot(y);

ident_data = iddata(x(:, 3), empty_input, Ts);

%% Creating identification model

id_ics = [2; -0.2];
id_params = [6; 0.6];

id_model = idnlhybrid(   ...
  'bouncing_ball_example_c', ... % Name of the source file of the model
  order,                     ... % The order of the model Ny, Nu, Nx
  id_params,                 ... % The parameters for our model
  Ts,                        ... % Sampling time (integration step)
  'ics', id_ics,             ... % Initial Conditions
  'Path', '.',               ... % Position of the sources with respect to cwd
  'jumpLogic', 'C');             % The jump logic privileges the flow set


fprintf(1, 'INIT Model Initial state: % 2.4f, % 2.4f\n', ...
  id_model.InitialStates(3).Value, id_model.InitialStates(4).Value);
fprintf(1, 'INIT Model Parameters: % 2.4f, % 2.4f\n', ...
  id_model.Parameters(1).Value, id_model.Parameters(2).Value);

% Setting limits for searching the result
id_model.Parameters(1).Minimum = 5;
id_model.Parameters(1).Maximum = 10;
id_model.Parameters(2).Minimum = 0.5;
id_model.Parameters(2).Maximum = 0.95;

% Please rember that we need to skip the first two states (time and steps)
id_model.InitialStates(3).Minimum = 1;
id_model.InitialStates(3).Maximum = 5;
id_model.InitialStates(3).Fixed = false;
id_model.InitialStates(4).Minimum = -0.5;
id_model.InitialStates(4).Maximum = 0.5;
id_model.InitialStates(4).Fixed = false;

%% Running identification

opts = nlgreyestOptions;
opts.Display = 'on';
res_model = pem(ident_data, id_model, opts);


%% Output data

fprintf(1, 'IDENT Model Initial state: % 2.4f, % 2.4f\n', ...
  res_model.InitialStates(3).Value, res_model.InitialStates(4).Value);
fprintf(1, 'IDENT Model Parameters: % 2.4f, % 2.4f\n', ...
  res_model.Parameters(1).Value, res_model.Parameters(2).Value);

[y2, ~, x] = sim(res_model, empty_iddata);
figure;
plot(y2);