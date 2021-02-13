#pragma once

/* Get size_t.  */
#include <stddef.h>
#include <stdint.h>

/* Attribute of a function whose result depends only on the arguments
   (possibly pointers) and global memory, and which has no side effects.  */
#ifndef _UC_ATTRIBUTE_PURE
# if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#  define _UC_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define _UC_ATTRIBUTE_PURE
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Check whether an UTF-8 string is well-formed.
   Return NULL if valid, or a pointer to the first invalid unit otherwise.  */
extern const uint8_t *
       u8_check (const uint8_t *s, size_t n)
       _UC_ATTRIBUTE_PURE;

#ifdef __cplusplus
}
#endif
