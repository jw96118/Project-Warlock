#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#define LOAD_MAX 128
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

LOADINGID CLoading::Get_LoadingID(void) const
{
	return m_eLoading;
}

CRITICAL_SECTION* CLoading::Get_Crt(void)
{
	return &m_Crt;
}

_bool CLoading::Get_Finish(void) const
{
	return m_bFinish;
}

const _tchar* CLoading::Get_LoadString(void)
{
	return m_szLoading;
}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	int i = 0;

	float temp = 100.f;
	//int i = 0;
	if (m_pLoadCount != nullptr)
	{
		// buffer
		FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
			RESOURCE_STATIC,
			L"Buffer_TerrainTex",
			Engine::BUFFER_TERRAINTEX,
			VTXCNTX,
			VTXCNTZ,
			VTXITV),
			E_FAIL);


		FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
			RESOURCE_STATIC,
			L"Buffer_CubeTex",
			Engine::BUFFER_CUBETEX),
			E_FAIL);

		//lstrcpy(m_szLoading, L"Texture Loading.............................");



		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_LHUD",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/LHUD.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Iven",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Inventory.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_RHUD",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/RHUD.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_THUD",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/THUD.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_EXE",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Exe.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HP",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/hpbar.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Ammo",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/ammobar.png"),
			E_FAIL);

		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MP",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/mpbar.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Boss_Gauge",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Boss_Gauge.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		//Monster Textures
#pragma region MonsterTexture

#pragma region CrusaderTexture
	//Crusader Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Back",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Back/Crusader_Back%d.png", 1),
			E_FAIL);

		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Attack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Bash/Crusader_Bash%d.png", 14),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Hit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Hit/Crusader_Hit%d.png", 2),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Death/Crusader_Death%d.png", 16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Stand",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Stand/Crusader_Stand%d.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Crusader_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Crusader/Walk/Crusader_Walk%d.png", 6),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
#pragma endregion

#pragma region GlacierTexture
		//Glacier Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_Back",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Glacier/Back/Glacier_Back%d.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_Shoot",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Glacier/Shoot/Glacier_Shoot%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Glacier/Death/Glacier_Death%d.png", 9),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_HitMove",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Glacier/Hit/Glacier_Hit%d.png", 17),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_Move",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Glacier/Move/Glacier_Move%d.png", 16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
#pragma endregion 

#pragma region SpiderTexture
		//Spider Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Spider_Attack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Spider/Attack/Spider_Attack%d.png", 14),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Spider_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Spider/Death/Spider_Death%d.png", 14),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Spider_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Spider/Idle/Spider_Idle%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
#pragma endregion

#pragma region BatTexture
		//Bat Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Attack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Attack/batGreyattack000%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Back",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Back/batGreyFly_back000%d.png", 7),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Death/batGreydeath%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Fly/batGreyFly000%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Hit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Hit/batGreyHit_000%d.png", 2),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Shoot",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/BatGray/Shoot/batGreyShoot000%d.png", 5),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
#pragma endregion

#pragma region NewshamblerTexture

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Idle/Newshambler_Idle%d.png", 1),
			E_FAIL);

		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Bash",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Bash/Newshambler_Bash%d.png", 12),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Death/Newshambler_Death%d.png", 13),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_FirstCast",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/FirstCast/Newshambler_FirstCast%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_FirstHit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/FirstHit/Newshambler_FirstHit%d.png", 7),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_FirstSlash",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/FirstSlash/Newshambler_FirstSlash%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_FirstWalk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/FirstWalk/Newshambler_FirstWalk%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Magic",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Magic/Newshambler_Magic%d.png", 12),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_SecondChomp",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/SecondChomp/Newshambler_SecondChomp%d.png", 10),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_SecondHit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/SecondHit/Newshambler_SecondHit%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_SecondWalk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/SecondWalk/Newshambler_SecondWalk%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Slash",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Slash/Newshambler_Slash%d.png", 14),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Newshambler_Walk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/Newshambler/Walk/Newshambler_Walk%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

#pragma endregion 

#pragma region HellBossTexture

		//Dwarf Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Dwarf_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/DwarfIdle/HellBoss_Dwarf_Idle%d.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Dwarf_Walk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/DwarfWalk/HellBoss_Dwarf_Walk%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Dwarf_Shoot",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/DwarfShoot/HellBoss_Dwarf_Shoot%d.png", 26),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Dwarf_EyeBlast",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/DwarfEyeBlast/HellBoss_Dwarf_EyeBlast%d.png", 21),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Dwarf_Morph",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/DwarfMorph/HellBoss_Dwarf_Morph%d.png", 31),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		//Big Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_Walk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigWalk/HellBoss_Big_Walk%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_AttackStart",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigAttackStart/HellBoss_Big_AttackStart%d.png", 5),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_AttackSpin",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigAttackSpin/HellBoss_Big_AttackSpin%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_AttackFire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigAttackFire/HellBoss_Big_AttackFire%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_AttackEnd",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigAttackEnd/HellBoss_Big_AttackEnd%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		//Big First Texture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_FirstWalk",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigFirstWalk/HellBoss_Big_FirstWalk%d.png", 7),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_FirstHit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigFirstHit/HellBoss_Big_FirstHit%d.png", 6),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_FirstAttack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigFirstAttack/HellBoss_Big_FirstAttack%d.png", 14),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Big_Morph",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/BigMorph/HellBoss_Big_Morph%d.png", 65),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		//Caco
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Caco_Float",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/CacoWalk/HellBoss_Caco_Float%d.png", 9),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Caco_Nova",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/CacoNova/HellBoss_Caco_Nova%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Caco_EyeLaser",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/CacoEyeLaser/HellBoss_Caco_EyeLaser%d.png", 11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Caco_Attack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/CacoAttack/HellBoss_Caco_Attack%d.png", 12),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Caco_Morph",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/CacoMorph/HellBoss_Caco_Morph%d.png", 43),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		//Final
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Final_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/FinalIdle/HellBoss_Final_Idle%d.png", 20),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_HellBoss_Final_Death",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/HellBoss/FinalDeath/HellBoss_Final_Death%d.png", 26),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		//MiniGame
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MiniMonster",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Monster/MiniGame/yeti_run000%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
#pragma endregion 

#pragma endregion



		//Projectile Textures
#pragma region ProjectileTexture
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Bat_Spit",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/Bat/Bat_Spit%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Glacier_Bullet",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/Glacier/Glacier_Bullet%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Ice_Spike1",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/IceSpike1/IceSpike1_%d.png", 18),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Ice_Spike2",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/IceSpike2/IceSpike2_%d.png", 16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Ice_Spike3",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/IceSpike3/IceSpike3_%d.png", 18),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_BigRocket",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/BigRocket/BigRocket.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Electric",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/Electric/Electric%d.png", 11),
			E_FAIL);
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Eater",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Projectile/Eater/Eater%d.png", 16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


#pragma endregion 



		//
		// ÅØ½ºÃÄ

		//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		//											RESOURCE_STAGE,
		//											L"Texture_Terrain",
		//											Engine::TEX_NORMAL,
		//											L"../Bin/Resource/Texture/Floor/TEXTURE_FLOOR_18.png"),
		//											E_FAIL);


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Cube",
			Engine::TEX_CUBE,
			L"../Bin/Resource/Texture/SkyBox/burger%d.dds",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Player",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Test/Player.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Dagger Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Dagger_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Dagger/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		// Dagger Stab
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Dagger_Stab",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Dagger/Stab/Stab%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Dagger Throw
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Dagger_Throw",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Dagger/Throw/Throw%d.png",
			12),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Staff Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Staff_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Staff/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Staff Fire
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Staff_Fire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Staff/Fire/Fire%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Staff Charge
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Staff_Charge",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Staff/Charge/Charge%d.png",
			16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Staff ChargeLoop
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Staff_ChargeLoop",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Staff/ChargeLoop/ChargeLoop%d.png",
			10),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Staff ChargeRelease
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Staff_ChargeRelease",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Staff/ChargeRelease/ChargeRelease%d.png",
			5),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		// Revolver Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Revolver_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Revolver/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Revolver Shot
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Revolver_Shot",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Revolver/Shot/Shot%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow Fire
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow_Fire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow/Fire/Fire%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow Reload
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow_Reload",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow/Reload/Reload%d.png",
			5),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow3 1Arrow
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow3_1Arrow",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow3/1Arrow/1Arrow%d.png",
			2),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow3 2Arrow
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow3_2Arrow",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow3/2Arrow/2Arrow%d.png",
			3),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow3 3Arrow
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow3_3Arrow",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow3/3Arrow/3Arrow%d.png",
			3),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// CrossBow3 3Arrow
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_CrossBow3_Reload",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/CrossBow3/Reload/Reload%d.png",
			7),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// ColdStaff Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_ColdStaff_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/ColdStaff/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// ColdStaff Attack
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_ColdStaff_Attack",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/ColdStaff/Attack/Attack%d.png",
			16),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun Belt
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_Belt",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/Belt/Belt%d.png",
			3),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun Fire
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_Fire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/Fire/Fire%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun Rotate
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_Rotate",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/Rotate/Rotate%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun TurretMode
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_TurretMode",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/TurretMode/TurretMode.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun TurretModeFire
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_TurretModeFire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/TurretModeFire/TurretModeFire%d.png",
			8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// Minigun TurretModeIdle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Minigun_TurretModeIdle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/Minigun/TurretModeIdle/TurretModeIdle%d.png",
			8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// RocketLauncher Idle
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_RocketLauncher_Idle",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/RocketLauncher/Idle/Idle.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		// RocketLauncher Fire
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_RocketLauncher_Fire",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/Texture/Weapon/RocketLauncher/Fire/Fire%d.png",
			6),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		//TempObject
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Terrain",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Terrain/Terrain%d.png", 21),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Wall",
			Engine::TEX_NORMAL,
			L"../Bin/Resource/ProjectWarlock/Texture/Wall/Wall%d.png", 19), E_FAIL);

		*m_pLoadCount += temp / LOAD_MAX;

		//Item
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Hp",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Hp/TEXTURE_HEALTH_BIG_000%d.png", 4),
			E_FAIL);

		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Mana",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Mana/city_mana_big%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Mini",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/MiniGame/upgrade_pickup000%d.png", 7),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Portal",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Portal/wormhole000%d.png", 4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Upgrade_Point",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Upgrade_Point/Upgrade_Point%d.png", 8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Arrow_Ammo_Box",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Ammo/Arrow_Ammo_Box.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_MiniGun_Ammo_Box",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Ammo/MiniGun_Ammo_Box.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Pistol_Ammo_Box",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Ammo/Pistol_Ammo_Box.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Item_Rocket_Ammo_Box",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Ammo/Rocket_Ammo_Box.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_EventDoor",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/ProjectWarlock/Texture/EventDoor/TEXTURE_CELLDOOR_GREY.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		//MiniMap

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MiniWall",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/MiniMap/MiniWall.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MiniMapBack",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/MiniMap/MiniMapBack.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MiniPlayer",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/MiniMap/MiniPlayer.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;
		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_MiniItem",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/MiniMap/MiniItem.png", 1),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_DaggerThrow",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/DaggerThrow.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_Arrow",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/Arrow.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_SpellSphere",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/SpellSphere/SpellSphere%d.png",
			11),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_Explosion",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/Explosion/Explosion%d.png",
			13),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_Shock",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/Shock/Shock%d.png",
			4),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Effect_Blood",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Effect/Blood/Blood%d.png",
			8),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/Shop.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_Slot_Selected",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/Slot_Selected.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_UpgradeButton_Available",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/UpgradeButton_Available.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;


		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_UpgradeButton_Pressed",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/UpgradeButton_Pressed.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_ColdStaff_HUD",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/ColdStaff_HUD.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_CrossBow3_HUD",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/CrossBow3_HUD.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;

		FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
			RESOURCE_STAGE,
			L"Texture_Shop_Slot_Upgrade",
			Engine::TEX_NORMAL,
			L"../../Client/Bin/Resource/Texture/Shop/Slot_Upgrade.png"),
			E_FAIL);
		*m_pLoadCount += temp / LOAD_MAX;



		//lstrcpy(m_szLoading, L"Loading Complete!!!");

		m_bFinish = true;
	}


	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading, float* pLoadCount)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);
	pInstance->m_pLoadCount = pLoadCount;

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);
	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

