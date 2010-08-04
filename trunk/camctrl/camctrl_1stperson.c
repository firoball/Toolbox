/*
 *******************************************************************************
 * camctrl_1stperson.c
 * Creation date: 25.06.2009
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2008-11-16 20:30:40 +0100 (So, 16 Nov 2008) $
 * $Revision: 21 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for 1st person camera
 *
 * Comments
 * 
 * for short descriptions see comments in camctrl_chase.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */

#include "..\acklib\acklib.h"
#include "camctrl.h"


/* ----- GLOBALS ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS ----- */

void CAMCTRL__1stperson_init(CAMCTRL* psCamctrl)
{
	/* report if relevant properties are not set and lock camera */
	if (psCamctrl->pentGenius == NULL)
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CAMCTRL__1stperson_init: CAMCTRL_setEntity not called");
		#endif
		psCamctrl->vCameraLocked = 1;
	}
	
 	/* no angle offset given - that's just fine, default to no offset */
 	if (psCamctrl->pangOffset == NULL)
	{
		psCamctrl->pangOffset = nullvector;
	}

	vec_set(&(psCamctrl->vecTemp), &(psCamctrl->pentGenius->x));
	/* view moving entity is not required for this mode. make it passable */
	psCamctrl->pentViewMover->flags |= PASSABLE;
}


void CAMCTRL__1stperson_loop(CAMCTRL* psCamctrl)
{
	vTemp = vec_dist(&(psCamctrl->pentGenius->x), &(psCamctrl->vecTemp));

	/* store moved distance */
	psCamctrl->vTemp += vTemp;

	/* set camera in place */
	vec_set (&(psCamctrl->pviewCam->x),   &(psCamctrl->pentGenius->x));
	vec_set (&(psCamctrl->pviewCam->pan), &(psCamctrl->pentGenius->pan));
	/* add camera angle offset */
	vec_add (&(psCamctrl->pviewCam->pan), &(psCamctrl->pangOffset->pan));
	/* walking influences camera height */
	psCamctrl->pviewCam->z += CAMCTRL_1STPERSON_SWAY * sinv(psCamctrl->vTemp * CAMCTRL_1STPERSON_SPEEDMUL) + CAMCTRL_1STPERSON_ZOFFSET;
	
	/* store position for next call */
	vec_set(&(psCamctrl->vecTemp), &(psCamctrl->pentGenius->x));
}
