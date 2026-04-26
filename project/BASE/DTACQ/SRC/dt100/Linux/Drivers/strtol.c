/*
 * strtol.c — kernel-side strtol() shim.
 *
 * The 2.4 build of this driver carried a copy of the BSD libc strtol()
 * pulled in via userspace headers (<limits.h>, <ansidecl.h>).  Those
 * headers are not available — and not legal — in a 2.6 kernel module
 * build.  The kernel provides simple_strtol() with the same signature,
 * so this file now just delegates.
 *
 * Behavioural difference vs. libc strtol(): simple_strtol() does not
 * touch errno on overflow.  None of the in-tree callers inspect errno,
 * so this is a non-issue for this driver.
 */
#include <linux/kernel.h>
#include <linux/module.h>

long strtol(const char *nptr, char **endptr, int base)
{
	return simple_strtol(nptr, endptr, base);
}
