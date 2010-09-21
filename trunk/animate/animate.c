/*
 *******************************************************************************
 * animate.c
 * Creation date: 05.08.2010
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
 * Script for entity animations
 *
 * Comments
 * 
 * for short descriptions see comments in animate.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */


#include <acknex.h>


/* ----- GLOBALS ----- */


ANIM_CYCLE* ANIM_psCycle;


/* ----- EXTERNAL FUNCTIONS ----- */


ANIM* ANIM_create(ENTITY* entTarget)
{
	return ANIM_create(entTarget, (var)10, (var)20);
}


ANIM* ANIM_create(ENTITY* entTarget, var vNumCycles)
{
	return ANIM_create(entTarget, vNumCycles, (var)20);
}


ANIM* ANIM_create(ENTITY* entTarget, var vNumCycles, var vBlendSpeed)
{
	var i;
	ANIM* psAnim;

	psAnim           = (ANIM*)malloc(sizeof(ANIM));
	psAnim->psCycles = NULL;
#ifdef blafusel
	psAnim->psCycles = (ANIM_CYCLE*)malloc(sizeof(ANIM_CYCLE) * vNumCycles);
	/* initialize animation cycles */
	for (i = 0; i < vNumCycles; i++)
	{
		ANIM_psCycle = &(psAnim->psCycles)[i];
		ANIM_psCycle->strName = NULL;
		ANIM_psCycle->vSpeed  = 1;
		ANIM_psCycle->vOption = 0;
	}
#endif	
	/* initialize animation */
	psAnim->entTarget      = entTarget;
	psAnim->vBlendTimer    = 0;
	psAnim->vAnimTimer     = 0;
	psAnim->vLastAnimTimer = 0;
	psAnim->vLastMode      = 0;
	psAnim->vOldMode       = 0;
	psAnim->vNumCycles     = vNumCycles;
	psAnim->vBlendSpeed    = vBlendSpeed;	
	psAnim->vCycleType     = ANIM_CYCLE_UNDEFINED;
	return psAnim;	
}


void ANIM_remove(ANIM* psAnim)
{
	/* internal animation cycles found - remove them first */
	if (psAnim->vCycleType == ANIM_CYCLE_INTERNAL)
	{
		ANIM_CYCLE__remove(psAnim->psCycles);
	}
	/* remove the animation */
	free(psAnim);
}


void ANIM_addCycle(ANIM* psAnim, var vModeID, STRING* strName, var vSpeed, var vOption)
{
	if (psAnim->vCycleType == ANIM_CYCLE_UNDEFINED)
	{
		psAnim->psCycles = ANIM_CYCLE__create(psAnim->vNumCycles);
		psAnim->vCycleType = ANIM_CYCLE_INTERNAL;
	}
	
	if ((vModeID < psAnim->vNumCycles) && (psAnim->vCycleType == ANIM_CYCLE_INTERNAL))
	{
		ANIM_psCycle = &(psAnim->psCycles)[vModeID];
		ANIM_psCycle->strName = str_create(strName);
		ANIM_psCycle->vSpeed = vSpeed;
		ANIM_psCycle->vOption = vOption & (ANM_SKIP | ANM_CYCLE | ANM_ADD);	
	}

	#ifdef SYSMSG_ACTIVE
	else
	{
		SYSMSG_print(SYSMSG_SHOW_ERROR, "ANIM_addCycle: Mode ID exceeds maximum animation cycles or external cycles were found");		
	}
	#endif
	
}


void ANIM_addExtCycles(ANIM* psAnim, CYCLES* psCycles)
{
	if ((psAnim->psCycles == NULL) && (psAnim->vCycleType == ANIM_CYCLE_UNDEFINED))
	{
		psAnim->psCycles = psCycles->psCycles;
		psAnim->vNumCycles = psCycles->vNumCycles;
		psAnim->vCycleType = ANIM_CYCLE_EXTERNAL;
	}
	
	#ifdef SYSMSG_ACTIVE
	else
	{
		SYSMSG_print(SYSMSG_SHOW_ERROR, "ANIM_addCycle: Cannot add external animation cycle. Already existing cycles were found.");		
	}
	#endif
}


void ANIM_play(ANIM* psAnim, var vMode)
{
	ANIM_play(psAnim, vMode, 0);
}


void ANIM_play(ANIM* psAnim, var vMode, var vSpeed)
{
	/* CAUTION: for speed reasons NO range check on vMode is performed!! */
	
	ANIM_psCycle = &(psAnim->psCycles)[vMode];

	/* mode has changed during blending phase? */
	if (psAnim->vLastMode != vMode)
	{
		psAnim->vOldMode = psAnim->vLastMode;
		psAnim->vLastMode = vMode;
		psAnim->vBlendTimer = 100;
		/* get last frame percentage */
		psAnim->vLastAnimTimer = psAnim->vAnimTimer;
		/* reset animation timer when mode change occurs */
		psAnim->vAnimTimer = 0;
	}
	else
	{
		psAnim->vAnimTimer += (time_step * ANIM_psCycle->vSpeed) + vSpeed;
		/* check whether animation is terminated or looping */
		if ((ANIM_psCycle->vOption & ANM_CYCLE) != 0)
		{
			/* loop animation */
			psAnim->vAnimTimer %= 100;
		}
		else
		{
			/* limit animation to 100% */
			psAnim->vAnimTimer = minv(psAnim->vAnimTimer, 100);
		}
	}

	/* play animation */
	ent_animate(psAnim->entTarget, ANIM_psCycle->strName, psAnim->vAnimTimer, ANIM_psCycle->vOption);	
		
	/* now perform blending */
	/* blending phase finished or mode change during blending phase? */
	if (psAnim->vBlendTimer != 0)
	{

		/* decrement blend percentage and blend old animation */
		psAnim->vBlendTimer = maxv(0, psAnim->vBlendTimer - (time_step * psAnim->vBlendSpeed));
		ANIM_psCycle = &(psAnim->psCycles)[psAnim->vOldMode];
		ent_blendframe (psAnim->entTarget, psAnim->entTarget, ANIM_psCycle->strName, psAnim->vLastAnimTimer, psAnim->vBlendTimer);
		if (psAnim->vBlendTimer == 0)
		{
			/* blending finished */
			psAnim->vOldMode = vMode;
		}
	}
	else
	{
		/* get last frame percentage */
		psAnim->vLastAnimTimer = psAnim->vAnimTimer;
	}
}


var ANIM_getAnimPercent(ANIM* psAnim)
{
	return (psAnim->vAnimTimer);
}


var ANIM_getAnimMode(ANIM* psAnim)
{
	return (psAnim->vLastMode);
}


CYCLES* CYCLES_create(var vNumCycles)
{
	CYCLES* psCycles;
	
	psCycles = (CYCLES*)malloc(sizeof(CYCLES));
	psCycles->psCycles = ANIM_CYCLE__create(vNumCycles);
	psCycles->vNumCycles = vNumCycles;
	
	return psCycles;	
}

void CYCLES_remove(CYCLES* psCycles)
{
	ANIM_CYCLE__remove(psCycles->psCycles);
	free(psCycles);	
}

void CYCLES_addCycle(CYCLES* psCycles, var vModeID, STRING* strName, var vSpeed, var vOption)
{
	var vNumCycles;
	
	if (vModeID < psCycles->vNumCycles)
	{
		ANIM_psCycle = &(psCycles->psCycles)[vModeID];
		ANIM_psCycle->strName = str_create(strName);
		ANIM_psCycle->vSpeed  = vSpeed;
		ANIM_psCycle->vOption = vOption & (ANM_SKIP | ANM_CYCLE | ANM_ADD);	
	}

	#ifdef SYSMSG_ACTIVE
	else
	{
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CYCLES_addCycle: Mode ID exceeds maximum animation cycles");		
	}
	#endif
	
}


/* ----- INTERNAL FUNCTIONS ----- */


ANIM_CYCLE* ANIM_CYCLE__create(var vNumCycles)
{
	ANIM_CYCLE* psCycles;
	var i;
	
	psCycles = (ANIM_CYCLE*)malloc(sizeof(ANIM_CYCLE) * vNumCycles);

	/* initialize animation cycles */
	for (i = 0; i < vNumCycles; i++)
	{
		psCycles[i].strName = NULL;
		psCycles[i].vSpeed  = 1;
		psCycles[i].vOption = 0;
	}

	return psCycles;	
}


void ANIM_CYCLE__remove(ANIM_CYCLE* psCycle)
{
	var i;
	var vNumCycles;
	
	vNumCycles = sizeof(psCycle) / sizeof(ANIM_CYCLE);	

	/* first remove all strings */
	for (i = 0; i < vNumCycles; i++)
	{
		if (psCycle[i].strName != NULL)
		{
			ptr_remove(psCycle[i].strName);
		}
	}
	/* remove the anim cycles */
	free(psCycle);
}



