/* general purpose temp variables */
VECTOR vecTemp;
ANGLE  angTemp;
COLOR  colTemp;
var    vTemp;
int    iTemp;
char   cTemp;
float  fTemp;
long   lTemp;

void vec_ang(ANGLE* pang)
{
	pang->pan = ang(pang->pan);
	pang->tilt = ang(pang->tilt);
	pang->roll = ang(pang->roll);
}

void vec_hys(VECTOR* pvec, var vHys)
{
	if (abs(pvec->x) < vHys) pvec->x = 0;
	if (abs(pvec->y) < vHys) pvec->y = 0;
	if (abs(pvec->z) < vHys) pvec->z = 0;
}

void vec_clamp(VECTOR* pvec, var vMinLim, var vMaxLim)
{
	clamp (pvec->x, vMinLim, vMaxLim);	
	clamp (pvec->y, vMinLim, vMaxLim);	
	clamp (pvec->z, vMinLim, vMaxLim);	
}

void vec_cross(VECTOR* pvecN, VECTOR* pvec1, VECTOR* pvec2)
{
	pvecN->x = pvec1->y * pvec2->z - pvec1->z * pvec2->y;
	pvecN->y = pvec1->z * pvec2->x - pvec1->x * pvec2->z;
	pvecN->z = pvec1->x * pvec2->y - pvec1->y * pvec2->x;	
}
