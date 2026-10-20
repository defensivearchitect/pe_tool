/* check.h -- minimal test helpers shared by the test programs. */
#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include <stdio.h>

static int check_failures = 0;

/* Record a failed check and keep going, so one run reports every
   failure. The file and line come from the CHECK macro, so they point
   at the test, not at this header. */
static void check(bool ok, const char *expr, const char *file, int line) {
    if (!ok) {
        fprintf(stderr, "%s:%d: check failed: %s\n", file, line, expr);
        check_failures++;
    }
}

#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

/* Print a summary and return the exit status for main(). */
static int check_report(void) {
    if (check_failures != 0) {
        fprintf(stderr, "%d check(s) failed\n", check_failures);
        return 1;
    }
    printf("all tests passed\n");
    return 0;
}

#endif
