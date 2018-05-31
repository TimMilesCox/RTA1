extern void instruction_display(word *pc, int howmany, int linefeed);

#ifdef	EDGE
#error	target images do not set PSR++32768
extern void b4double(int left, int right);
extern void q4double();
extern void q4readout();
extern void stacktop4();
extern void stored4(int ea);
#endif
