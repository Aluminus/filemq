/*-
 * Copyright (c) 2003 Allan Saddi <allan@saddi.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ALLAN SADDI AND HIS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL ALLAN SADDI OR HIS CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha1.h"
#include "sha256.h"
#include "sha384.h"
#include "sha512.h"

static uint8_t tv1[3] = {
  0x61, 0x62, 0x63
};

static uint8_t tv2[56] = {
  0x61, 0x62, 0x63, 0x64, 0x62, 0x63, 0x64, 0x65,
  0x63, 0x64, 0x65, 0x66, 0x64, 0x65, 0x66, 0x67,
  0x65, 0x66, 0x67, 0x68, 0x66, 0x67, 0x68, 0x69,
  0x67, 0x68, 0x69, 0x6a, 0x68, 0x69, 0x6a, 0x6b,
  0x69, 0x6a, 0x6b, 0x6c, 0x6a, 0x6b, 0x6c, 0x6d,
  0x6b, 0x6c, 0x6d, 0x6e, 0x6c, 0x6d, 0x6e, 0x6f,
  0x6d, 0x6e, 0x6f, 0x70, 0x6e, 0x6f, 0x70, 0x71
};

static uint8_t tv3[112] = {
  0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
  0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
  0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
  0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
  0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
  0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
  0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
  0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72,
  0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
  0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
  0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75
};

static uint8_t res1[3][SHA1_HASH_SIZE] = {
  { 0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a, 0xba, 0x3e, 0x25, 0x71,
    0x78, 0x50, 0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d },
  { 0x84, 0x98, 0x3e, 0x44, 0x1c, 0x3b, 0xd2, 0x6e, 0xba, 0xae, 0x4a, 0xa1,
    0xf9, 0x51, 0x29, 0xe5, 0xe5, 0x46, 0x70, 0xf1 },
  { 0x34, 0xaa, 0x97, 0x3c, 0xd4, 0xc4, 0xda, 0xa4, 0xf6, 0x1e, 0xeb, 0x2b,
    0xdb, 0xad, 0x27, 0x31, 0x65, 0x34, 0x01, 0x6f }
};

static uint8_t res256[3][SHA256_HASH_SIZE] = {
  { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde,
    0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad },
  { 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8, 0xe5, 0xc0, 0x26, 0x93,
    0x0c, 0x3e, 0x60, 0x39, 0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
    0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 },
  { 0xcd, 0xc7, 0x6e, 0x5c, 0x99, 0x14, 0xfb, 0x92, 0x81, 0xa1, 0xc7, 0xe2,
    0x84, 0xd7, 0x3e, 0x67, 0xf1, 0x80, 0x9a, 0x48, 0xa4, 0x97, 0x20, 0x0e,
    0x04, 0x6d, 0x39, 0xcc, 0xc7, 0x11, 0x2c, 0xd0 }
};

static uint8_t res384[3][SHA384_HASH_SIZE] = {
  { 0xcb, 0x00, 0x75, 0x3f, 0x45, 0xa3, 0x5e, 0x8b, 0xb5, 0xa0, 0x3d, 0x69,
    0x9a, 0xc6, 0x50, 0x07, 0x27, 0x2c, 0x32, 0xab, 0x0e, 0xde, 0xd1, 0x63,
    0x1a, 0x8b, 0x60, 0x5a, 0x43, 0xff, 0x5b, 0xed, 0x80, 0x86, 0x07, 0x2b,
    0xa1, 0xe7, 0xcc, 0x23, 0x58, 0xba, 0xec, 0xa1, 0x34, 0xc8, 0x25, 0xa7 },
  { 0x09, 0x33, 0x0c, 0x33, 0xf7, 0x11, 0x47, 0xe8, 0x3d, 0x19, 0x2f, 0xc7,
    0x82, 0xcd, 0x1b, 0x47, 0x53, 0x11, 0x1b, 0x17, 0x3b, 0x3b, 0x05, 0xd2,
    0x2f, 0xa0, 0x80, 0x86, 0xe3, 0xb0, 0xf7, 0x12, 0xfc, 0xc7, 0xc7, 0x1a,
    0x55, 0x7e, 0x2d, 0xb9, 0x66, 0xc3, 0xe9, 0xfa, 0x91, 0x74, 0x60, 0x39 },
  { 0x9d, 0x0e, 0x18, 0x09, 0x71, 0x64, 0x74, 0xcb, 0x08, 0x6e, 0x83, 0x4e,
    0x31, 0x0a, 0x4a, 0x1c, 0xed, 0x14, 0x9e, 0x9c, 0x00, 0xf2, 0x48, 0x52,
    0x79, 0x72, 0xce, 0xc5, 0x70, 0x4c, 0x2a, 0x5b, 0x07, 0xb8, 0xb3, 0xdc,
    0x38, 0xec, 0xc4, 0xeb, 0xae, 0x97, 0xdd, 0xd8, 0x7f, 0x3d, 0x89, 0x85 }
};

static uint8_t res512[3][SHA512_HASH_SIZE] = {
  { 0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba, 0xcc, 0x41, 0x73, 0x49,
    0xae, 0x20, 0x41, 0x31, 0x12, 0xe6, 0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2,
    0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a, 0x21, 0x92, 0x99, 0x2a,
    0x27, 0x4f, 0xc1, 0xa8, 0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd,
    0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 0xe8, 0x0e, 0x2a, 0x9a, 0xc9, 0x4f,
    0xa5, 0x4c, 0xa4, 0x9f },
  { 0x8e, 0x95, 0x9b, 0x75, 0xda, 0xe3, 0x13, 0xda, 0x8c, 0xf4, 0xf7, 0x28,
    0x14, 0xfc, 0x14, 0x3f, 0x8f, 0x77, 0x79, 0xc6, 0xeb, 0x9f, 0x7f, 0xa1,
    0x72, 0x99, 0xae, 0xad, 0xb6, 0x88, 0x90, 0x18, 0x50, 0x1d, 0x28, 0x9e,
    0x49, 0x00, 0xf7, 0xe4, 0x33, 0x1b, 0x99, 0xde, 0xc4, 0xb5, 0x43, 0x3a,
    0xc7, 0xd3, 0x29, 0xee, 0xb6, 0xdd, 0x26, 0x54, 0x5e, 0x96, 0xe5, 0x5b,
    0x87, 0x4b, 0xe9, 0x09 },
  { 0xe7, 0x18, 0x48, 0x3d, 0x0c, 0xe7, 0x69, 0x64, 0x4e, 0x2e, 0x42, 0xc7,
    0xbc, 0x15, 0xb4, 0x63, 0x8e, 0x1f, 0x98, 0xb1, 0x3b, 0x20, 0x44, 0x28,
    0x56, 0x32, 0xa8, 0x03, 0xaf, 0xa9, 0x73, 0xeb, 0xde, 0x0f, 0xf2, 0x44,
    0x87, 0x7e, 0xa6, 0x0a, 0x4c, 0xb0, 0x43, 0x2c, 0xe5, 0x77, 0xc3, 0x1b,
    0xeb, 0x00, 0x9c, 0x5c, 0x2c, 0x49, 0xaa, 0x2e, 0x4e, 0xad, 0xb2, 0x17,
    0xad, 0x8c, 0xc0, 0x9b }
};

int
main (int argc, char *argv[])
{
  union {
    SHA1_Context sha1;
    SHA256_Context sha256;
    SHA384_Context sha384;
    SHA512_Context sha512;
  } s;
  union {
    uint8_t sha1[SHA1_HASH_SIZE];
    uint8_t sha256[SHA256_HASH_SIZE];
    uint8_t sha384[SHA384_HASH_SIZE];
    uint8_t sha512[SHA512_HASH_SIZE];
  } h;
  uint8_t buf[1000];
  int i;
  int retval = 0;

  memset (buf, 0x61, sizeof (buf));

  printf ("SHA-1:\n");

  printf ("  Test vector #1: ");
  SHA1_Init (&s.sha1);
  SHA1_Update (&s.sha1, tv1, sizeof (tv1));
  SHA1_Final (&s.sha1, h.sha1);
  if (!memcmp (h.sha1, res1[0], sizeof (h.sha1)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #2: ");
  SHA1_Init (&s.sha1);
  SHA1_Update (&s.sha1, tv2, sizeof (tv2));
  SHA1_Final (&s.sha1, h.sha1);
  if (!memcmp (h.sha1, res1[1], sizeof (h.sha1)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #3: ");
  SHA1_Init (&s.sha1);
  for (i = 0; i < 1000; i++)
    SHA1_Update (&s.sha1, buf, sizeof (buf));
  SHA1_Final (&s.sha1, h.sha1);
  if (!memcmp (h.sha1, res1[2], sizeof (h.sha1)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("SHA-256:\n");

  printf ("  Test vector #1: ");
  SHA256_Init (&s.sha256);
  SHA256_Update (&s.sha256, tv1, sizeof (tv1));
  SHA256_Final (&s.sha256, h.sha256);
  if (!memcmp (h.sha256, res256[0], sizeof (h.sha256)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #2: ");
  SHA256_Init (&s.sha256);
  SHA256_Update (&s.sha256, tv2, sizeof (tv2));
  SHA256_Final (&s.sha256, h.sha256);
  if (!memcmp (h.sha256, res256[1], sizeof (h.sha256)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #3: ");
  SHA256_Init (&s.sha256);
  for (i = 0; i < 1000; i++)
    SHA256_Update (&s.sha256, buf, sizeof (buf));
  SHA256_Final (&s.sha256, h.sha256);
  if (!memcmp (h.sha256, res256[2], sizeof (h.sha256)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("SHA-384:\n");

  printf ("  Test vector #1: ");
  SHA384_Init (&s.sha384);
  SHA384_Update (&s.sha384, tv1, sizeof (tv1));
  SHA384_Final (&s.sha384, h.sha384);
  if (!memcmp (h.sha384, res384[0], sizeof (h.sha384)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #2: ");
  SHA384_Init (&s.sha384);
  SHA384_Update (&s.sha384, tv3, sizeof (tv3));
  SHA384_Final (&s.sha384, h.sha384);
  if (!memcmp (h.sha384, res384[1], sizeof (h.sha384)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #3: ");
  SHA384_Init (&s.sha384);
  for (i = 0; i < 1000; i++)
    SHA384_Update (&s.sha384, buf, sizeof (buf));
  SHA384_Final (&s.sha384, h.sha384);
  if (!memcmp (h.sha384, res384[2], sizeof (h.sha384)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("SHA-512:\n");

  printf ("  Test vector #1: ");
  SHA512_Init (&s.sha512);
  SHA512_Update (&s.sha512, tv1, sizeof (tv1));
  SHA512_Final (&s.sha512, h.sha512);
  if (!memcmp (h.sha512, res512[0], sizeof (h.sha512)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #2: ");
  SHA512_Init (&s.sha512);
  SHA512_Update (&s.sha512, tv3, sizeof (tv3));
  SHA512_Final (&s.sha512, h.sha512);
  if (!memcmp (h.sha512, res512[1], sizeof (h.sha512)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  printf ("  Test vector #3: ");
  SHA512_Init (&s.sha512);
  for (i = 0; i < 1000; i++)
    SHA512_Update (&s.sha512, buf, sizeof (buf));
  SHA512_Final (&s.sha512, h.sha512);
  if (!memcmp (h.sha512, res512[2], sizeof (h.sha512)))
    printf ("PASS\n");
  else {
    printf ("FAIL\n");
    retval = 1;
  }

  return retval;
}
