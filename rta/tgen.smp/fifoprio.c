/**********************************************************
	this is a function to enable tweaking
	it's only executed at emulator start

	emulated core yields platform thread
	when emulated machine execution decides
	lowest priority at gaining platform thread
**********************************************************/

static int derive_fifo_priority()
{
   return sched_get_priority_min(SCHED_FIFO) + 1;
}

