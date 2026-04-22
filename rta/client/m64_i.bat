rem     set CL64=c:\Program Files (x86)\Microsoft visual Studio 10.0\VC\bin\amd64\cl.exe
rem     or similar
set     CL64=
CL64	/J /DDOS /DINTEL /Fe..\binary.rta\win64\i ..\win64.rel\argue.obj Ws2_64.Lib i.c
rem	C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\WS2_64.Lib
