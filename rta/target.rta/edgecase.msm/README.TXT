./sysimage.rom is generated to show the same application group

can be dynamically loaded
		r load /util/gate

and in an NVRAM chain
		r gate

normally root NVRAM chain ../initial.app/sysimage.rom is run

then this app is dynamic load only

this NVRAM chain is

	initial.app -> simple.app -> clanger.app -> hello.app -> edgecase.msm/gate

