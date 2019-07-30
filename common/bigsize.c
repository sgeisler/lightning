#include <common/bigsize.h>

size_t bigsize_len(bigsize_t v)
{
	if (v < 0xfd) {
		return 1;
	} else if (v <= 0xffff) {
		return 3;
	} else if (v <= 0xffffffff) {
		return 5;
	} else {
		return 9;
	}
}

size_t bigsize_put(u8 buf[BIGSIZE_MAX_LEN], bigsize_t v)
{
	u8 *p = buf;

	if (v < 0xfd) {
		*(p++) = v;
	} else if (v <= 0xffff) {
		(*p++) = 0xfd;
		(*p++) = v >> 8;
		(*p++) = v;
	} else if (v <= 0xffffffff) {
		(*p++) = 0xfe;
		(*p++) = v >> 24;
		(*p++) = v >> 16;
		(*p++) = v >> 8;
		(*p++) = v;
	} else {
		(*p++) = 0xff;
		(*p++) = v >> 56;
		(*p++) = v >> 48;
		(*p++) = v >> 40;
		(*p++) = v >> 32;
		(*p++) = v >> 24;
		(*p++) = v >> 16;
		(*p++) = v >> 8;
		(*p++) = v;
	}
	return p - buf;
}

size_t bigsize_get(const u8 *p, size_t max, bigsize_t *val)
{
	if (max < 1)
		return 0;

	switch (*p) {
	case 0xfd:
		if (max < 3)
			return 0;
		*val = ((u64)p[1] << 8) + p[2];
		return 3;
	case 0xfe:
		if (max < 5)
			return 0;
		*val = ((u64)p[1] << 24) + ((u64)p[2] << 16)
			+ ((u64)p[3] << 8) + p[4];
		return 5;
	case 0xff:
		if (max < 9)
			return 0;
		*val = ((u64)p[1] << 56) + ((u64)p[2] << 48)
			+ ((u64)p[3] << 40) + ((u64)p[4] << 32)
			+ ((u64)p[5] << 24) + ((u64)p[6] << 16)
			+ ((u64)p[7] << 8) + p[8];
		return 9;
	default:
		*val = *p;
		return 1;
	}
}