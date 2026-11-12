#include "stdafx.h"
#include "EffectManager.h"

bool EffectManager::Start()
{
	LoadEffects();
	return true;
}

void EffectManager::LoadEffects()
{
	/** ステージ分割用エフェクト(右上)を読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision_UpperRight,
		u"Assets/Effect/Stage_Division_UpperRight.efk"
	);

	/** ステージ分割用エフェクト(左上)を読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision_UpperLeft,
		u"Assets/Effect/Stage_Division_UpperLeft.efk"
	);

	/** ステージ分割用エフェクト(右下)を読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision_LowerRight,
		u"Assets/Effect/Stage_Division_LowerRight.efk"
	);

	/** ステージ分割用エフェクト(左下)を読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageDivision_LowerLeft,
		u"Assets/Effect/Stage_Division_LowerLeft.efk"
	);

	/** ステージ確保用エフェクトを読み込む */
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_StageSecure,
		u"Assets/Effect/Stage_Secure.efk"
	);

	/** 掃除機吸い込みエフェクトを読み込む*/
	nsK2EngineLow::EffectEngine::GetInstance()->ResistEffect(
		(int)EffectID::EffectID_VaccumSuction,
		u"Assets/Effect/Vaccum_Suction.efk"
	);
}