/* This short script shows usage of the LIST object. */
/* See list.h for a short description of all functions. */
/* Don't modify LIST parmeters directly! use at own risk! */
/* - Firoball */

#include <acknex.h>
#include <default.c>

#define PRAGMA_PATH ".."
#define TOOLBOX_USE_LIST
#define TOOLBOX_USE_SYSMSG
#include "toolbox.h"

STRING* str = "#3000";
TEXT* debug_txt =
{
	pos_x = 0;
	pos_y = 0;
	string (str);
	flags = VISIBLE;
}


void main ()
{
	video_mode = 7; 
	screen_color.blue = 150;
	int i;
	int *pSrc, *pTarget;
	LIST* pTestList;
	
	SYSMSG_create();
	pTestList = LIST_create();


	//store data
	for (i = 0; i < 50; i++)
	{
		pSrc = malloc(sizeof(int));
		*pSrc = i * 3;	//add something

		/* add item to list - must be casted to (void*)*/
		LIST_append(pTestList, (void*)pSrc);		
	}

	str_cpy(str, "");

	//read data
	for (i = 0; i < LIST_items(pTestList); i++)
	{
		/* retrieve item from list - don't forget to cast to correct datatype */
		pTarget = (int*)LIST_getItem(pTestList, i);
		
		str_cat_num(str, "%lf\n", *pTarget);
	}

	/* remove list */
	LIST_remove(pTestList);

	/* remove list and stored objects - BE CAREFUL with engine objects and structs!! */
	//LIST_removeAll(pTestList);
	
	while(key_esc == 0)
	{
		wait(1);
	}
	SYSMSG_remove();
}