function han_eso_level2(block)
% Level-2 conversion of han_eso. Parameters, in order: d, bet, b, T.
% Input: [control; measurement]. Output: three observer states.
setup(block);
end

function setup(block)
block.NumDialogPrms = 4;
block.DialogPrmsTunable = {'Tunable', 'Tunable', 'Tunable', 'Tunable'};
block.NumInputPorts = 1;
block.NumOutputPorts = 1;
block.NumContStates = 0;
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

block.InputPort(1).Dimensions = 2;
block.InputPort(1).DatatypeID = 0;
block.InputPort(1).Complexity = 'Real';
block.InputPort(1).SamplingMode = 'Sample';
block.InputPort(1).DirectFeedthrough = false;
block.OutputPort(1).Dimensions = 3;
block.OutputPort(1).DatatypeID = 0;
block.OutputPort(1).Complexity = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

% Preserve the inherited sample time of the original han_eso.
% T remains the integration increment in the observer equations.
block.SampleTimes = [-1 0];
block.RegBlockMethod('PostPropagationSetup', @postPropagationSetup);
block.RegBlockMethod('InitializeConditions', @initializeConditions);
block.RegBlockMethod('Outputs', @outputs);
block.RegBlockMethod('Update', @update);
end

function postPropagationSetup(block)
block.NumDworks = 1;
block.Dwork(1).Name = 'observerStates';
block.Dwork(1).Dimensions = 3;
block.Dwork(1).DatatypeID = 0;
block.Dwork(1).Complexity = 'Real';
block.Dwork(1).UsedAsDiscState = true;
end

function initializeConditions(block)
block.Dwork(1).Data = zeros(3, 1);
end

function outputs(block)
block.OutputPort(1).Data = block.Dwork(1).Data;
end

function update(block)
x = block.Dwork(1).Data;
u = block.InputPort(1).Data;
d = block.DialogPrm(1).Data;
bet = block.DialogPrm(2).Data;
b = block.DialogPrm(3).Data;
T = block.DialogPrm(4).Data;

e = x(1) - u(2);
xnext = zeros(3, 1);
xnext(1) = x(1) + T * (x(2) - bet(1) * e);
xnext(2) = x(2) + T * (x(3) - bet(2) * fal(e, 0.5, d) + b * u(1));
xnext(3) = x(3) - T * bet(3) * fal(e, 0.25, d);
block.Dwork(1).Data = xnext;
end

function f = fal(e, a, d)
% Preserve the nonlinear function used by the original implementation.
if abs(e) < d
    f = ((a-3)*d^a/(d*cos(d)-3*sin(d)))*sin(e) ...
        + ((d^a*cos(d)-a*d^(a-1)*sin(d)) ...
        /(d^3*cos(d)-3*d^2*sin(d)))*e^3;
else
    f = abs(e)^a * sign(e);
end
end
