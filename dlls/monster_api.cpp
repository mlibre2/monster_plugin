//
// botman's monster - MetaMOD plugin
//
// monster_api.cpp
//

/*
 *    This is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    This is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this code; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include "extdll.h"
#include "meta_api.h"
#include "sdk_util.h"      // UTIL_LogPrintf, etc

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
   NULL,         // pfnGetEntityAPI            HL SDK; called before game DLL
   NULL,         // pfnGetEntityAPI_Post       META; called after game DLL
   GetEntityAPI2,   // pfnGetEntityAPI2        HL SDK2; called before game DLL
   GetEntityAPI2_Post, // pfnGetEntityAPI2_Post  META; called after game DLL
   NULL,         // pfnGetNewDLLFunctions   HL SDK2; called before game DLL
   NULL,         // pfnGetNewDLLFunctions_Post META; called after game DLL
	NULL,         // pfnGetEngineFunctions	META; called before HL engine
   NULL,         // pfnGetEngineFunctions_Post META; called after HL engine
};

// Description of plugin
plugin_info_t Plugin_info = {
   META_INTERFACE_VERSION,                // interface version
   "Monster",                             // name
   "3.00.00",                             // version
   "06/30/2002",                          // date
   "botman <botman@planethalflife.com>",  // author
   "http://planethalflife.com/botman/",   // url
   "MONSTER",                             // logtag
   PT_CHANGELEVEL,                        // (when) loadable
   PT_CHANGELEVEL,                        // (when) unloadable
};

char *VNAME=Plugin_info.name;
char *VVERSION=Plugin_info.version;
char *VDATE=Plugin_info.date;
char *VAUTHOR=Plugin_info.author;
char *VURL=Plugin_info.url;
char *VLOGTAG=Plugin_info.logtag;
char *COMPILE_TIME=__DATE__ ", " __TIME__;

// Global vars from metamod:
meta_globals_t *gpMetaGlobals;      // metamod globals
gamedll_funcs_t *gpGamedllFuncs;    // gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;     // metamod utility functions

cvar_t init_dllapi_log = {"monster_log", "0", FCVAR_EXTDLL, 0, NULL};
cvar_t *dllapi_log = NULL;
cvar_t init_monster_spawn = {"monster_spawn", "1", FCVAR_EXTDLL, 0, NULL};
cvar_t *monster_spawn = NULL;


// Metamod requesting info about this plugin:
//  ifvers         (given) interface_version metamod is using
//  pPlugInfo      (requested) struct with info about plugin
//  pMetaUtilFuncs   (given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query(char *ifvers, plugin_info_t **pPlugInfo,
      mutil_funcs_t *pMetaUtilFuncs) 
{
   if(ifvers);   // to satisfy gcc -Wunused
   // Give metamod our plugin_info struct
   *pPlugInfo=&Plugin_info;
   // Get metamod utility function table.
   gpMetaUtilFuncs=pMetaUtilFuncs;
   return(TRUE);
}

// Metamod attaching plugin to the server.
//  now            (given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable   (requested) table of function tables this plugin catches
//  pMGlobals      (given) global vars from metamod
//  pGamedllFuncs   (given) copy of function tables from game dll
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, 
      meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs) 
{
   if(now);   // to satisfy gcc -Wunused
   if(!pMGlobals) {
      LOG_ERROR(PLID, "Meta_Attach called with null pMGlobals");
      return(FALSE);
   }
   gpMetaGlobals=pMGlobals;
   if(!pFunctionTable) {
      LOG_ERROR(PLID, "Meta_Attach called with null pFunctionTable");
      return(FALSE);
   }
   memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
   gpGamedllFuncs=pGamedllFuncs;

   LOG_MESSAGE(PLID, "%s v%s, %s", VNAME, VVERSION, VDATE);
   LOG_MESSAGE(PLID, "by %s", VAUTHOR);
   LOG_MESSAGE(PLID, "   %s", VURL);
   LOG_MESSAGE(PLID, "compiled: %s CDT", COMPILE_TIME);

   LOG_CONSOLE(PLID, "[%s] %s v%s, %s", VLOGTAG, VNAME, VVERSION, VDATE);
   LOG_CONSOLE(PLID, "[%s] by %s", VLOGTAG, VAUTHOR);

   CVAR_REGISTER(&init_dllapi_log);
   dllapi_log = CVAR_GET_POINTER("monster_log");

   CVAR_REGISTER(&init_monster_spawn);
   monster_spawn = CVAR_GET_POINTER("monster_spawn");

   return(TRUE);
}


extern void monster_unload(void);

// Metamod detaching plugin from the server.
// now      (given) current phase, ie during map, etc
// reason   (given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason) {
   // remove all the monsters currently in the level...
   monster_unload();
   return(TRUE);
}

