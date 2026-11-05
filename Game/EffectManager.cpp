#include "stdafx.h"
#include "EffectManager.h"

bool EffectManager::Start()
{
	LoadEffects();
	return true;
}

void EffectManager::LoadEffects()
{
	/** エフェクトを読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision,
		u"Assets/Effect/Stage_Division.efk"
	);
}