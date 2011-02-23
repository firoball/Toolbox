/*
 *******************************************************************************
 * invent.c
 * Creation date: 03.05.2009
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
 * Script for Inventory functions
 *
 * Comments
 * 
 * for short descriptions see comments in invent.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */


#include <acknex.h>


/* ----- GLOBALS ----- */


#ifdef SYSMSG_ACTIVE
var vInstInventory = 0;	/* instance counter for INVENTORY */
var vInstInvitem   = 0; /* instance counter for INVITEM */
STRING* strMsg = "#30";
STRING* strInst = "#9";
#endif

void INVITEM_pfExtRemoval(void* fpExt); /* function pointer */


/* ----- EXTERNAL FUNCTIONS ----- */

/*
NOTE:
You could simulate the digits text with a str_printf call, 
and then use str_width the get the pixel-precise width of that text.
*/
INVENTORY* INVENTORY_create(STRING* strBmap, var vLayer)
{
	INVENTORY* psInv;
	var vWidth;
	var vHeight;
	
	/* allocate memory */
	psInv = (INVENTORY*)sys_malloc(sizeof(INVENTORY));

	#ifdef SYSMSG_ACTIVE
	vInstInventory++;
	str_cpy(strMsg, "INVENTORY_create: new instance - ");
	str_for_num(strInst, vInstInventory); 
	str_cat(strMsg, strInst);
	SYSMSG_print(SYSMSG_SHOW_DEBUG, strMsg);
	#endif
	
	/* initialize data */
	psInv->vMaxItems  = INV_UNLIMITEDITEMS;
	psInv->vMaxWeight = INV_UNLIMITEDWEIGHT;
	psInv->vMaxAmount = INV_UNLIMITEDAMOUNT;

	psInv->vCurItems  = 0;
	psInv->vCurWeight = 0;

	psInv->vWeightCur = 0;
	psInv->vAmountCur = 0;
	
	psInv->psCurItem     = NULL;
	psInv->pbmapBackdrop = bmap_create(strBmap);

	psInv->vAlignment = INV_ALIGN_LEFT | INV_ALIGN_BOTTOM;

	psInv->vPosX = 0;
	psInv->vPosY = 0;

	psInv->vFadeActive = 0;

	vWidth = bmap_width(psInv->pbmapBackdrop);
	vHeight = bmap_height(psInv->pbmapBackdrop);

	/* initialize panel of current item */
	psInv->ppanCur = pan_create("digits(0, 0, \"%2.0f\", \"*\", 1, 1); 
		digits(20, 0, \"%2.0f\", \"*\", 1, 1);
		digits(0, 0, \"%3.0f\", \"*\", 1, 1);
		digits(0, 0, \"/%3.0f\", \"*\", 1, 1);", vLayer);
	pan_setvar(psInv->ppanCur, 1, 1, &psInv->vAmountCur);
	pan_setvar(psInv->ppanCur, 1, 2, &psInv->vWeightCur);
	pan_setvar(psInv->ppanCur, 1, 3, &psInv->vCurWeight);
	pan_setvar(psInv->ppanCur, 1, 4, &psInv->vMaxWeight);
	psInv->ppanCur->bmap = psInv->pbmapBackdrop;
	psInv->ppanCur->size_x = vWidth;
	psInv->ppanCur->size_y = vHeight;
	psInv->ppanCur->scale_x = INV_SCALE;
	psInv->ppanCur->scale_y = INV_SCALE;
	psInv->ppanCur->alpha = INV_ALPHA;
	psInv->ppanCur->flags |= TRANSLUCENT;
	//TEMPORARY SOLUTION
	pan_setpos(psInv->ppanCur, 1, 3, vector(psInv->ppanCur->size_x * psInv->ppanCur->scale_x + INV_PANELDIST, 
		psInv->ppanCur->size_y * psInv->ppanCur->scale_y -10, 0));
	pan_setpos(psInv->ppanCur, 1, 4, vector(psInv->ppanCur->size_x * psInv->ppanCur->scale_x + INV_PANELDIST + 20, 
		psInv->ppanCur->size_y * psInv->ppanCur->scale_y -10, 0));
 
	//TEMP
	psInv->ppanItem = pan_create(" ", vLayer + 1); 

	INVENTORY_updatePos(psInv);

	/* report error */
	#ifdef SYSMSG_ACTIVE
	if(psInv->pbmapBackdrop == NULL)
	{
		SYSMSG_print(SYSMSG_SHOW_ERROR, "INVENTORY_create: BMAP not found");
	} 
	#endif

	return psInv;
}


void INVENTORY_remove(INVENTORY* psInv)
{
	INVITEM* psItem;

	/* remove all inventory items from memory */
	while (psInv->psCurItem != NULL)
	{
		/* get next item in list - resolve items linked to themselves (last item in list) */
		if (psInv->psCurItem == (INVITEM*)psInv->psCurItem->psNext)
		{
			psItem = NULL; /* terminate loop */
		}
		else
		{
			psItem = (INVITEM*)psInv->psCurItem->psNext; /* take next item */
		}
		/* remove current item */
		INVITEM__remove(psInv->psCurItem);
		/* set current item to next item */
		psInv->psCurItem = psItem;
	}
	
	/* remove all inventory panels */
	ptr_remove(psInv->ppanCur);
	ptr_remove(psInv->ppanItem);

	sys_free(psInv);

	#ifdef SYSMSG_ACTIVE
	vInstInventory--;
	str_cpy(strMsg, "INVENTORY_remove: delete instance - ");
	str_for_num(strInst, vInstInventory); 
	str_cat(strMsg, strInst);
	SYSMSG_print(SYSMSG_SHOW_DEBUG, strMsg);
	#endif
}


void INVENTORY_show(INVENTORY* psInv)
{
	/* inventory already visible? */
	if (!INVENTORY_isVisible(psInv) && !psInv->vFadeActive)
	{
		psInv->vFadeActive = 1;
		psInv->ppanCur->alpha = 0;
//		psInv->ppanPrev->flags |= SHOW;
		psInv->ppanCur->flags  |= SHOW;
//		psInv->ppanNext->flags |= SHOW;

		while (psInv->ppanCur->alpha < INV_ALPHA)
		{
			wait (1);
			psInv->ppanCur->alpha += INV_FADESPEED * time_step;
		}
		psInv->ppanCur->alpha = INV_ALPHA;
		psInv->vFadeActive = 0;
	}
}


void INVENTORY_hide(INVENTORY* psInv)
{
	/* inventory already visible? */
	if (INVENTORY_isVisible(psInv) && !psInv->vFadeActive)
	{
		psInv->vFadeActive = 1;
		while (psInv->ppanCur->alpha > 0)
		{
			wait (1);
			psInv->ppanCur->alpha -= INV_FADESPEED * time_step;
		}
		psInv->ppanCur->alpha = 0;
		psInv->vFadeActive = 0;

//		psInv->ppanPrev->flags &= ~SHOW;
		psInv->ppanCur->flags  &= ~SHOW;
//		psInv->ppanNext->flags &= ~SHOW;
	}
}


var INVENTORY_isVisible(INVENTORY* psInv)
{
	var vRetval;
	
	if ((psInv->ppanCur->flags & SHOW) == SHOW)
	{
		vRetval = 1;
	}
	else
	{
		vRetval = 0;
	}
}

void INVENTORY_setPos(INVENTORY* psInv, var vX, var vY, var vAlign)
{
	psInv->vPosX = vX;
	psInv->vPosY = vY;
	psInv->vAlignment = vAlign;

	INVENTORY_updatePos(psInv);
}


void INVENTORY_setSize(INVENTORY* psInv, var vMaxItems, var vMaxWeight, var vMaxAmount)
{
	psInv->vMaxItems  = vMaxItems;
	psInv->vMaxWeight = vMaxWeight;
	psInv->vMaxAmount = vMaxAmount;
}


void INVENTORY_updatePos(INVENTORY* psInv)
{
	var vWidth;
	var vHeight;
	var vPosX;
	var vPosY;
	
	/* left or right aligned? */
	if ((psInv->vAlignment & INV_ALIGN_LEFT) == INV_ALIGN_LEFT)
	{
		vPosX = psInv->vPosX;
	}
	else
	{
		/* calculate total width */
		vWidth  = psInv->ppanCur->size_x  * psInv->ppanCur->scale_x;
		vWidth += 2 * INV_PANELDIST;

		vPosX = screen_size.x - vWidth - psInv->vPosX;
	}

	/* top or bottom aligned? */
	if ((psInv->vAlignment & INV_ALIGN_TOP) == INV_ALIGN_TOP)
	{
		vPosY = psInv->vPosY;
	}
	else
	{
		/* calculate total height */
		vHeight = psInv->ppanCur->size_y  * psInv->ppanCur->scale_y;
		vHeight += 2 * INV_PANELDIST;
		
		vPosY = screen_size.y - vHeight - psInv->vPosY;
	}

	/* position panels - x axis */
	psInv->ppanCur->pos_x  = vPosX + INV_PANELDIST;
	/* position panels - y axis */
	psInv->ppanCur->pos_y  = vPosY + INV_PANELDIST;
}


INVITEM* INVENTORY_getItem(INVENTORY* psInv)
{
	return psInv->psCurItem;
}


INVITEM* INVENTORY_searchItem(INVENTORY* psInv, long lItemId)
{
	INVITEM* psItem;
	INVITEM* psRetItem;
	var i;

	psItem    = psInv->psCurItem;
	psRetItem = NULL;

	/* only scan each item once */
	for(i = 0; i < psInv->vCurItems; i++)
	{
		if(psItem->lItemId == lItemId)
		{
			/* item found */
			psRetItem = psItem;
			break;	
		}
		/* get next item for follow-up loop */
		psItem = (INVITEM*)psItem->psNext;
	} 
	return psRetItem;
}


void INVENTORY_next(INVENTORY* psInv)
{
	if (psInv->psCurItem != NULL && INVENTORY_isVisible(psInv))
	{
		psInv->psCurItem = (INVITEM*)psInv->psCurItem->psNext;
		INVENTORY__update(psInv);
	}
}


void INVENTORY_prev(INVENTORY* psInv)
{
	if (psInv->psCurItem != NULL && INVENTORY_isVisible(psInv))
	{
		psInv->psCurItem = (INVITEM*)psInv->psCurItem->psPrev;
		INVENTORY__update(psInv);
	}
}


var INVENTORY_add(INVENTORY* psInv, INVITEM* psItem)
{
	INVITEM* psTemp;
	var vAmount;
	var vResult;
	var vNew;
	
	psTemp = INVENTORY_searchItem(psInv, psItem->lItemId);
	vResult = 0;
	vNew = 0;

	/* item is not yet in inventory or is not stackable */
	if (psTemp != NULL)
	{
		/* why Lite-C sucks - lesson #1 */
		if (psTemp->vStackable == 0)
		{
			vNew = 1;
		}
	}
	else
	{
		vNew = 1;
	}
	
	if (vNew != 0) 
//	if (psTemp == NULL || (psTemp != NULL && psTemp->vStackable == 0))
	{
		/* check if there is still room for new items */
		if (INVENTORY__checkItems(psInv) && INVENTORY__checkWeight(psInv, psItem) && INVENTORY__checkAmount(psInv, psItem->vAmount))
		{
			/* initialize linked list if this is the first item */
			if (psInv->psCurItem == NULL)
			{
				psInv->psCurItem = psItem;
				/* link item to itself */
				psInv->psCurItem->psNext = (void*)psItem;
				psInv->psCurItem->psPrev = (void*)psItem;
			}
			else
			{
				/* insert item at the 'end' */
				((INVITEM*)psInv->psCurItem->psPrev)->psNext = (void*)psItem;	/* update NEXT of previous item */
				psItem->psPrev = psInv->psCurItem->psPrev;							/* update PREV of new item */ 
				psInv->psCurItem->psPrev = (void*)psItem;								/* update PREV of current item */
				psItem->psNext = (void*)psInv->psCurItem;								/* update NEXT of new item */
			}
			psInv->vCurItems++;	
			vResult = 1; /* success */

		}
		/* item cannot be added */
		else
		{
			INVITEM__remove(psItem);
		}
	}
	/* item was already in inventory */
	else
	{
		/* take existing and new amount into account */
		vAmount = psTemp->vAmount + psItem->vAmount;
		if (INVENTORY__checkItems(psInv) && INVENTORY__checkWeight(psInv, psItem) && INVENTORY__checkAmount(psInv, vAmount))
		{
			/* add new amount to existing item */
			psTemp->vAmount = vAmount;
			/* INVITEM instance is not needed anymore */
			INVITEM__remove(psItem);
			vResult = 1;
		}		
		/* item cannot be added */
		else
		{
			INVITEM__remove(psItem);
		}
	}
	
	INVENTORY__updateWeight(psInv);
	INVENTORY__update(psInv);

	return vResult;
}


void INVENTORY_sub(INVENTORY* psInv)
{
	INVITEM* psItem;

	psItem = psInv->psCurItem;

	if (psItem != NULL)
	{
		psItem->vAmount--;
		if (psItem->vAmount == 0)
		{
			psInv->vCurItems--;
			/* any other items left in inventory? */
			if (psInv->vCurItems > 0)
			{
				/* set follow up item as current item */
				psInv->psCurItem = (INVITEM*)psItem->psNext;
			}
			else
			{
				/* no more items left */
				psInv->psCurItem = NULL;	
			}
		
			/* remove item from list */
			INVITEM__remove(psItem);
		}
		INVENTORY__updateWeight(psInv);
		INVENTORY__update(psInv);
	}
}



INVITEM* INVITEM_create(STRING* strBmap, long lItemId, var vWeight, var vAmount)
{
	INVITEM* psItem;
	
	/* allocate memory */
	psItem = (INVENTORY*)sys_malloc(sizeof(INVITEM));
	
	#ifdef SYSMSG_ACTIVE
	vInstInvitem++;
	str_cpy(strMsg, "INVITEM_create: new instance - ");
	str_for_num(strInst, vInstInvitem); 
	str_cat(strMsg, strInst);
	SYSMSG_print(SYSMSG_SHOW_DEBUG, strMsg);
	#endif
	
	/* initialize data */
	psItem->vWeight    = vWeight;
	psItem->vAmount    = vAmount;
	psItem->vStackable = 1;
	psItem->lItemId    = lItemId;	

	psItem->pbmapPicture = bmap_create(strBmap);
	psItem->psExt        = NULL;
	psItem->pfExtRemove  = NULL;
	
	/* pointers for linked list */
	psItem->psNext = NULL;
	psItem->psPrev = NULL;
	
	/* report error */
	#ifdef SYSMSG_ACTIVE
	if(psItem->pbmapPicture == NULL)
	{
		SYSMSG_print(SYSMSG_SHOW_ERROR, "INVITEM_create: BMAP not found");
	} 
	#endif

	return psItem;
}


void INVITEM_setExtension(INVITEM* psItem, void* psExt, void* pfExtRemove)
{
	/* opbject pointer */
	psItem->psExt = psExt;
	/* function pointer */
	psItem->pfExtRemove = pfExtRemove;
}


void INVITEM_setStackable(INVITEM* psItem, var vStackable)
{
	psItem->vStackable = vStackable;
}


var INVITEM_getAmount(INVITEM* psItem)
{
	return psItem->vAmount;
}


var INVITEM_getWeight(INVITEM* psItem)
{
	return psItem->vWeight;
}


/* ----- INTERNAL FUNCTIONS ----- */


var INVENTORY__checkAmount(INVENTORY* psInv, var vAmount)
{
	var vRetval;
	
	/* amount check always succeeds if no limit is set */
	if (psInv->vMaxAmount == INV_UNLIMITEDAMOUNT)
	{
		vRetval = 1; /* success */
	}
	else
	{
		if (vAmount <= psInv->vMaxAmount)
		{
			vRetval = 1; /* success */
		}
		else
		{
			vRetval = 0; /* no success */
		}
	}
	
	return vRetval;
}

var INVENTORY__checkItems(INVENTORY* psInv)
{
	var vRetval;
	
	if (  (psInv->vCurItems + 1 < psInv->vMaxItems)
		|| (psInv->vMaxItems == INV_UNLIMITEDITEMS) 
		)
	{
		vRetval = 1; /* success */
	}
	else
	{
		vRetval = 0; /* no success */
	}
	
	return vRetval;
}

var INVENTORY__checkWeight(INVENTORY* psInv, INVITEM* psItem)
{
	var vWeightLeft;
	var vNewWeight;
	var vRetval;
	
	/* weight check always succeeds if no limit is set */
	if (psInv->vMaxWeight == INV_UNLIMITEDWEIGHT)
	{
		vRetval = 1; /* success */
	}
	else
	{
		/* new weight takes amount of item into account */
		vNewWeight = psItem->vWeight * psItem->vAmount;
		vWeightLeft = psInv->vMaxWeight - psInv->vCurWeight;

		/* still enough room for extra load? */
		if (vWeightLeft >= vNewWeight)
		{
			vRetval = 1; /* success */
		}
		else
		{
			vRetval = 0; /* no success */
		}
	}
	
	return vRetval;
}

void INVENTORY__update(INVENTORY* psInv)
{
	INVITEM* psItem;

	/* refresh info variables for panel display */
	psItem = psInv->psCurItem;
	if (psItem != NULL)
	{
		psInv->vWeightCur = psItem->vWeight; 	
		psInv->vAmountCur = psItem->vAmount; 	
	}
	else
	{
		psInv->vWeightCur = 0; 	
		psInv->vAmountCur = 0; 	
	}
}

void INVENTORY__updateWeight(INVENTORY* psInv)
{
	INVITEM* psItem;
	var i;
	var vWeight;
	
	psItem = psInv->psCurItem;
	vWeight = 0;
	
	/* only scan each item once */
	for(i = 0; i < psInv->vCurItems; i++)
	{
		/* calculate total item weight */
		vWeight += psItem->vWeight * psItem->vAmount;
		
		/* get next item for follow-up loop */
		psItem = (INVITEM*)psItem->psNext;
	}
	
	psInv->vCurWeight = vWeight;
}


void INVITEM__remove(INVITEM* psItem)
{
	/* remove item image */
	bmap_remove(psItem->pbmapPicture);

	/* remove item from linked list */
	if (psItem->psPrev != NULL)
	{
		((INVITEM*)psItem->psPrev)->psNext = psItem->psNext;
	}

	if (psItem->psNext != NULL)
	{
		((INVITEM*)psItem->psNext)->psPrev = psItem->psPrev;
	}
	/* remove attached external object if any */
	if (psItem->pfExtRemove != NULL && psItem->psExt != NULL)
	{
		INVITEM_pfExtRemoval = psItem->pfExtRemove;
		INVITEM_pfExtRemoval(psItem->psExt);
	}
	
	/* free memory */
	sys_free(psItem);

	#ifdef SYSMSG_ACTIVE
	vInstInvitem--;
	str_cpy(strMsg, "INVITEM__remove: delete instance - ");
	str_for_num(strInst, vInstInvitem); 
	str_cat(strMsg, strInst);
	SYSMSG_print(SYSMSG_SHOW_DEBUG, strMsg);
	#endif
}

