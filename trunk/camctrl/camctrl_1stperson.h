/*
 *******************************************************************************
 * camctrl_1stperson.h
 * Creation date: 25.06.2009
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
 * Definition script for 1stperson camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
#ifndef CAMCTRL_1STPERSON_H
#define CAMCTRL_1STPERSON_H
/*! \file 
 * \b Internal - include file for 1st person camera.
 * Included by camctrl.h. Should not be included directly.
 */

/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */

#define CAMCTRL_1STPERSON_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef CAMCTRL_1STPERSON_SWAY
#define CAMCTRL_1STPERSON_SWAY     10	/*!< Strength of camera up/down movement during walk */
#endif

#ifndef CAMCTRL_1STPERSON_ZOFFSET
#define CAMCTRL_1STPERSON_ZOFFSET  50	/*!< Offset for camera height */
#endif

#ifndef CAMCTRL_1STPERSON_SPEEDMUL
#define CAMCTRL_1STPERSON_SPEEDMUL 2	/*!< Multiplier for walk speed impact */
#endif
/* \} */


/* ----- STRUCTURES ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
void CAMCTRL__1stperson_loop(CAMCTRL* psCamctrl);
/*! \internal - Do not use! */
void CAMCTRL__1stperson_init(CAMCTRL* psCamctrl);

#include "camctrl_1stperson.c"


#endif