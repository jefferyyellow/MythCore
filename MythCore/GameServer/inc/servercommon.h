// ������rand()%x�ķ�ʽ�õ�0-(x-1)�����������ΪRAND_MAX��������x,���Իᵼ�²�����ȫ���
#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define MYTH_PERCENT_FLOAT	10000.0f
#define MYTH_PERCENT_INT	10000
#define MYTH_MILLIONTH		0.0001