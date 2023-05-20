
/******************************************************************************************
	compile -fleading-underscores masks kernel calls in linux
	so just unmask the few assembly [_]names we have
******************************************************************************************/


#define	leloup	_leloup
#define	base	_base
#define	b0p	_b0p
#define	psr	_psr
#define	b0_name	_b0_name
#define	_register __register
#define	memory	_memory
#define	apc	_apc
#define	apcz	_apcz
#define breakpoint _breakpoint
#define	register_set _register_set
#define	iselect	_iselect
#define	metric	_metric
#define indication _indication
#define	devices	_devices
