/*
 * $Id: gtpng_crc32.c 74 2010-02-22 11:42:26Z ahto.truu $
 *
 * Copyright 2008-2010 GuardTime AS
 *
 * This file is part of the GuardTime client SDK.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include "gtpng_crc32.h"

/* Table of CRCs of all 8-bit messages. */
static uint32_t crc_table[256];

/* Has the table been computed? */
static int crc_table_computed = 0;

/* Internal helper: computes the table for a fast CRC */
static void make_crc_table(void)
{
	uint32_t c;
	int n, k;
	for (n = 0; n < 256; n++) {
		c = (uint32_t) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

/* Internal helper: updates a running CRC with the bytes buf[0..len-1]. */
static uint32_t update_crc(uint32_t crc, const void *buf, size_t len)
{
	uint32_t c = crc;
	const unsigned char *p = buf;

	if (!crc_table_computed)
		make_crc_table();

	while (len > 0) {
		c = crc_table[(c ^ *p) & 0xff] ^ (c >> 8);
		--len;
		++p;
	}
	return c;
}

/**/

uint32_t GTPNG_crc32(const void *buf, size_t len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}
