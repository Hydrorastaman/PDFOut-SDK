#define Version "v0.8.18"


[Setup]
AppId={{FF563549-0482-4EA0-B681-EC113E090AD2}}
AppName=PdfOut SDK
AppVerName=PdfOut SDK {#Version}
DefaultDirName={code:GetProgramFiles}\PdfOut SDK
DefaultGroupName=PdfOut SDK
OutputDir=installer
OutputBaseFilename=PdfOutSDKInstaller_{#Version}
AllowRootDirectory=false
UsePreviousAppDir=true
PrivilegesRequired=admin
UsePreviousGroup=false
DisableStartupPrompt=true
Compression=lzma
SolidCompression=yes
ChangesAssociations=yes
;WizardImageFile="Bitmaps\DlgBmp.bmp"
;WizardSmallImageFile="Bitmaps\BannrBmp.bmp"
RestartIfNeededByRun=no
;LicenseFile=License.rtf

[Files]
; pdfout lib
Source: "api\*";                     DestDir: "{app}\include\pdfout"; Flags: recursesubdirs
Source: "bin\dll\Win32\pdfout.dll";  DestDir: "{app}\lib\Win32";
Source: "bin\dll\Win32\pdfout.lib";  DestDir: "{app}\lib\Win32";
Source: "bin\dll\x64\pdfout.dll";    DestDir: "{app}\lib\x64";
Source: "bin\dll\x64\pdfout.lib";    DestDir: "{app}\lib\x64";

; emf2pdf lib
Source: "emf2pdf\api\*";             DestDir: "{app}\include\emf2pdf"; Flags: recursesubdirs
Source: "bin\dll\Win32\emf2pdf.dll"; DestDir: "{app}\lib\Win32";
Source: "bin\dll\Win32\emf2pdf.lib"; DestDir: "{app}\lib\Win32";
Source: "bin\dll\x64\emf2pdf.dll";   DestDir: "{app}\lib\x64";
Source: "bin\dll\x64\emf2pdf.lib";   DestDir: "{app}\lib\x64";

; samples
Source: "samples\*";                    DestDir: "{app}\samples"; Flags: recursesubdirs

[Icons]
Name: "{group}\{cm:UninstallProgram,PdfOut SDK}"; Filename: "{uninstallexe}"

[Run]
;

[UninstallRun]
;

[UninstallDelete]
Name: {app}; Type: filesandordirs

[Code]

function GetProgramFiles(Param: string): string;
begin
  if IsWin64 then Result := ExpandConstant('{pf64}')
    else Result := ExpandConstant('{pf32}')
end;
