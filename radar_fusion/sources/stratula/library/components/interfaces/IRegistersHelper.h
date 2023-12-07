
#define COMBINE_X(a, b, c) a##b##c
#define COMBINE(a, b, c)   COMBINE_X(a, b, c)


#ifdef AddrType
#undef AddrType
#endif

#ifdef RegType
#undef RegType
#endif

#ifdef IRegistersType
#undef IRegistersType
#endif

#ifdef BatchType
#undef BatchType
#endif
