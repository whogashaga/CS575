#define IN
#define OUT

kernel
void
Regression(	IN global const float *dX,
	   	IN global const float *dY,
		OUT global float *dSumx2,
		OUT global float *dSumx,
		OUT global float *dSumxy,
		OUT global float *dSumy )
{
	int gid = get_global_id( 0 );

	float x = dX[gid];
	float y = dY[gid];
	dSumx2[ gid ]   = dX[gid] * dX[gid];
	dSumx[ gid ]    = dX[gid];
	dSumxy[ gid ]   = dX[gid] * dY[gid];
	dSumy[ gid ]    = dY[gid];
}
