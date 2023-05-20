#define	TEST1	(left >> scale_right) - right
#define	TEST2	centre / divisor * multiplier
#define	TEST3	centre >> scale_right << scale_left
#define	TEST4	xor_mask ^ TEST1 | or_mask
#define	TEST5	(TEST1) >> long_scale_right << long_scale_left
#define	TEST6	(TEST1) >> long_scale_right | (TEST2) << long_scale_left
#define	TEST7	((TEST1) >> long_scale_right) | ((TEST2) << long_scale_left)
#define	TEST8	short_left / divisor * multiplier
#define	TEST9	short_left / divisor * multiplier & and_short
#define	TEST10	short_left * long_multiplier / long_divisor
#define	TEST11	short_right * 12000000 / 3000000
#define	TEST12	centre / divisor * multiplier & and_u32
