function han_ctrl_level2(block)
% Level-2 conversion of han_ctrl. Parameters: aa, bet1, b, d.
% One five-element input vector; one scalar output.
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
block.InputPort(1).Dimensions = 5;
block.InputPort(1).DatatypeID = 0;
block.InputPort(1).Complexity = 'Real';
block.InputPort(1).SamplingMode = 'Sample';
block.InputPort(1).DirectFeedthrough = true;
block.OutputPort(1).Dimensions = 1;
block.OutputPort(1).DatatypeID = 0;
block.OutputPort(1).Complexity = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';
block.SampleTimes = [-1 0];
block.RegBlockMethod('Outputs', @outputs);
end

function outputs(block)
u = block.InputPort(1).Data;
aa = block.DialogPrm(1).Data;
bet1 = block.DialogPrm(2).Data;
b = block.DialogPrm(3).Data;
d = block.DialogPrm(4).Data;
e1 = u(1) - u(3);
e2 = u(2) - u(4);
u0 = bet1(1)*fal(e1, aa(1), d) + bet1(2)*fal(e2, aa(2), d);
y = u0 - u(5)/b;
% The original code overwrites its nonlinear output with this PD law.
% Retain that behavior during the interface migration.
y = e1*bet1(1) + e2*bet1(2);
block.OutputPort(1).Data = y;
end

function f = fal(e, a, d)
if abs(e) < d
    f = ((a-3)*d^a/(d*cos(d)-3*sin(d)))*sin(e) ...
        + ((d^a*cos(d)-a*d^(a-1)*sin(d)) ...
        /(d^3*cos(d)-3*d^2*sin(d)))*e^3;
else
    f = abs(e)^a * sign(e);
end
end

