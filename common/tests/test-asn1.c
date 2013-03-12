/*
 * Copyright (c) 2012 Red Hat Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the
 *       above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or
 *       other materials provided with the distribution.
 *     * The names of contributors to this software may not be
 *       used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Author: Stef Walter <stefw@gnome.org>
 */

#include "config.h"
#include "CuTest.h"

#include "asn1.h"
#include "debug.h"
#include "oid.h"
#include "x509.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct {
	p11_dict *asn1_defs;
} test;

static void
setup (CuTest *cu)
{
	test.asn1_defs = p11_asn1_defs_load ();
	CuAssertPtrNotNull (cu, test.asn1_defs);
}

static void
teardown (CuTest *cu)
{
	p11_dict_free (test.asn1_defs);
	memset (&test, 0, sizeof (test));
}

static void
test_tlv_length (CuTest *cu)
{
	struct {
		const char *der;
		size_t der_len;
		int expected;
	} tlv_lengths[] = {
		{ "\x01\x01\x00", 3, 3 },
		{ "\x01\x01\x00\x01\x02", 5, 3 },
		{ "\x01\x05\x00", 3, -1 },
		{ NULL }
	};

	int length;
	int i;

	setup (cu);

	for (i = 0; tlv_lengths[i].der != NULL; i++) {
		length = p11_asn1_tlv_length ((const unsigned char *)tlv_lengths[i].der, tlv_lengths[i].der_len);
		CuAssertIntEquals (cu, tlv_lengths[i].expected, length);
	}

	teardown (cu);
}

int
main (void)
{
	CuString *output = CuStringNew ();
	CuSuite* suite = CuSuiteNew ();
	int ret;

	putenv ("P11_KIT_STRICT=1");
	p11_debug_init ();

	SUITE_ADD_TEST (suite, test_tlv_length);

	CuSuiteRun (suite);
	CuSuiteSummary (suite, output);
	CuSuiteDetails (suite, output);
	printf ("%s\n", output->buffer);
	ret = suite->failCount;
	CuSuiteDelete (suite);
	CuStringDelete (output);

	return ret;
}