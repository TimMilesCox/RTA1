rem     set     CL64=c:\Program Files\Microsoft Visual Studio 10.0\VC\bin\x86_amd64\cl.exe
rem     or similar
set     CL64=

call	..\util.rta\make64.bat

CL64	/J /DDOS /DINTEL /Fe..\binary.rta\win64\fp ..\win64.rel\argue.obj Ws2_64.Lib fp.c
CL64	/J /DDOS /DINTEL /Fe..\binary.rta\win64\i ..\win64.rel\argue.obj Ws2_64.Lib i.c
CL64	/J /DDOS /DINTEL /Fe..\binary.rta\win64\fponline ..\win64.rel\argue.obj Ws2_64.Lib fponline.c
CL64	/J /DDOS /DINTEL /Fe..\binary.rta\win64\oconsole Ws2_64.Lib oconsole.c

