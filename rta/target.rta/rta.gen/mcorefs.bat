masmx -klne minimald
if not exist minimald.txo ( exit )
lstring -b minimald.txo minimald.seg
4kformat -x < coreload corefs.txo -K 4
4kformat < coreload corefs.abs -K 4

