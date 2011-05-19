/*
 * Copyright (C) 2008 Sebastian Dröge <sebastian.droege@collabora.co.uk>.
 *
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1, a copy of which is found in LICENSE included in the
 * packaging of this file.
 */

#ifndef HACKS_H
#define HACKS_H

#include <gst/base/gstbitreader.h>

#if !GST_CHECK_VERSION(0,10,28)
#define GST_FLOW_CUSTOM_SUCCESS_1 101
#endif

#if !GST_CHECK_VERSION(0,10,29)
#define GST_TRACE_OBJECT(obj,...) GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE, obj, __VA_ARGS__)
#define GST_TRACE(...) GST_CAT_LEVEL_LOG (GST_CAT_DEFAULT, GST_LEVEL_TRACE, NULL, __VA_ARGS__)
#endif

#if !GST_CHECK_VERSION(0,10,30)

static inline void
gst_bit_reader_skip_unchecked (GstBitReader * reader, guint nbits)
{
  reader->bit += nbits;
  reader->byte += reader->bit / 8;
  reader->bit = reader->bit % 8;
}

#define __GST_BIT_READER_READ_BITS_UNCHECKED(bits) \
static inline guint##bits \
gst_bit_reader_peek_bits_uint##bits##_unchecked (const GstBitReader *reader, guint nbits) \
{ \
  guint##bits ret = 0; \
  const guint8 *data; \
  guint byte, bit; \
  \
  data = reader->data; \
  byte = reader->byte; \
  bit = reader->bit; \
  \
  while (nbits > 0) { \
    guint toread = MIN (nbits, 8 - bit); \
    \
    ret <<= toread; \
    ret |= (data[byte] & (0xff >> bit)) >> (8 - toread - bit); \
    \
    bit += toread; \
    if (bit >= 8) { \
      byte++; \
      bit = 0; \
    } \
    nbits -= toread; \
  } \
  \
  return ret; \
} \
\
static inline guint##bits \
gst_bit_reader_get_bits_uint##bits##_unchecked (GstBitReader *reader, guint nbits) \
{ \
  guint##bits ret; \
  \
  ret = gst_bit_reader_peek_bits_uint##bits##_unchecked (reader, nbits); \
  \
  gst_bit_reader_skip_unchecked (reader, nbits); \
  \
  return ret; \
}

__GST_BIT_READER_READ_BITS_UNCHECKED (8)
__GST_BIT_READER_READ_BITS_UNCHECKED (16)
__GST_BIT_READER_READ_BITS_UNCHECKED (32)
__GST_BIT_READER_READ_BITS_UNCHECKED (64)

#undef __GST_BIT_READER_READ_BITS_UNCHECKED

#endif

#endif
