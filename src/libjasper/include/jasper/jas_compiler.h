#ifndef JAS_COMPILER_H
#define JAS_COMPILER_H

#ifdef _MSC_VER
#ifndef __cplusplus
#undef inline
#define inline __inline
#endif
#endif

#ifdef __GNUC__
#define JAS_ATTRIBUTE_CONST __attribute__((const))
#define JAS_ATTRIBUTE_PURE __attribute__((pure))
#define JAS_FORCE_INLINE inline __attribute__((always_inline))
#define JAS_UNREACHABLE() __builtin_unreachable()
#define JAS_LIKELY(x) __builtin_expect (!!(x), 1)
#define JAS_UNLIKELY(x) __builtin_expect (!!(x), 0)
#else
#define JAS_ATTRIBUTE_CONST
#define JAS_ATTRIBUTE_PURE
#define JAS_FORCE_INLINE inline
#define JAS_UNREACHABLE()
#define JAS_LIKELY(x) (x)
#define JAS_UNLIKELY(x) (x)
#endif

#ifdef __clang__
#define JAS_ATTRIBUTE_DISABLE_USAN \
  __attribute__((no_sanitize("undefined")))
#elif defined(__GNUC__) && __GNUC__ >= 6
#define JAS_ATTRIBUTE_DISABLE_USAN \
  __attribute__((no_sanitize_undefined))
#else
#define JAS_ATTRIBUTE_DISABLE_USAN
#endif

#endif
