% Open the repository vehicle model without automatically running simulation.
projectRoot = fileparts(mfilename('fullpath'));
modelFolder = fullfile(projectRoot,'electrical','simulink','整车仿真模型');
addpath(modelFolder);
open_system(fullfile(modelFolder,'simulink14dof01.slx'));
