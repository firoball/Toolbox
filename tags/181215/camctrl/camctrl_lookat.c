/*
 *******************************************************************************
 * camctrl_lookat.c
 * Creation date: 22.09.2010
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2010-09-22 00:07:23 +0200 (Mi, 22 Sep 2010) $
 * $Revision: 6 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for lookat camera
 *
 * Comments
 * 
 * for short descriptions see comments in camctrl_lookat.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */

#include "..\acklib\acklib.h"
#include "camctrl.h"


/* ----- GLOBALS ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS ----- */

void CAMCTRL__lookat_init(CAMCTRL* psCamctrl)
{
	/* report if relevant properties are not set and lock camera */
	if (psCamctrl->pentGenius == NULL)
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CAMCTRL__lookat_init: CAMCTRL_setEntity not called");
		#endif
		psCamctrl->vCameraLocked = 1;
	}
	/* view mover is not used, set it to passable to avoid unwanted collisions */
	psCamctrl->pentViewMover->flags|= PASSABLE;
}


void CAMCTRL__lookat_loop(CAMCTRL* psCamctrl)
{
	/* get ditance between view and genius entity */
	vec_set (&vecTemp, psCamctrl->pentGenius->x);
	vec_sub (&vecTemp, &(psCamctrl->pviewCam->x));

	/* calculate lookat angle */
	vec_to_angle (&angTemp, &vecTemp);

	/* smooth out angle difference */
	vec_sub (&angTemp, &(psCamctrl->pviewCam->pan));
	vec_ang (&angTemp);
	vec_scale (&angTemp, CAMCTRL_LOOKAT_SMOOTHFAC * time_step);

	/* adjust view angles */ 
	vec_add(&(psCamctrl->pentViewMover->pan), &angTemp);
	vec_ang (&(psCamctrl->pentViewMover->pan));
	/* view mover is also updated, although it is not used for this mode. This is done in order 
	 * to guarantee consistent angle data in case camera mode is changed on the fly.
	 */
	vec_set (&(psCamctrl->pviewCam->pan), &(psCamctrl->pentViewMover->pan));
}
