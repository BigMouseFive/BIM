; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "BIM系统"
#define MyAppVersion "V1.0-M"
#define MyAppPublisher "EchizenRyoma"
#define MyAppExeName "BIM.exe"

[Setup]                              
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。   
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{DA254EE4-CE65-4385-BDA4-816485DA3124}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}  
DisableProgramGroupPage=yes
DisableDirPage=yes
OutputDir=.\
OutputBaseFilename=BIM系统V1.0
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkablealone;   

[Files]
Source: "..\exec\BIM.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\exec\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\vcredist_x64.2013.exe"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\vcredist_x64.2013.exe"; Description: "vs2013运行库"; Flags: nowait postinstall skipifsilent

