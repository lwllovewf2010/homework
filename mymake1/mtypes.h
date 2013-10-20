#ifndef GORMAKE_MTYPES_H_
#define GORMAKE_MTYPES_H_

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;

typedef     char                    s8;
typedef     short int               s16;
typedef     int                     s32;
typedef     long long int           s64;

#define ARRAY_SIZE(x)     (sizeof(x) / sizeof((x)[0]))

#endif // GORMAKE_MTYPES_H_


