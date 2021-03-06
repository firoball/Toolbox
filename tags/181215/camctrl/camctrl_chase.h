/*
 *******************************************************************************
 * camctrl_chase.h
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
 * Definition script for chase camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
#ifndef CAMCTRL_CHASE_H
#define CAMCTRL_CHASE_H
/*! \file 
 * \b Internal - include file for chase camera.
 * Included by camctrl.h. Should not be included directly.
 */

/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */

#define CAMCTRL_CHASE_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef CAMCTRL_CHASE_SMOOTHFAC
#define CAMCTRL_CHASE_SMOOTHFAC     0.3	/*!< Smooth factor for position adjustment (0.01 - 1.0) */
#endif

/* \} */


/* ----- STRUCTURES ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
void CAMCTRL__chase_loop(CAMCTRL* psCamctrl);
/*! \internal - Do not use! */
void CAMCTRL__chase_init(CAMCTRL* psCamctrl);

#include "camctrl_chase.c"


#endif