/*
 *******************************************************************************
 * movement.h
 * Creation date: 24.08.2010
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2010-08-07 01:31:01 +0200 (Sa, 07 Aug 2010) $
 * $Revision: 5 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Definition script for entity movement
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef MOVEMENT_H
#define MOVEMENT_H
/*! \file 
 *  Include file for entity movement.
 */

/* ----- DEFINITIONS ----- */


#define MOVEMENT_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/*! \name Constant Data 
 *  These constants are used as parameters for certain functions.
 * \{ */
#define ANIM_CYCLE_UNDEFINED	0	/*!< type of animation cycle is undefined */
#define ANIM_CYCLE_INTERNAL	1	/*!< type of animation cycle is internal (not shared) */
#define ANIM_CYCLE_EXTERNAL	2	/*!< type of animation cycle is external (shared) */
/* \} */

/* ----- STRUCTURES ----- */


/*! Definition of an entity movement. 
 *  It is used for moving an entity by certain relative and absolute distances.
 *  Relative distance usually is the entity movement itself, while abolute distance
 *  can be caused by features of the level environment, e.g. icy floor or gravity.
 *  The MOVE structure keeps track of all the temporary values required for the movement calculation.
 *  Use the MOVE_create function for creation of a new entity movement.
 */
typedef struct
{
	ENTITY* entTarget;	/*!< Pointer to affected entity */
	VECTOR vecRelDist;	/*!< move distance vector of entity relative to entity viewing direction */
	VECTOR vecAbsDist;	/*!< move distance vector of entity relative to level */
	VECTOR vecRelForce;	/*!< force impact on entity relative to entity viewing direction */
	VECTOR vecAbsForce;	/*!< force impact on entity relative to level */
	var vState;				/*!< current movement state of entity */
	var vOldState;			/*!< last/old movement state of entity */
	var vIsJumping;		/*!< indicator whether entity currently is jumping */
}MOVE;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Creates new animation.
 *  Allows 10 animation cycles by default.
 *  Cycles are stored within an array, so gaps in the cycle id numbering may
 *  reduce the amount of cycles which can be stored.
 *  Make sure that the maximum cycle id is 9!
 *  \param	entTarget Pointer to entity to be animated 
 *  \return	pointer to newly created ANIM
 */
ANIM* ANIM_create(ENTITY* entTarget);

/*! Creates new animation.
 *  Allows a given number of animation cycles.
 *  Cycles are stored within an array, so gaps in the cycle id numbering may
 *  reduce the amount of cycles which can be stored.
 *  Make sure that the maximum cycle id is one less than the passed amount of cycles!
 *  \param	entTarget Pointer to entity to be animated 
 *  \param	vNumCycles   number of animation cycles 
 *  \return	pointer to newly created ANIM
 */
ANIM* ANIM_create(ENTITY* entTarget, var vNumCycles);

/*! Creates new animation.
 *  Allows a given number of animation cycles.
 *  Cycles are stored within an array, so gaps in the cycle id numbering may
 *  reduce the amount of cycles which can be stored.
 *  Make sure that the maximum cycle id is one less than the passed amount of cycles!
 *  Further thespeed for determining how fast blending is done can be given.
 *  \param	entTarget Pointer to entity to be animated 
 *  \param	vNumCycles   number of animation cycles 
 *  \param	vBlendSpeed  fadeout speed of animation blending
 *  \return	pointer to newly created ANIM
 */
ANIM* ANIM_create(ENTITY* entTarget, var vNumCycles, var vBlendSpeed);

/*! Removes an animation from memory. 
 *  \param	psAnim  Pointer to ANIM to be deleted 
 */
void ANIM_remove(ANIM* psAnim);

/*! Add internal animation cycle to ANIM. 
 *  May only be called during initialization phase but not during animation. 
 *  Internal aniamtion cycles are added on creation of the animation. When done for
 *  many instances with the same animation cycle set this can be very slow. In this case 
 *  external animation cycles should be considered as they are faster at initialization.
 *  Internal and external animation cycles must not be mixed!
 *  \param	psAnim   Pointer to ANIM 
 *  \param	vModeID  index/ID of animation mode (should be kept small and without gaps)
 *  \param	strName  internal identifier string (frame name) of aniamtion sequence
 *  \param	vSpeed   playback speed of animation
 *  \param  vOption  animation options as used by ent_animate: ANM_SKIP, ANM_CYCLE, ANM_ADD
 */
void ANIM_addCycle(ANIM* psAnim, var vModeID, STRING* strName, var vSpeed, var vOption);

/*! Add external animation cycles to ANIM. 
 *  May only be called during initialization phase but not during animation. 
 *  Adding cycles this way makes it possible to define all cycles one time at application 
 *  start and not each time, an animation is created. Useful if many entities share the
 *  same animation cycles.
 *  Internal and external animation cycles must not be mixed!
 *  \param	psAnim   Pointer to ANIM 
 *  \param	psCycles  Pointer to CYCLES - existing set of animation cycles
 */
void ANIM_addExtCycles(ANIM* psAnim, CYCLES* psCycles);

/*! Play animation .
 *  Has to be called cyclically every frame. 
 *  \param	psAnim	Pointer to ANIM 
 *  \param	vMode		Current animation mode of entity 
 */
void ANIM_play(ANIM* psAnim, var vMode);

/*! Play animation .
 *  Has to be called cyclically every frame. 
 *  \param	psAnim	Pointer to ANIM 
 *  \param	vMode		Current animation mode of entity 
 *  \param	vSpeed	additional time corrented animation speed of current cycle (for changing playback speed at runtime)
 */
void ANIM_play(ANIM* psAnim, var vMode, var vSpeed);

/*! Return progress in percent of currently played animation.
 *  \param	psAnim	Pointer to ANIM 
 *  \return	animation progress in percent
 */
VECTOR* MOVE_getRelDist(MOVE* psMove);
VECTOR* MOVE_getAbsDist(MOVE* psMove);
void MOVE_getAbsDist(MOVE* psMove);

/*! Return mode id of currently active movement.
 *  \param	psMove	Pointer to MOVE 
 *  \return	movement mode id
 */
var MOVE_getState(MOVE* psMove);



/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
ANIM_CYCLE* ANIM_CYCLE__create(var vNumCycles);
/*! \internal - Do not use! */
void ANIM_CYCLE__remove(ANIM_CYCLE* psCycle);


#include "movement.c"

#endif