/*
 *******************************************************************************
 * camctrl.c
 * Creation date: 04.11.2008
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
 * Script for camera control
 *
 * Comments
 * 
 * for short descriptions see comments in camctrl.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */

#include <acknex.h>
#include "camctrl.h"

/* camera modes */
#include "camctrl_1stperson.h"
#include "camctrl_chase.h"
#include "camctrl_follow.h"


/* ----- GLOBALS ----- */


/* ----- EXTERNAL FUNCTIONS ----- */

CAMCTRL* CAMCTRL_create(void)
{
	CAMCTRL* psCamctrl;
	VIEW* pviewCam;
		
	/* no parameters given - create new view */
	pviewCam = view_create(CAMCTRL_DEFLAYER);
	psCamctrl = CAMCTRL_create(pviewCam);
	/* overwrite view creation flag in order to free memory on CAMCTRL object removal */
	psCamctrl->vViewCreated = ON;
	
	return (psCamctrl);
}

CAMCTRL* CAMCTRL_create(VIEW* pviewCam)
{
	CAMCTRL* psCamctrl;
	
	psCamctrl = (CAMCTRL*)malloc(sizeof(CAMCTRL));
	vec_set(&(pviewCam->x), nullvector);
	pviewCam->flags &= ~SHOW;	/* make sure SHOW flag is NOT set */
	psCamctrl->vViewCreated  = OFF;
	psCamctrl->pviewCam      = pviewCam;
	psCamctrl->pvecTargetPos = NULL; 
	psCamctrl->pvecLookAtPos = NULL;
	psCamctrl->pvecViewDist  = NULL;
	psCamctrl->pentGenius    = NULL;
	psCamctrl->pentViewMover = ent_create("sphere.mdl", &(pviewCam->x), NULL);
	psCamctrl->pentViewMover->flags|= INVISIBLE;
	psCamctrl->pangOffset    = NULL;
	psCamctrl->pangTarget    = NULL;
	psCamctrl->vMode         = CAMCTRL_1STPERSON;
	psCamctrl->vLastMode     = CAMCTRL_1STPERSON;
	psCamctrl->vSmoothing    = CAMCTRL_DEFSMOOTHFAC;
	psCamctrl->vSpeed        = CAMCTRL_DEFSPEED;
	vec_set(&(psCamctrl->vecTemp), nullvector);
	psCamctrl->vTemp         = 0;

	/* default values */
	vec_set(&psCamctrl->vecDefaultDist, vector(CAMCTRL_DEFDIST,0,0));

	return (psCamctrl);
}

void CAMCTRL_remove(CAMCTRL* psHandle)
{
	/* view created at CAMCTRL object creation? remove it! */
	if (psHandle->vViewCreated == ON)
		ptr_remove(psHandle->pviewCam);
	/* remove CAMCTRL object from memory */
	free(psHandle);
}

void CAMCTRL_show(CAMCTRL* psHandle)
{
	if (!(psHandle->pviewCam->flags & SHOW))
	{
		psHandle->pviewCam->flags |= SHOW;
		/* init camera mode */
		CAMCTRL__mode_init(psHandle);
		/* cyclically perform camera movement */
		CAMCTRL__move_loop(psHandle);
	}
}

void CAMCTRL_hide(CAMCTRL* psHandle)
{
	if (psHandle->pviewCam->flags & SHOW)
	{
		psHandle->pviewCam->flags &= ~SHOW;
		/* deactivate camera movement */
		psHandle->vMode = CAMCTRL_INACTIVE;
	}
}

void CAMCTRL_setMode(CAMCTRL* psHandle, var vMode)
{
	/* store old camera mode */
	psHandle->vLastMode = psHandle->vMode;
	/* set new camera mode */
	psHandle->vMode = vMode;
	/* perform deinitialization */
	CAMCTRL__mode_deinit(psHandle);
	if (psHandle->pviewCam->flags & SHOW)
	{
		/* init new camera mode */
		CAMCTRL__mode_init(psHandle);
		/* cyclically perform camera movement if view is already visible */
		CAMCTRL__move_loop(psHandle);
	}
}

void CAMCTRL_setDist(CAMCTRL* psHandle, VECTOR* pvecDist)
{
	/* get pointer to position offset from distance vector */
	psHandle->pvecViewDist = pvecDist;
}

void CAMCTRL_setAngle(CAMCTRL* psHandle, ANGLE* pangDir)
{
	/* get pointer to angle offset from ANGLE vector */
	psHandle->pangOffset = pangDir;
}

void CAMCTRL_setPos(CAMCTRL* psHandle, VECTOR* pvecPos)
{
	vec_set(&(psHandle->pviewCam->x), pvecPos);
}

void CAMCTRL_setEntity(CAMCTRL* psHandle, ENTITY* pentFocus)
{
	/* set internal pointer to parent entity */
	psHandle->pentGenius = pentFocus;
	/* get target view position from entity */
	psHandle->pvecTargetPos = &(pentFocus->x);
	/* get target view angles from entity */
	psHandle->pangTarget = &(pentFocus->pan);
	
	/* set view genius */
	psHandle->pviewCam->genius = pentFocus;
	
}

void CAMCTRL_setSmoothFac(CAMCTRL* psHandle, var vFac)
{
	/* users are dumb. limit to valid range */
	psHandle->vSmoothing = clamp(vFac, 0.01, 1);
}

void CAMCTRL_setSpeed(CAMCTRL* psHandle, var vSpeed)
{
	psHandle->vSpeed = vSpeed;
}

VIEW* CAMCTRL_getView(CAMCTRL* psHandle)
{
	return (psHandle->pviewCam);
}


/* ----- INTERNAL FUNCTIONS ----- */

void CAMCTRL__move_loop(CAMCTRL* psHandle)
{
	/* camera is locked due to missing initialization */
	if (psHandle->vCameraLocked == 1)
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_DEBUG, "CAMCTRL__move_loop: camera is locked and inactive");
		#endif
		return;
	}
	
	proc_mode = PROC_LATE; /* put function to end of scheduler list */
	while (psHandle->pviewCam->flags & SHOW)
	{
		switch(psHandle->vMode)
		{
			case CAMCTRL_1STPERSON:
				CAMCTRL__1stperson_loop(psHandle);
				break;
				
			case CAMCTRL_CHASE:
				CAMCTRL__chase_loop(psHandle);
				break;
				
			case CAMCTRL_FIXED:
				break;
				
			case CAMCTRL_LOOKAT:
				break;
				
			case CAMCTRL_FOLLOW:
				CAMCTRL__follow_loop(psHandle);
				break;
				
			default:
				/* invalid mode selected */
				break;
		}
		wait (1);
	}
}

void CAMCTRL__mode_init(CAMCTRL* psHandle)
{
	/* remove camera lock */
	psHandle->vCameraLocked = 0;
	
	switch(psHandle->vMode)
	{
		case CAMCTRL_1STPERSON:
			CAMCTRL__1stperson_init(psHandle);
			break;
			
		case CAMCTRL_CHASE:
			CAMCTRL__chase_init(psHandle);
			break;
			
		case CAMCTRL_FIXED:
			break;
			
		case CAMCTRL_LOOKAT:
			break;
			
		case CAMCTRL_FOLLOW:
			CAMCTRL__follow_init(psHandle);
			break;
			
		default:
			/* invalid mode selected */
			break;
	}
}

void CAMCTRL__mode_deinit(CAMCTRL* psHandle)
{
	/* all pointers which can be modified by optional calls are reset in order
	   to avoid different behaviour on multiple runs due to some remaining 
	   old setup from a different camera mode.
	 */
	psHandle->pvecTargetPos = NULL; 
	psHandle->pvecLookAtPos = NULL;
	psHandle->pvecViewDist  = NULL;
	psHandle->pentGenius    = NULL;
	psHandle->pangOffset    = NULL;
	psHandle->pangTarget    = NULL;
	vec_set(&(psHandle->vecTemp), nullvector);
	psHandle->vTemp         = 0;
	psHandle->pentViewMover->flags &= ~PASSABLE;
}
