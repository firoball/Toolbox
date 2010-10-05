/*
 *******************************************************************************
 * state.c
 * Creation date: 29.12.2009
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
 * Script for State manager functions
 *
 * Comments
 * 
 * for short descriptions see comments in state.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */

#include <acknex.h>


/* ----- GLOBALS ----- */


void STATEMGR__pfCallback(); /* function pointer */
var STATEMGR__pfCheckCallback(); /* function pointer */


/* ----- EXTERNAL FUNCTIONS ----- */


STATEMGR* STATEMGR_create(var vState)
{
	STATEMGR* psState;
	
	/* allocate memory */
	psState = (STATEMGR*)sys_malloc(sizeof(STATEMGR));
	
	/* initialize data */
	psState->vState        = vState;
	psState->pfAction      = NULL;
	psState->pfInit        = NULL;
	psState->pfDeinit      = NULL;
	psState->psTransitions = LIST_create(); 
	psState->psActions     = LIST_create(); 

	return psState;
}


void STATEMGR_remove(STATEMGR* psState)
{
	/* delete everything (done inside LIST object automatically) */
	LIST_removeAll(psState->psTransitions);
	LIST_removeAll(psState->psActions);
	sys_free(psState);
}


void STATEMGR_addTransition(STATEMGR* psState, var vCurState, var vNewState, void* pfCheck, void* pfSwitch)
{
	STATE_TRAN* psTran;
	int i;
	var vTranExists;
		
	/* check if state was already added */
	vTranExists = 0;
	for (i = 0; i < LIST_items(psState->psTransitions); i++)
	{
		psTran = (STATE_TRAN*)LIST_getItem(psState->psTransitions, i);

		if ((vCurState == psTran->vCurState) && (vNewState == psTran->vNewState))
		{
			#ifdef SYSMSG_ACTIVE
			SYSMSG_print(SYSMSG_SHOW_ERROR, "STATEMGR_addTransition: transition already exists");
			#endif
			vTranExists = 1;
			break;
		}
	}

	/* transition does not yet exist - continue */
	if (vTranExists != 1)
	{
		/* allocate memory */
		psTran = (STATE_TRAN*)sys_malloc(sizeof(STATE_TRAN));
		
		psTran->vCurState = vCurState;
		psTran->vNewState = vNewState;
		psTran->pfCheck   = pfCheck;
		psTran->pfSwitch  = pfSwitch;
	
		/* now add transition to state machine */
		LIST_append(psState->psTransitions, (void*)psTran);	
	}
}


void STATEMGR_addState(STATEMGR* psState, var vCurState, void* pfAction, void* pfInit, void* pfDeinit)
{
	STATE_ACTION* psAction;
	int i;
	var vStateExists;
		
	/* check if state was already added */
	vStateExists = 0;
	for (i = 0; i < LIST_items(psState->psActions); i++)
	{
		psAction = (STATE_ACTION*)LIST_getItem(psState->psActions, i);

		if (vCurState == psAction->vState)
		{
			#ifdef SYSMSG_ACTIVE
			SYSMSG_print(SYSMSG_SHOW_ERROR, "STATEMGR_addState: state already exists");
			#endif
			vStateExists = 1;
			break;
		}
	}

	/* state does not yet exist - continue */
	if (vStateExists != 1)
	{
		/* allocate memory */
		psAction = (STATE_ACTION*)sys_malloc(sizeof(STATE_ACTION));
		
		psAction->vState    = vCurState;
		psAction->pfAction  = pfAction;
		psAction->pfInit    = pfInit;
		psAction->pfDeinit  = pfDeinit;

		/* now add state to state machine */
		LIST_append(psState->psActions, (void*)psAction);	

		/* make sure that initial state gets its cyclic action assigned */
		if (psState->vState == vCurState)
		{
			STATEMGR__updateActions(psState, psAction);
		}
	}
}


void STATEMGR_execute(STATEMGR* psState)
{
	/* check whether atate machine is ready */
	if (	(LIST_items(psState->psActions) > 0) && (LIST_items(psState->psTransitions) > 0) )
	{
		/* first check, if transition has to be done */
		STATEMGR__checkTransitions(psState);

		/* now execute cyclic action callback */
		if (psState->pfAction != NULL)
		{
			STATEMGR__pfCallback = psState->pfAction;
			STATEMGR__pfCallback();
		}
	}
}


var STATEMGR_getState(STATEMGR* psState)
{
	return psState->vState;
}


/* ----- INTERNAL FUNCTIONS ----- */

void STATEMGR__checkTransitions(STATEMGR* psState)
{
	int i;
	var vTran;
	STATE_TRAN* psTran;

	for (i = 0; i < LIST_items(psState->psTransitions); i++)
	{
		vTran = 0;
		psTran = (STATE_TRAN*)LIST_getItem(psState->psTransitions, i);

		/* current state fits, now check transition condition callback */
		if (psState->vState == psTran->vCurState)
		{
			if (psTran->pfCheck != NULL)
			{
				/* call transition check function */
				STATEMGR__pfCheckCallback = psTran->pfCheck;
				vTran = STATEMGR__pfCheckCallback();
				
				/* condition fulfilled - perform transition */
				if (vTran != 0)
				{
					STATEMGR__doTransition(psState, psTran);
					break;
				}
			}
			
		}
	}	
}


void STATEMGR__doTransition(STATEMGR* psState, STATE_TRAN* psTran)
{
	/* run deinitialization function of old state */
	if (psState->pfDeinit != NULL)
	{
		STATEMGR__pfCallback = psState->pfDeinit;
		STATEMGR__pfCallback();
	}
	
	/* update current state */
	psState->vState = psTran->vNewState;

	/* execute transition action if available*/
	if (psTran->pfSwitch != NULL)
	{
		STATEMGR__pfCallback = psTran->pfSwitch;
		STATEMGR__pfCallback();
	}					

	/* update pointer for cyclic callback action and run init function of new state */
	STATEMGR__getActions(psState);
}


void STATEMGR__getActions(STATEMGR* psState)
{
	int i;
	STATE_ACTION* psAction;
	
	for (i = 0; i < LIST_items(psState->psActions); i++)
	{
		psAction = (STATE_ACTION*)LIST_getItem(psState->psActions, i);

		/* check whether right Action was picked */
		if (psState->vState == psAction->vState)
		{
			STATEMGR__updateActions(psState, psAction);
			break;
		}
	}
}


void STATEMGR__updateActions(STATEMGR* psState, STATE_ACTION* psAction)
{
	/* found corresponding actions */
	psState->pfAction = psAction->pfAction;
	psState->pfInit   = psAction->pfInit;
	psState->pfDeinit = psAction->pfDeinit;

	/* run initialization function of new state */
	if (psState->pfInit != NULL)
	{
		STATEMGR__pfCallback = psState->pfInit;
		STATEMGR__pfCallback();
	}					
}
