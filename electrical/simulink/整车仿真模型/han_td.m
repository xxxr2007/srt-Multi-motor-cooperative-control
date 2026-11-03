function han_td_level2(block)
% Level-2 conversion of han_td. Parameters, in order: r, h, T.
% One scalar input; one two-element output vector of TD states.
setup(block);
end

function setup(block)
block.NumDialogPrms = 3;
block.DialogPrmsTunable = {'Tunable', 'Tunable', 'Tunable'};
block.NumInputPorts = 1;
block.NumOutputPorts = 1;
block.NumContStates = 0;
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;
block.InputPort(1).Dimensions = 1;
block.InputPort(1).DatatypeID = 0;
block.InputPort(1).Complexity = 'Real';
block.InputPort(1).SamplingMode = 'Sample';
block.InputPort(1).DirectFeedthrough = false;
block.OutputPort(1).Dimensions = 2;
block.OutputPort(1).DatatypeID = 0;
block.OutputPort(1).Complexity = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

% Preserve the inherited sample time; T is the original integration step.
block.SampleTimes = [-1 0];
block.RegBlockMethod('PostPropagationSetup', @postPropagationSetup);
block.RegBlockMethod('InitializeConditions', @initializeConditions);
block.RegBlockMethod('Outputs', @outputs);
block.RegBlockMethod('Update', @update);
end

function postPropagationSetup(block)
block.NumDworks = 1;
block.Dwork(1).Name = 'tdStates';
block.Dwork(1).Dimensions = 2;
block.Dwork(1).DatatypeID = 0;
block.Dwork(1).Complexity = 'Real';
block.Dwork(1).UsedAsDiscState = true;
end

function initializeConditions(block)
block.Dwork(1).Data = zeros(2, 1);
end

function outputs(block)
block.OutputPort(1).Data = block.Dwork(1).Data;
end

function update(block)
x = block.Dwork(1).Data;
u = block.InputPort(1).Data;
r = block.DialogPrm(1).Data;
h = block.DialogPrm(2).Data;
T = block.DialogPrm(3).Data;
xnext = zeros(2, 1);
xnext(1) = x(1) + T*x(2);
xnext(2) = x(2) + T*fst2(x, u, r, h);
block.Dwork(1).Data = xnext;
end

function f = fst2(x, u, r, h)
delta = r*h;
delta0 = delta*h;
y = x(1) - u + h*x(2);
a0 = sqrt(delta*delta + 8*r*abs(y));
if abs(y) <= delta0
    a = x(2) + y/h;
else
    a = x(2) + 0.5*(a0-delta)*sign(y);
end
if abs(a) <= delta
    f = -r*a/delta;
else
    f = -r*sign(a);
end
end
