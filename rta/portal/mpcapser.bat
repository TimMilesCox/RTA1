if	"%1"=="PCAP" (
	set BINARY=pcapserv
	set METHOD=/DPCAP
	set IPATH1=/I\usr\local\WpdPack_4_1_2\WpdPack\include
	set LIB1=\usr\local\WpdPack_4_1_2\WpdPack\Lib\wpcap.lib
)  else	(
	set BINARY=pktserv
	set METHOD=/UPCAP
	set IPATH1=/I\Users\Smaragd\win10pcap\Packet_dll
	set IPATH2=/I\usr\local\WpcapSrc_4_1_3\winpcap\wpcap\libpcap
	)

echo binary %BINARY%
echo interface %METHOD%
echo library %PCAPLIB%

cl /J	/DINTERFACES=6 		^
	/DX86_MSW		^
	%METHOD%		^
	%IPATH1%			^
	%IPATH2%			^
	/Fe%RTA_BINARY%s\%BINARY% ^
	..\win32.rel\argue.obj	^
	..\win32.rel\portal.obj	^
	%LIB1%			^
	pcapserv.c

