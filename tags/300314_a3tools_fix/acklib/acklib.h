/*
 *******************************************************************************
 * acklib.h
 * Creation date: 21.06.2009
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
 * Definition Script for Library functions 
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef ACKLIB_H
#define ACKLIB_H
/*! \file 
 *  Include for general purpose functions.
 */

/*! \name General Purpose Variables 
 *  These variables can be used temporaily in own scripts.
 *  Their value remains valid until the next wait() instruction or the next
 *  function call which could also use those temporary variables.
 * \{ */
#ifdef DOXYGEN //Lite-C does not support externs
extern VECTOR vecTemp;	/*!< General purpose VECTOR */
extern ANGLE  angTemp;	/*!< General purpose ANGLE */
extern COLOR  colTemp;	/*!< General purpose COLOR */
extern var    vTemp;	/*!< General purpose var */
extern int    iTemp;	/*!< General purpose int */
extern char   cTemp;	/*!< General purpose char */
extern float  fTemp;	/*!< General purpose float */
extern long   lTemp;	/*!< General purpose long */
#endif
/* \} */

/* ----- DEFINITIONS ----- */


/* ----- STRUCTURES ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Limit angles for ANGLE vector like ang(). 
 *  \param	pang pointer to affected ANGLE 
 */
void vec_ang(ANGLE* pang);

/*! Avoid hysteresis for VECTOR. 
 *  \param	pvec pointer to affected VECTOR 
 *  \param	vHys allowed hysteresis (base 0) 
 */
void vec_hys(VECTOR* pvec, var vHys);

/*! perform clamp() on all elements of a VECTOR. 
 *  \param	pvec pointer to affected VECTOR 
 *  \param	vMinLim minimum limit 
 *  \param	vMaxLim maximum limit 
 */
void vec_clamp(VECTOR* pvec, var vMinLim, var vMaxLim);

/*! Calculate cross product of two vectors. 
 *  \param	pvecN pointer to normal (will be overwritten) 
 *  \param	pvec1 pointer to first VECTOR 
 *  \param	pvec2 pointer to second VECTOR 
 */
void vec_cross(VECTOR* pvecN, VECTOR* pvec1, VECTOR* pvec2);

#include "acklib.c"

#endif