#include "kestring.h"

uint32_t kestrlen(const char *str)
{
	uint32_t len = 0;
	while(*str) {
		len++;
		str++;
	}
	return len;
}

void kememcpy(void *dst, const void *src, const uint32_t count)
{
	/*byte access*/
	uint8_t *dst_byte = dst;
	const uint8_t *src_byte = src;
	uint32_t i;

	for(i = 0; i < count; i++) {
		*dst_byte = *src_byte;
		dst_byte++;
		src_byte++;
	}
}

void kelmemcpy(void *dst, const void *src, const uint32_t count)
{
	/*byte access*/
	uint8_t *dst_byte = dst;
	const uint8_t *src_byte = src;
	int i, j;

	for(i = count - 1, j = 0; i >= 0; i--, j++) {
		*(dst_byte + j) = *(src_byte + i);
	}
}

uint8_t kememcmp(const void *dst, const void *src, const uint32_t count)
{
	const uint8_t *dst_byte = dst;
	const uint8_t *src_byte = src;
	uint32_t i;
	for(i = 0; i < count; i++) {
		if(dst_byte[i] != src_byte[i])
			return 1;
	}
	return 0;
}
