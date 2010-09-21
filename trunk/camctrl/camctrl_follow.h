/*
 *******************************************************************************
 * camctrl_follow.h
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
 * Definition script for follower camera
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
#ifndef CAMCTRL_FOLLOW_H
#define CAMCTRL_FOLLOW_H
/*! \file 
 * \b Internal - include file for follower camera.
 * Included by camctrl.h. Should not be included directly.
 */

/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */


#define CAMCTRL_FOLLOW_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/* ----- STRUCTURES ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
void CAMCTRL__follow_loop(CAMCTRL* psCamctrl);
/*! \internal - Do not use! */
void CAMCTRL__follow_init(CAMCTRL* psCamctrl);

#include "camctrl_follow.c"