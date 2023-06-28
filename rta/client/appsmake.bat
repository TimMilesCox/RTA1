rem     set     CL32=c:\Program Files\Microsoft Visual Studio 10.0\VC\bin\x86_amd32\cl.exe
rem     or similar
set     CL32=

call	..\util.rta\make.bat

CL32	/J /DDOS /DINTEL /Fe..\binary.rta\win32\fp ..\win32.rel\argue.obj Ws2_32.Lib fp.c
CL32	/J /DDOS /DINTEL /Fe..\binary.rta\win32\i ..\win32.rel\argue.obj Ws2_32.Lib i.c
CL32	/J /DDOS /DINTEL /Fe..\binary.rta\win32\fponline ..\win32.rel\argue.obj Ws2_32.Lib fponline.c
CL32	/J /DDOS /DINTEL /Fe..\binary.rta\win32\oconsole Ws2_32.Lib oconsole.c


