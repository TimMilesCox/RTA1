
/******************************************************************************************
	compile -fleading-underscores masks kernel calls in linux
	so just unmask the few assembly [_]names we have
******************************************************************************************/


#define	leloup	_leloup

#define	memory	_memory
#define	core	_core

#define breakpoint _breakpoint
#define	touchpoint _touchpoint
#define	touchpoint2 _touchpoint2
#define	general_indication _general_indication

#define	devices	_devices

#define	ultra	_ultra
#define	ultra1	_ultra1
#define	ultra2	_ultra2
#define	ultra3	_ultra3
#define	ultra4	_ultra4
#define	ultra5	_ultra5
#define	ultra6	_ultra6
#define	ultra7	_ultra7

