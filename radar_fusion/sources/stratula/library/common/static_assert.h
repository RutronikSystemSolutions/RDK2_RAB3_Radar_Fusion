#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

#define STATIC_ASSERT(pred, msg)                         \
    {                                                    \
        static int STATIC_ASSERT_##msg[(pred) ? 1 : -1]; \
        (void)STATIC_ASSERT_##msg;                       \
    }


#endif  // STATIC_ASSERT_H