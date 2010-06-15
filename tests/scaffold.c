/* Test facility scaffolding.

   Copyright (C) 2009 Free Software Foundation, Inc.

   This file is part of the Decimal Floating Point C Library.

   Author(s): Ryan S. Arnold <rsa@us.ibm.com>

   The Decimal Floating Point C Library is free software; you can
   redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License version 2.1.

   The Decimal Floating Point C Library is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
   the GNU Lesser General Public License version 2.1 for more details.

   You should have received a copy of the GNU Lesser General Public
   License version 2.1 along with the Decimal Floating Point C Library;
   if not, write to the Free Software Foundation, Inc., 59 Temple Place,
   Suite 330, Boston, MA 02111-1307 USA.

   Please see libdfp/COPYING.txt for more information.  */


#include <stdio.h>
#include "decode.h"

static int fail = 0;

/* String compare macros  */
#ifndef _SC
#include <string.h>
#include <stdarg.h>
#include <limits.h> /* For CHAR_MAX.  */

/* Generally don't invoke this directly unless you already have __LINE__ set in
 * a variable and want _SC_P to output it.  The name stands for Compare with
 * Position.  */
#define _SC_P(f,l,x,y) do { \
  if(strcmp(x,y)) { \
    fprintf(stderr, "Error: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", x,y,f,l); \
    ++fail; \
  } else { \
    fprintf(stdout, "Success: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", x,y,f,l); \
  } \
} while (0)

#define _SC(x,y) _SC_P (__FILE__,__LINE__,x,y)

#endif /* _SC  */

#ifdef _WANT_PC
static char buf[CHAR_MAX];
#ifndef _PC

/* _PC == Printf_dfp Compare with Position  */
#define _PC_P(f,l,x,y,args...) do { \
  memset(buf,'\0',CHAR_MAX); \
  /* Invokes printf dfp.  */  \
  sprintf(buf, y, ##args); \
  _SC_P(f,l,x,buf); \
} while (0)

/* _PC == Printf_dfp Compare
 *
 * Variadic macro used to compare a printf invocation with an expected result.
 *
 * X: Expected String
 * Y: Format String
 * ...: Argument list
 *
 * Use it like you'd use printf, except you include an 'expected result' string
 * to precede everything.
 *
 * e.g.
 *   _PC("0.000033333","%DDf.\n", (_Decimal128) 0.00033333DL);
 *
 */
#define _PC(x,y,...) _PC_P (__FILE__,__LINE__,x,y,__VA_ARGS__)
#endif /* _PC  */
#endif /* _WANT_PC  */

#ifdef _WANT_VC
static char bufx[CHAR_MAX];
static char bufy[CHAR_MAX];
#ifndef _VC
/* _VC_P == Value Compare with Position  */
#define _VC_P(f,l,x,y,fmt) do { \
  memset(bufx,'\0',CHAR_MAX); \
  memset(bufy,'\0',CHAR_MAX); \
  /* Invokes printf dfp.  */  \
  sprintf(bufx, fmt, x); \
  sprintf(bufy, fmt, y); \
  if(x!=y) { \
    fprintf(stderr, "Error: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", bufx,bufy,f,l); \
    ++fail; \
  } else { \
    fprintf(stdout, "Success: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", bufx,bufy,f,l); \
  } \
} while (0)

/* _VC == Value Compare
 *
 * Macro used to compare the result of an operation against an expected result.
 * X: Expected Value
 * Y: Actual Value
 */
#define _VC(x,y,fmt) _VC_P (__FILE__,__LINE__,x,y,fmt)
#endif /* _VC  */
#endif /* _WANT_VC  */

#ifdef _WANT_QC
#include <math.h> /* To pick up __dfp_compatible().  */

/* Provide a polymorphic quantize() function.  */
#define quantize(x) \
  (!__dfp_compatible(x)							      \
    ? DEC_NAN								      \
    : (sizeof (x) == sizeof (_Decimal128)				      \
      ? quantized128(x)							      \
      : (sizeof (x) == sizeof (_Decimal64)				      \
	? quantized64(x)						      \
	: quantized32(x)))						      \
  )

#define _DECIMAL (type, varname)					      \
  do {									      \
       type varname;							      \
  }while (0)

static char bufx[CHAR_MAX];
static char bufy[CHAR_MAX];
#ifndef _QC
/* _QC_P == Quantize Compare with Position  */
#define _QC_P(f,l,x,y,fmt,type) do { \
  memset(bufx,'\0',CHAR_MAX); \
  memset(bufy,'\0',CHAR_MAX); \
  /* Invokes printf dfp.  */  \
  sprintf(bufx, fmt, x); \
  sprintf(bufy, fmt, y); \
  if(x!=y) { \
    fprintf(stderr, "Error: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", bufx,bufy,f,l); \
    ++fail; \
  } else { \
    fprintf(stdout, "Success: Expected: \"%s\" - Result: \"%s\" in: %s:%d.\n", bufx,bufy,f,l); \
  } \
} while (0)

/* _QC == Quantize Compare
 *
 * Macro used to compare the result of an operation against an expected result.
 * X: Expected Value
 * Y: Actual Value
 */
#define _QC(x,y,fmt) _QC_P (__FILE__,__LINE__,x,y,fmt)
#endif /* _QC  */
#endif /* _WANT_QC  */



#ifdef _WANT_DC
static char dbuf[CHAR_MAX];
#ifndef _DC
/* _DC == Decoded[32|64|128] Compare
 */

/* Pick up the decoded[32|64|128] prototypes.  */
#include "decode.h"

/* _DC_P == decoded[32|64|128] Compare with Position.  Use this if the position
 * is pre-determined.  Don't call this on Non-_Decimal values.  The outcome is
 * undefined.  */
#define _DC_P(f,l,x,y) do { \
  memset(dbuf,'\0',CHAR_MAX); \
  /* Invoke the correct decoded{32|64|128]() based on arg size.  */ \
  (sizeof (y) == sizeof (_Decimal128)? decoded128(y,&dbuf[0]): \
    (sizeof (y) == sizeof (_Decimal64)? decoded64(y,&dbuf[0]): \
       decoded32(y,&dbuf[0]))); \
  _SC_P(f,l,x,dbuf); \
} while (0)

/* _DC == decoded[32|64|128] Compare
 *
 * Macro used to compare a decoded[32|64|128]() invocation with an
 * expected result.
 *
 * X: Expected decoded[32|64|128] Output String
 * Y: _Decimal[32|64|128] Argument
 *
 * It is like decoded[32|64|128], except you include an 'expected
 * result' string to precede everything and you don't need to define a buffer.
 *
 * e.g.
 *   _DC("+0,000,000,000,000,000,000,000,000,000,000,100E-2", (_Decimal128) 1.00DL);
 *
 */
#define _DC(x,y) _DC_P (__FILE__,__LINE__,x,y)
#endif /* _DC  */
#endif /* _WANT_DC  */


#ifndef _REPORT
/* Don't print anything if there are no failures.  */
#define _REPORT() do { \
    if(fail) { \
      fprintf(stderr, "Found %d failures.\n", fail); \
    } \
} while (0)

#endif /* _REPORT  */

