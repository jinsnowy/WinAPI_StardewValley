#pragma once
#include <vector>
#include "Vec2.h"
#include "../framework.h"

typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Pixel;

enum COLL_CHANNEL
{
	CO_OBJECT,
	CO_TILE,
	CO_PLAYER,
	CO_ITEM,
	CO_UI,
	CO_MOUSE,
	CO_ALL,
};

enum OBJ_TYPE 
{
	OBJ_NORMAL = 0,
	OBJ_TILE,
	OBJ_TREE,
	OBJ_ROCK,
	OBJ_GRASS,
	OBJ_TREETRUNK,
};

enum PR_TYPE
{
	PR_OUTDOOR,
	PR_END
};

enum EDIT_MODE
{
	SEL_GROUND = 0,
	SEL_STATIC,
	SEL_TILEOBJECT,
	SEL_OBJECT,
	SEL_OPTION,
	SEL_END
};

enum STAGE_SHOW {
	SHOW_NORMAL,
	SHOW_TILEOPTION,
	SHOW_GRID,
	SHOW_COLL,
};

enum STAGE_TAG
{
	ST_GROUND = 0,
	ST_STATIC,
	ST_END
};

enum BEACON_TAG
{
	BC_NONE,
	BC_ONE,
	BC_TWO,
	BC_THREE
};

enum SOUND_CHANNEL
{
	SD_BACKGROUND,
	SD_MAIN,
	SD_EFFECT,
	SD_END
};

// Scene Type
enum SCENE_CREATE
{
	SC_NONE,
	SC_START,
	SC_MAPEDIT,
	SC_INHOUSE,
	SC_FARM,
	SC_MIDDLEWAY,
	SC_TOWN,
	SC_END
};

// Tile Option
enum TILE_OPTION
{
	TO_NONE,
	TO_NOMOVE,
	TO_CROP_GROUND,
	TO_BEACON_1,
	TO_BEACON_2,
	TO_BEACON_3,
	TO_END
};

// Move dir type
enum MOVE_DIR
{
	MD_BACK = -1,
	MD_NONE,
	MD_FRONT
};

// Collider Type
enum COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_LINE,
	CT_POINT,
	CT_PIXEL,
	CT_END
};

// Collision State
enum COLLISION_STATE
{
	CS_ENTER,
	CS_STAY,
	CS_LEAVE,
	CS_END
};

// Animation Type
enum ANIMATION_TYPE
{
	AT_ATLAS,
	AT_FRAME,
	AT_END
};

// Animation Option
enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_RETURN,  // 한 번 돌고 기본 모션으로 돌아감
	AO_ONCE_DESTROY, // 한 번 돌고 오브젝트 삭제 (이펙트)
	AO_TIME_RETURN,  // (버프 같은) 타임 오버시 기본 모션으로 돌아감 
	AO_TIME_DESTROY  // 타임 오버시 오브젝트 삭제
};

enum DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	CENTER,
};

struct SceneState {
	SCENE_CREATE nextScene = SC_NONE;
	BEACON_TAG nextBeacon = BC_NONE;
	DIRECTION nextDir = RIGHT;
};
