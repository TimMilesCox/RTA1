#if	defined(__llvm__) || defined(__clang__)
#error	this script is for compiling environments with static link
#error	its use in PIC environments  will halve the performance of the target
#error	or fail to produce any target which works
#error	if you have forced an overwrite to anything in $RTA_BINARY reload from zip
#error	construct changed emulators with rta/altgen/tgen  binaries -> $ARTA
#error	this script may still be used with static linking gcc
#endif

