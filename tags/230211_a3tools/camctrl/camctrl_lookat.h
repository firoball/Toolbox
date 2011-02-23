/*
 *******************************************************************************
 * camctrl_lookat.h
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
 * Definition script for lookat camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
#ifndef CAMCTRL_LOOKAT_H
#define CAMCTRL_LOOKAT_H
/*! \file 
 * \b Internal - include file for lookat camera.
 * Included by camctrl.h. Should not be included directly.
 */

/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */


#define CAMCTRL_LOOKAT_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef CAMCTRL_LOOKAT_SMOOTHFAC
#define CAMCTRL_LOOKAT_SMOOTHFAC     0.3	/*!< Smooth factor for viewing angle adjustment (0.01 - 1.0) */
#endif

/* \} */


/* ----- STRUCTURES ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
void CAMCTRL__lookat_loop(CAMCTRL* psCamctrl);
/*! \internal - Do not use! */
void CAMCTRL__lookat_init(CAMCTRL* psCamctrl);

#include "camctrl_lookat.c"