/******************************************************************
	this exploits the fact that the 24-bit RTA1 word
	is modeled in a platform 32-bit word

	a good read has high order octet [0] word.parity = 0
        it's hoped the generated code evaluates value -> X
	without re-reading it
******************************************************************/

#define	OPERAND(X)	if ((X = operand_read(ea, designator)) < 0) return;

#define	OPERAND7(X)	if ((X = operand_read(ea, 7)) < 0) return;
#define OPERANDL(Y)	if ((Y = burst_read2(ea)) < 0) return;
#define OPERANDQ(Y)	if ((Y = burst_read4(ea)) < 0) return;
