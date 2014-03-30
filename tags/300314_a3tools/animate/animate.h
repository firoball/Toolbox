/*
 *******************************************************************************
 * animate.h
 * Creation date: 04.08.2010
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


/*! \name Constant Data 
 *  These constants are used as parameters for certain functions.
 * \{ */
#define ANIM_CYCLE_UNDEFINED	0	/*!< type of animation cycle is undefined */
#define ANIM_CYCLE_INTERNAL	1	/*!< type of animation cycle is internal (not shared) */
#define ANIM_CYCLE_EXTERNAL	2	/*!< type of animation cycle is external (shared) */
/* \} */

/* ----- STRUCTURES ----- */


/*! Definition of an animation cycle.
 *  An aniamtion cycle covers all the information required to play a series of frames belonging
 *  together (e.g. walk animation). For each animation sequence an animation cycle has to be 
 *  defined during initialization phase before aniamtion is being played.
 */
typedef struct
{
	STRING* strName;  /*!< Internal identifier of animation sequence */
	var vSpeed;       /*!< Playback speed of animation */
	var vOption;      /*!< ent_animate mode: ANM_SKIP, ANM_CYCLE, ANM_ADD */
}ANIM_CYCLE;

/*! Definition of an animation. 
 *  It is used for playback of different animation cycles of an entity.
 *  This module automatically performs blending between different frames.
 *  By defining multiple aniamtion cycles it is possible to adjust
 *  playback speed separately for each animation sequence.
 *  Use the ANIM_create function for creation of a new animation.
 */
typedef struct
{
	ENTITY* entTarget;   /*!< Pointer to affected entity */
	ANIM_CYCLE* psCycles;/*!< List of animation cycles */
	var vBlendTimer;     /*!< blending state of last animation cycle 0-100% */
	var vAnimTimer;      /*!< state of current animation cycle 0-100% */
	var vLastAnimTimer;  /*!< state of last animation cycle 0-100% */
	var vLastMode;       /*!< last value of animation mode (used for detection of mode change) */
	var vOldMode;        /*!< old animation mode (used for blending) */
	var vNumCycles;      /*!< number of animation cycles */
	var vBlendSpeed;     /*!< factor for adjusting blending speed in ticks */
	var vCycleType;      /*!< type of animation cycle (ANIM_CYCLE_xxx) */
}ANIM;

/*! Definition of a set of animation cycles. 
 *  It is used for storing several animation cycles.
 *  Animation cycles caneither be stored directly inside of an ANIM animation
 *  or can be stored externally in a CYCLES set of animation cycles.
 *  For often used animation cycles it is recommended to use an external set of cycles.
 *  It initializes faster and consumes less memory.
 */
typedef struct
{
	ANIM_CYCLE* psCycles;/*!< List of animation cycles */
	var vNumCycles;      /*!< number of animation cycles */
}CYCLES;

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
var ANIM_getAnimPercent(ANIM* psAnim);

/*! Return mode id of currently played animation.
 *  \param	psAnim	Pointer to ANIM 
 *  \return	animation mode id
 */
var ANIM_getAnimMode(ANIM* psAnim);


/*! Creates new set animation cycles.
 *  Allows a given number of animation cycles.
 *  Cycles are stored within an array, so gaps in the cycle id numbering may
 *  reduce the amount of cycles which can be stored.
 *  Make sure that the maximum cycle id is one less than the passed amount of cycles!
 *  \param	vNumCycles   number of animation cycles 
 *  \return	pointer to newly created ANIM
 */
CYCLES* CYCLES_create(var vNumCycles);


/*! Removes a set of animation cyclesfrom memory. 
 *  \param	psCycle  Pointer to CYCLES to be deleted 
 */
void CYCLES_remove(CYCLES* psCycle);


/*! Add external animation cycle to set of aniamtion cycles.
 *  May only be called during initialization phase but not during animation. 
 *  Toa ssign an external aniamtion cycle list to an animation, use ANIM_addExtCycles().
 *  \param	psCycles Pointer to CYCLES - list of animation cycles
 *  \param	vModeID  index/ID of animation mode (should be kept small and without gaps)
 *  \param	strName  internal identifier string (frame name) of aniamtion sequence
 *  \param	vSpeed   playback speed of animation
 *  \param  vOption  animation options as used by ent_animate: ANM_SKIP, ANM_CYCLE, ANM_ADD
 */
void CYCLES_addCycle(CYCLES* psCycles, var vModeID, STRING* strName, var vSpeed, var vOption);


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
ANIM_CYCLE* ANIM_CYCLE__create(var vNumCycles);
/*! \internal - Do not use! */
void ANIM_CYCLE__remove(ANIM_CYCLE* psCycle);


#include "animate.c"

#endif