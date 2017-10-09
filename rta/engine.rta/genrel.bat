
echo	logic sections generated with executable space %RTA_MBANKS% banks

cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\alu.obj alu.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\fpu.obj fpu.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\ii.obj ii.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\memory.obj memory.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\rta.obj rta.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\rw.obj rw.c
cl /J /c /DMBANKS=%RTA_MBANKS% /DX86_MSW /Fo..\win32.rel\sr.obj sr.c

