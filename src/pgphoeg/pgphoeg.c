#include "postgres.h"
#include "fmgr.h"
#include <sys/stat.h>

//#DEFINE INVARIANT(inv) (*mem_#inv)

/* --> User Fibonacci implementation */
/* Naive implementation of fib with indirect recursion. */
int fib(int k) {
  int (*mem_fib) = (*mem_fib);/*INVARIANT(fib);*/
  
  if (k == 0 || k == 1) 
    return 1;
  
  return mem_fib(k - 1) + mem_fib(k - 2);
}
/* <-- User Fibonacci implementation */

int mem_fib(int k) {
  //"CALL ffib(k);"
}

CREATE OR REPLACE FUNCTION ffib(integer)
  RETURNS integer
  LANGUAGE SQL
AS $$
  SELECT COALESCE(
    (SELECT value FROM fib WHERE key=$1),
    fib_mem(
      $1,
      impl_c_fib($1)
    )
  );
$$;

/* Server-side Fibonacci implementation, should be automatically generated. */

PG_FUNCTION_V1(impl_c_fib);

Datum impl_c_fib(PG_FUNCTION_ARGS) {
  int32 key = PG_GETARG_INT32(0);

  PG_RETURN_INT32(fib(key));
}
