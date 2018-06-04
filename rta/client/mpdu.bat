pushd	%HOME%\dynamic.rta
echo	started mpdu
masmx	-wk %RTA_BINARY%\..\..\target.rta\fponline\pdu pdu
symbol	< pdu.txo > pduwords.bat set
lstring	-b	pdu.txo pdu
call pduwords
slab	pdu pduimage %FPONLINE_WORDS%
popd

