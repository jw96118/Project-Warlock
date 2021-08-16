#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	// ID_DYNAMIC : �ǽð� ������Ʈ�� �ʿ��� ������Ʈ
	enum COMPONENTID	{ ID_DYNAMIC, ID_STATIC, ID_END };

	enum WINMODE		{ MODE_FULL, MODE_WIN };

	enum INFO			{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION		{ ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum RESOURCETYPE	{ RESOURCE_STATIC, RESOURCE_STAGE, RESOURCE_END	};

	enum BUFFERID		{ BUFFER_TRICOL, BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_SCROLLTEX , BUFFER_TOOLTEX, BUFFER_LIGHTTEX};
	enum TEXTURETYPE	{ TEX_NORMAL, TEX_CUBE, TEX_END };
	
	enum RENDERID		{ RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
	 
	enum COLLTYPE		{ COL_FALSE, COL_TRUE, COL_END };
	enum MESHTYPE		{ TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI };

	enum ANGLE			{ ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END };
	enum MOUSEKEYSTATE	{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum PROJECTIONTYPE	{ PROJ_PERSPECTIVE, PROJ_ORTHOGRAPH, PROJ_END};

	enum OBJECTTYPE		{ OBJ_TERRAIN, OBJ_WALL, OBJ_PLAYER, OBJ_MONSTER, OBJ_ITEM, OBJ_END };
	enum MONSTERTYPE	{ NO_MONSTER, MONSTER_BAT, MONSTER_SPIDER, MONSTER_SHIELD, MONSTER_ICE, MONSTER_MIDDLEBOSS, MONSTER_BOSS, MONSTER_END };
	enum ITEMTYPE		{NO_ITEM, ITEM_HP, ITEM_MANA, ITEM_MINIGAME, ITEM_UPGRADE, ITEM_PORTAL, ITEM_AMMO,
							ITEM_AMMO_MINIGUN, ITEM_AMMO_ARROW, ITEM_AMMO_ROCKET, ITEM_END};

	enum CHANNELID		{ BGM, PLAYER, MONSTER, WEAPON, EFFECT, MAX_CHANNEL };

}

#endif // Engine_Enum_h__