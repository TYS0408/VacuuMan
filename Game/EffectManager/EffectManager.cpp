#include "stdafx.h"
#include "EffectManager.h"

bool EffectManager::Start()
{
	LoadEffects();
	return true;
}

void EffectManager::LoadEffects()
{
	/** ステージ分割用エフェクトを読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision,
		u"Assets/Effect/Stage_Division.efk"
	);

	/** ステージ確保用エフェクトを読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageSecure,
		u"Assets/Effect/Stage_Secure.efk"
	);
}