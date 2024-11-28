; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Gyermo"
#define MyAppVersion "24.11.28"
#define MyAppPublisher "Jeroen P. Broks"
#define MyAppURL "https://github.com/jcr6/Gyermo"
#define MyAppExeName "Gyermo.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{BE518AB0-2390-468E-A700-241F9CC38561}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=E:\Projects\Applications\Slyvina\Apps\PakHuis\License.txt
OutputDir=E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows_Install
OutputBaseFilename=Gyermo_Setup
SetupIconFile=E:\Projects\Applications\Slyvina\Apps\Gyermo\Gyermo\JCR.ico
Compression=lzma
SolidCompression=yes
ChangesAssociations = yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Registry]
Root: HKCR; Subkey: ".jcr";                             ValueData: "{#MyAppName}";          Flags: uninsdeletevalue; ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}";                     ValueData: "Program {#MyAppName}";  Flags: uninsdeletekey;   ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\DefaultIcon";             ValueData: "{app}\{#MyAppExeName},0";               ValueType: string;  ValueName: ""
Root: HKCR; Subkey: "{#MyAppName}\shell\open\command";  ValueData: """{app}\{#MyAppExeName}"" ""%1""";  ValueType: string;  ValueName: ""

[Files]
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\Gyermo.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\Gyermo.jcr"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\Gyermo.pdb"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libFLAC-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libjpeg-9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libmodplug-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libmpg123-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libogg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libopus-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libopusfile-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libpng16-16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libtiff-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libvorbis-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libvorbisfile-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\libwebp-7.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\SDL2_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\SDL2_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\zlib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Applications\Slyvina\Apps\Gyermo\Exe\Windows\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

