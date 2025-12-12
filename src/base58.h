/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * base58.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: base58.h 5884 2023-05-01 03:49:29Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2023-05-01 03:49:29  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef BASE58_H
#define BASE58_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int base58_encode(const unsigned char *in, size_t in_len, unsigned char *out, size_t *out_len);
extern int base58_decode(const unsigned char *in, size_t in_len, unsigned char *out, size_t *out_len);
extern size_t base58_encode_length(size_t in_len);
extern size_t base58_decode_length(size_t in_len);

#ifdef __cplusplus
}
#endif

#endif /* BASE58_H */
