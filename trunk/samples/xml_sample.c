/* This short script shows usage of the XMLFILE object. */
/* See xmlreader.h for a short description of all functions. */
/* Don't modify XMLFILE parameters directly! use at own risk! */
/* - Firoball */


#include <acknex.h>
#include <default.c>


#include "..\xmlreader\xmlreader.h"

void main ()
{
	video_mode = 7; 
	screen_color.blue = 150;


	STRING* strT = str_create(""); 
	STRING* strC = str_create("");
	XMLPAR *pParHndl, *pPar;
	XMLATTRIB * pAttrib;
	XMLFILE* pXml = XMLFILE_create("test.xml");

	pParHndl = XMLFILE_parse(pXml);

	if (pParHndl != NULL)
	{
		/* get XMLPAR by Tag name */
		pPar = XMLPAR_getElementByTag(pParHndl, "LastTag");
	
		/* copy properties into strings and display them */
		XMLPAR_getTag(pPar, strT);
		XMLPAR_getContent(pPar, strC);
		printf("Copy access: %s -> %s", strT->chars, strC->chars);

	
		/* get XMLPAR by Tag name */
		pPar = XMLPAR_getElementByTag(pParHndl, "NextTag");
		/* get XMLPAR by Tag index (sub par of pPar) */
		pPar = XMLPAR_getElementByIndex(pPar, 1);
	
		/* get pointers to properties and display them */
		printf("Ptr access: %s -> %s", (XMLPAR_getPTag(pPar))->chars, (XMLPAR_getPContent(pPar))->chars);

		
		/* get XMLATTRIB by attribute name */
		pPar = XMLPAR_getElementByTag(pParHndl, "NextTag");
		pAttrib = XMLATTRIB_getElementByAttribute(pPar, "nextattrib");

		/* copy properties into strings and display them */
		XMLATTRIB_getAttribute(pAttrib, strT);
		XMLATTRIB_getContent(pAttrib, strC);
		printf("Copy access: %s -> %s", strT->chars, strC->chars);


		/* get XMLATTRIB by attribute index */
		pAttrib = XMLATTRIB_getElementByIndex(pPar, 0);
		
		/* get pointers to properties and display them */
		printf("Ptr access: %s -> %s", (XMLATTRIB_getPAttribute(pAttrib))->chars, (XMLATTRIB_getPContent(pAttrib))->chars);
	}	
	XMLFILE_remove(pXml);


	while(1)
	{
		wait(1);
	}	
}