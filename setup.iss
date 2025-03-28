#define MyAppName "Taymer"
#define MyAppVersion "1.0"
#define MyAppPublisher "Your Company"
#define MyAppExeName "Taymer.exe"

[Setup]
AppId={{B7A28F91-9C9D-4C3E-9F93-8E5F15E9F8A1}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=installer\Output
OutputBaseFilename=TaymerSetup
Compression=lzma
SolidCompression=yes
;SetupIconFile=app.ico

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "build\release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\release\*.dll"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
;Source: "app.ico"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon 