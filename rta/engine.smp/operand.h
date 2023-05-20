/******************************************************************
	this exploits the fact that the 24-bit RTA1 word
	is modeled in a platform 32-bit word

	a good read has high order octet [0] word.parity = 0

	these call R / W operations
	if the operations break they raise fault before return
	for stores you weren't doing anything more anyway

	parameters are like X = register or temp, Y = ea, Z = designator

	they are macroised to add the core identifier transparently

	you can have the same macros without xcore
	if you are not emulating multiple cores

******************************************************************/

#define	OPERAND(X,Y,Z)		if ((X = operand_read(Y, Z, xcore)) < 0) return;
#define OPERANDH(Z,Y)		if ((Z = memory_hold(Y, xcore)) == NULL) return;

#define BURST_READ2(X,Y)	if (burst_read2(X, Y, xcore) < 0) return;
#define BURST_READ4(X,Y)	if (burst_read4(X, Y, xcore) < 0) return;

#define	OPERAND_WRITE(X,Y,Z)	operand_write(X, Y, Z, xcore);

#define	BURST_WRITE2(X,Y)	burst_write2(X, Y, xcore);
#define	BURST_WRITE4(X,Y)	burst_write4(X, Y, xcore);
