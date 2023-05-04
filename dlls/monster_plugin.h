//
// monster_plugin.h
//

#ifndef MONSTER_PLUGIN_H
#define MONSTER_PLUGIN_H


typedef struct
{
   char *name;
   bool need_to_precache;
} monster_type_t;


class CMBaseMonster;

typedef struct
{
   int monster_index;
   edict_t *monster_pent;
   bool killed;
   int respawn_index;
   CMBaseMonster *pMonster;
} monster_t;

#define MAX_MONSTER_ENTS 200

extern monster_t monsters[MAX_MONSTER_ENTS];


#define MAX_MONSTER_COUNT 20

typedef struct {
   Vector origin;
   float angle_min, angle_max;
   float delay;
   unsigned char monster[MAX_MONSTER_COUNT];
   int monster_count;
   float respawn_time;
   bool need_to_respawn;
} monster_spawnpoint_t;

#define MAX_MONSTERS 100
extern monster_spawnpoint_t monster_spawnpoint[MAX_MONSTERS];

extern DLL_GLOBAL short g_sModelIndexFireball;// holds the index for the fireball
extern DLL_GLOBAL short g_sModelIndexSmoke;// holds the index for the smoke cloud
extern DLL_GLOBAL short g_sModelIndexWExplosion;// holds the index for the underwater explosion
extern DLL_GLOBAL short g_sModelIndexBubbles;// holds the index for the bubbles model
extern DLL_GLOBAL short g_sModelIndexBloodDrop;// holds the sprite index for the initial blood
extern DLL_GLOBAL short g_sModelIndexBloodSpray;// holds the sprite index for splattered blood
extern DLL_GLOBAL short g_sModelIndexLaser;// holds the index for the laser beam
extern DLL_GLOBAL const char *g_pModelNameLaser;
extern DLL_GLOBAL short g_sModelIndexLaserDot;// holds the index for the laser beam dot

#endif
