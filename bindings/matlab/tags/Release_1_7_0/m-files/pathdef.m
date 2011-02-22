function p = pathdef
%PATHDEF Search path defaults.
%   PATHDEF returns a string that can be used as input to MATLABPATH
%   in order to set the path.

  
%   Copyright 1984-2000 The MathWorks, Inc.
%   $Revision$ $Date$

% PATH DEFINED HERE -- Don't change this line.

p = [...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\mex-file;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\cvs;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\in-progess;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\in-progess\cvs;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\regression-test;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\regression-test\cvs;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\regression-test\reg-test-exe;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\utils;',...
'i:\dev\tango-cvs\cvs-sourceforge\matlab_binding\m-files\utils\cvs;',...
matlabroot,'\toolbox\matlab\general;',...
matlabroot,'\toolbox\matlab\ops;',...
matlabroot,'\toolbox\matlab\lang;',...
matlabroot,'\toolbox\matlab\elmat;',...
matlabroot,'\toolbox\matlab\elfun;',...
matlabroot,'\toolbox\matlab\specfun;',...
matlabroot,'\toolbox\matlab\matfun;',...
matlabroot,'\toolbox\matlab\datafun;',...
matlabroot,'\toolbox\matlab\audio;',...
matlabroot,'\toolbox\matlab\polyfun;',...
matlabroot,'\toolbox\matlab\funfun;',...
matlabroot,'\toolbox\matlab\sparfun;',...
matlabroot,'\toolbox\matlab\graph2d;',...
matlabroot,'\toolbox\matlab\graph3d;',...
matlabroot,'\toolbox\matlab\specgraph;',...
matlabroot,'\toolbox\matlab\graphics;',...
matlabroot,'\toolbox\matlab\uitools;',...
matlabroot,'\toolbox\matlab\strfun;',...
matlabroot,'\toolbox\matlab\iofun;',...
matlabroot,'\toolbox\matlab\timefun;',...
matlabroot,'\toolbox\matlab\datatypes;',...
matlabroot,'\toolbox\matlab\verctrl;',...
matlabroot,'\toolbox\matlab\winfun;',...
matlabroot,'\toolbox\matlab\winfun\comcli;',...
matlabroot,'\toolbox\matlab\demos;',...
matlabroot,'\toolbox\local;',...
matlabroot,'\toolbox\simulink\simulink;',...
matlabroot,'\toolbox\simulink\blocks;',...
matlabroot,'\toolbox\simulink\components;',...
matlabroot,'\toolbox\simulink\fixedandfloat;',...
matlabroot,'\toolbox\simulink\fixedandfloat\fxpdemos;',...
matlabroot,'\toolbox\simulink\fixedandfloat\obsolete;',...
matlabroot,'\toolbox\simulink\simdemos;',...
matlabroot,'\toolbox\simulink\simdemos\aerospace;',...
matlabroot,'\toolbox\simulink\simdemos\automotive;',...
matlabroot,'\toolbox\simulink\simdemos\simfeatures;',...
matlabroot,'\toolbox\simulink\simdemos\simgeneral;',...
matlabroot,'\toolbox\simulink\simdemos\simnew;',...
matlabroot,'\toolbox\simulink\dee;',...
matlabroot,'\toolbox\simulink\dastudio;',...
matlabroot,'\toolbox\stateflow\stateflow;',...
matlabroot,'\toolbox\stateflow\sfdemos;',...
matlabroot,'\toolbox\stateflow\coder;',...
matlabroot,'\toolbox\compiler;',...
matlabroot,'\toolbox\daq\daq;',...
matlabroot,'\toolbox\daq\daqguis;',...
matlabroot,'\toolbox\daq\daqdemos;',...
matlabroot,'\toolbox\optim;',...
matlabroot,'\toolbox\powersys\powersys;',...
matlabroot,'\toolbox\powersys\powerdemo;',...
matlabroot,'\toolbox\sb2sl;',...
matlabroot,'\toolbox\signal\signal;',...
matlabroot,'\toolbox\signal\sigtools;',...
matlabroot,'\toolbox\signal\sptoolgui;',...
matlabroot,'\toolbox\signal\sigdemos;',...
matlabroot,'\work;',...
     ...
];

p = [userpath,p];
