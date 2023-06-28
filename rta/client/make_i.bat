rem     set CL32=c:\Program Files (x86)\Microsoft visual Studio 10.0\VC\bin\amd32\cl.exe
rem     or similar
set     CL32=

CL32	/J /DDOS /DINTEL /Fe..\binary.rta\win32\i ..\emulator.rel\argue.obj Ws2_32.Lib i.c
rem	C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\WS2_32.Lib
