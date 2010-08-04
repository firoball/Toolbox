/*
 *******************************************************************************
 * animate.h
 * Creation date: 04.08.2010
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
 * Definition script for entity animations
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef ANIMATE_H
#define ANIMATE_H
/*! \file 
 *  Include file for entity animations.
 */

/* ----- DEFINITIONS ----- */


#define ANIMATE_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/* ----- STRUCTURES ----- */


/*! Definition of an animation cycle.
 *  An aniamtion cycle covers all the information required to play a series of frames belonging
 *  together (e.g. walk animation). For each animation sequence an animation cycle has to be 
 *  defined during initialization phase before aniamtion is being played.
 */
typedef struct
{
	STRING* strName;	/*!< Internal identifier of animation sequence */
	var vSpeed;			/*!< Playback speed of aniamtion */
	var vOption;
}ANIM_CYCLE;

/*! Definition of an animation. 
 *  A list is used to store a set of pointers to user defined objects.
 *  The size of the list is adapted automatically at runtime. The size of new memory 
 *  allocated when the list has reached its current limit, can be modified by redefining
 *  LIST_ARRAY_STEPS.
 *  Use the LIST_create function for creation of a new list.
 */
typedef struct
{
	ENTITY* entTarget;	/*!< Pointer to affected entity */
	ANIM_CYCLE* psCycles;/*!< List of animation cycles */
	var vBlendTimer;		/*!< blending state of last animation cycle 0..100% */
	var vAnimTimer;		/*!< state of current animation cycle 0..100% */
	var vLastMode;			/*!< last active animation mode */
	var vOldMode;
	var vNumCycles;		/*!< number of animation cycles */
	var vBlendSpeed;		/*!< factor for adjusting blending speed in ticks */
}ANIM;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! creates new animation.
 *  Allows 10 animation cycles by default.
 *  \param	entTarget Pointer to entity to be animated 
 *  \return	pointer to newly created ANIM
 */
ANIM* ANIM_create(ENTITY* entTarget);

/*! creates new animation.
 *  Allows a given number animation cycles.
 *  \param	entTarget Pointer to entity to be animated 
 *  \param	vCycles   number of animation cycles 
 *  \return	pointer to newly created ANIM
 */
ANIM* ANIM_create(ENTITY* entTarget, var vCycles);

/*! Removes an animation from memory. 
 *  \param	psAnim  Pointer to ANIM to be deleted 
 */
void ANIM_remove(ANIM* psAnim);

/*! Add animation cycle to ANIM. 
 *  MAy only be called during initialization phase but not during animation. 
 *  Indices of follow-up items are NOT affected 
 *  \param	psHost  Pointer to host LIST 
 *  \param	iIndex List index for stored item 
 */
void ANIM_addCycle(ANIM* psAnim, var vModeID, STRING* strName, var vSpeed, var vOption);

/*! Play animation .
 *  Has to be called cyclically every frame. 
 *  \param	psAnim	Pointer to ANIM 
 *  \param	vMode		Current animation mode of entity 
 */
void ANIM_play(ANIM* psAnim, var vMode);


/*! return progress in percent of currently played aniamtion.
 *  Allows a given number animation cycles.
 *  \param	psAnim	Pointer to ANIM 
 *  \return	animation progress in percent
 */
var ANIM_getAnimPercent(ANIM* psAnim);

/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */


/*! \internal - Do not use! */
//void LIST__extend(LIST* psHost);
/*! \internal - Do not use! */
//void LIST__init(LIST* psHost);

#include "animate.c"

#endif