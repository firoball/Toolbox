/*
 *******************************************************************************
 * camctrl_chase.c
 * Creation date: 24.06.2009
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date$
 * $Revision$
 * $Author$
 *
 *******************************************************************************
 * Description
 *
 * Script for chase camera
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

void CAMCTRL__chase_init(CAMCTRL* psCamctrl)
{
	/* report if relevant properties are not set and lock camera */
	if (psCamctrl->pentGenius == NULL)
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CAMCTRL__chase_init: CAMCTRL_setEntity not called");
		#endif
		psCamctrl->vCameraLocked = 1;
	}

 	/* no distance given - that's just fine, use default distance */
	if (psCamctrl->pvecViewDist == NULL)
	{
		psCamctrl->pvecViewDist = &psCamctrl->vecDefaultDist;
	}

 	/* no angle offset given - that's just fine, default to no offset */
 	if (psCamctrl->pangOffset == NULL)
	{
		psCamctrl->pangOffset = nullvector;
	}

}


void CAMCTRL__chase_loop(CAMCTRL* psCamctrl)
{
	vec_set (&vecTemp, psCamctrl->pvecViewDist);

	/* obtain orientation of parent entity */
	vec_set (&angTemp, psCamctrl->pangOffset);
	ang_add (&angTemp, &(psCamctrl->pentGenius->pan));
	
	/* rotate distance vector by current angle and set distance vector in place */
	vec_rotate (&vecTemp, &angTemp);
	vec_add (&vecTemp, &(psCamctrl->pentGenius->x));
	vec_sub (&vecTemp, &(psCamctrl->pentViewMover->x));
	vec_scale (&vecTemp, 0.3 * time_step);	/* smooth out camera movement */	

	/* move pentViewMover and attach camera view */	
	c_move(psCamctrl->pentViewMover, nullvector, &vecTemp, 
		IGNORE_ME | IGNORE_YOU | IGNORE_PASSABLE | /*IGNORE_MODELS |*/ 
		IGNORE_SPRITES | IGNORE_PUSH | IGNORE_CONTENT | USE_AABB | GLIDE);

		
	/* adjust viewing angles depending on new position */ 
	vec_set (&vecTemp, &(psCamctrl->pentGenius->x));
	vec_sub (&vecTemp, &(psCamctrl->pentViewMover->x));
	vec_to_angle (&angTemp, &vecTemp);
	vec_set (&(psCamctrl->pentViewMover->pan), &angTemp);
	vec_ang (&(psCamctrl->pentViewMover->pan));

	/* set camera in place */
	vec_set (&(psCamctrl->pviewCam->x),   &(psCamctrl->pentViewMover->x));
	vec_set (&(psCamctrl->pviewCam->pan), &(psCamctrl->pentViewMover->pan));
}
