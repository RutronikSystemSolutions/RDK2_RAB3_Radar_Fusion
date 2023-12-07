#ifndef ERRORS_H
#define ERRORS_H 1

#include <universal/error_definitions.h>


/// Strata return type, one of the codes defined in universal/error_definitions.h
typedef int sr_t;


//Evaluate the return value of a function call and return it, if it is not 0
#define RETURN_ON_ERROR(expression)  \
    {                                \
        const sr_t ret = expression; \
        if (ret)                     \
        {                            \
            return ret;              \
        }                            \
    }

//Evaluate the return value of a function call and return it after a cleanup, if it is not 0
#define CLEAN_RETURN_ON_ERROR(expression, cleanup) \
    {                                              \
        const sr_t ret = expression;               \
        if (ret)                                   \
        {                                          \
            cleanup;                               \
            return ret;                            \
        }                                          \
    }

#endif /* ERRORS_H */
