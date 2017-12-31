// 不能用rand()%x的方式得到0-(x-1)的随机数，因为RAND_MAX不能整除x,所以会导致不能完全随机
#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define MYTH_PERCENT_FLOAT	10000.0f
#define MYTH_PERCENT_INT	10000
#define MYTH_MILLIONTH		0.0001