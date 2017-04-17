Acknex Toolbox Project

Acknex/Lite-C script collection for useful standard tasks.
Compatible to Acknex7 and Acknex8.


Content Overview

Main include file for the toolbox project is toolbox.h. By setting defines before inclusion of this file, 
it is possible to enable only the required modules. 
By default, no modules at all are included in order to keep compile time small. 
Pick the modules you need and enable them using following defines:

TOOLBOX_USE_SYSMSG
sysmsg.h: System message module

TOOLBOX_USE_ACKLIB
acklib.h: General purpose functions and variables

TOOLBOX_USE_CONTROL
control.h: Key control setup module

TOOLBOX_USE_CAMCTRL
camctrl.h: Camera module

TOOLBOX_USE_INVENTORY
invent.h: Inventory module

TOOLBOX_USE_LIST
list.h: Extendable list module

TOOLBOX_USE_STATE
state.h: State manager module

TOOLBOX_USE_XMLREADER
xmlreader.h: XML parser module

TOOLBOX_USE_ANIMATE
animate.h: Animation module

It is also possible to include the header files of the toolbox modules directly. 



Documentation: open /doc/index.chm
Usage: check /samples/ folder


Project was migrated from SVN as is.
Please respect license.txt

-Firoball
http://firoball.de