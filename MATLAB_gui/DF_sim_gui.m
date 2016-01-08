function varargout = DF_sim_gui(varargin)
% DF_SIM_GUI M-file for DF_sim_gui.fig
%      DF_SIM_GUI, by itself, creates a new DF_SIM_GUI or raises the existing
%      singleton*.
%
%      H = DF_SIM_GUI returns the handle to a new DF_SIM_GUI or the handle to
%      the existing singleton*.
%
%      DF_SIM_GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in DF_SIM_GUI.M with the given input arguments.
%
%      DF_SIM_GUI('Property','Value',...) creates a new DF_SIM_GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before DF_sim_gui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to DF_sim_gui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help DF_sim_gui

% Last Modified by GUIDE v2.5 21-Jan-2014 12:17:42

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @DF_sim_gui_OpeningFcn, ...
                   'gui_OutputFcn',  @DF_sim_gui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
        gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before DF_sim_gui is made visible.
function DF_sim_gui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to DF_sim_gui (see VARARGIN)

%Initially run DF simulation with LOB = 45 degrees
[avg1, avg2, avg3, avg4, lobEstimate] = DFsim_fcn(90,get(handles.iq_flip_button,'Value'),1);

%Set average labels
set(handles.avg1,'String',avg1);
set(handles.avg2,'String',avg2);
set(handles.avg3,'String',avg3);
%set(handles.avg4,'String',avg4);

%Set lob estimate
set(handles.lobEst,'String',lobEstimate);


%Format graph
%set(handles.axes1,'Ylim',[-5000 5000]);
set(handles.axes1,'XGrid','on');
set(handles.axes1,'YGrid','on');
set(handles.lobValue,'String',get(handles.slider1,'Value'));
set(handles.axes1,'Ylim',[0, 10000]);
set(handles.axes1,'Xlim',[0, 720]);

yl=ylabel('Amplitude');
set(yl,'Rotation',90);
set(yl,'FontSize',12);
xl=xlabel('Samples');
set(xl,'FontSize',12);
graph_title = title('FM Demodulated Output of DF Antenna');
set(graph_title,'FontSize',20);

% Choose default command line output for DF_sim_gui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes DF_sim_gui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = DF_sim_gui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on slider movement.
function slider1_Callback(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

%Clear graph
cla(handles.axes1);
%Rerun sim with new LOB value
[avg1, avg2, avg3, avg4, lobEstimate] = DFsim_fcn(get(handles.slider1,'Value'),get(handles.iq_flip_button,'Value'),1);

%Format graph
set(handles.axes1,'XGrid','on');
set(handles.axes1,'YGrid','on');
set(handles.lobValue,'String',get(handles.slider1,'Value'));
set(handles.axes1,'Ylim',[0, 10000]);
set(handles.axes1,'Xlim',[0, 720]);

%Set average labels
set(handles.avg1,'String',avg1);
set(handles.avg2,'String',avg2);
set(handles.avg3,'String',avg3);
%set(handles.avg4,'String',avg4);

%Set lob estimate
set(handles.lobEst,'String',lobEstimate);

% --- Executes during object creation, after setting all properties.
function slider1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on key press with focus on slider1 and none of its controls.
function slider1_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  structure with the following fields (see UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in iq_flip_button.
function iq_flip_button_Callback(hObject, eventdata, handles)
%Clear graph
cla(handles.axes1);

%Rereun sim with new IQ flip value
[avg1, avg2, avg3, avg4, lobEstimate] = DFsim_fcn(get(handles.slider1,'Value'),get(handles.iq_flip_button,'Value'),1);
%Set average labels
set(handles.avg1,'String',avg1);
set(handles.avg2,'String',avg2);
set(handles.avg3,'String',avg3);
%set(handles.avg4,'String',avg4);

%Set lob estimate
set(handles.lobEst,'String',lobEstimate);

% hObject    handle to iq_flip_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of iq_flip_button

% --- Executes during object creation, after setting all properties.
function axes1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes1
