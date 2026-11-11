#include "stdafx.h"
#include "Stage.h"
#include"EffectManager/EffectManager.h"
namespace
{
	/** ステージのファイルパス*/
	const char* STAGE_FILE_PATH = "Assets/modelData/Stage/Stage_β.tkm";

	/** ステージの座標*/
	Vector3 STAGE_POS = Vector3(0.0f, 0.0f, 0.0f);

	/** エフェクトの位置(床の中心、少し浮かせる)*/
	const Vector3 EFFECT_POS = Vector3(0.0f, 20.0f, 400.0f);

	/** ステージ分割エフェクトの大きさ*/
	//const Vector3 EFFECT_SCALE = Vector3(50.0f, 60.0f, 60.0f);

	/** 確保エフェクトを床から離す位置*/
	constexpr float SECURE_EFFECT_HEIGHT = 20.0f;

	const EffectID DIVISION_EFFECT_IDS[] =
	{
		EffectID::EffectID_StageDivision_UpperLeft,
		EffectID::EffectID_StageDivision_UpperRight,
		EffectID::EffectID_StageDivision_LowerLeft,
		EffectID::EffectID_StageDivision_LowerRight,
	};
}

Stage::~Stage()
{
	if (m_stageEffect)
	{
		if (!m_stageEffect->IsDead())
		{
			m_stageEffect->Stop();
			DeleteGO(m_stageEffect);
		}

		m_stageEffect = nullptr;
	}
}

bool Stage::Start()
{
	//ステージのモデルをロード。
	m_stageModelRender.Init(STAGE_FILE_PATH);

	/** ステージの位置を設定 */
	m_stageModelRender.SetPosition(STAGE_POS);

	m_stageModelRender.Update();
	/** 当たり判定*/
	m_stagePhysicsObject.CreateFromModel(
	m_stageModelRender.GetModel(),
	m_stageModelRender.GetModel().GetWorldMatrix());

	return true;
}


void Stage::Update()
{

	m_stageModelRender.Update();

}

void Stage::ShowStageDivisionEffect(int areaIndex,const Vector3&offset,const Vector3&scale)
{
	/** 同じ区画なら何もしない*/
	if (areaIndex == m_currentDivisionArea)
	{
		return;
	}

	/** 前の区画のエフェクトを削除*/
	if(m_stageEffect !=nullptr)
		if (!m_stageEffect->IsDead())
		{
			m_stageEffect->Stop();
			DeleteGO(m_stageEffect);
		}

	m_stageEffect = nullptr;

	/** 新しい区画のエフェクトを出す*/
	m_stageEffect = NewGO<EffectEmitter>(0, "stageeffect");
	m_stageEffect->Init(static_cast<int>(DIVISION_EFFECT_IDS[areaIndex]));
	m_stageEffect->SetPosition(EFFECT_POS + offset);
	m_stageEffect->SetScale(scale);
	m_stageEffect->Play();
	m_stageEffect->Update();

	m_currentDivisionArea = areaIndex;
}


void Stage::PlayStageSecureEffect(const Vector3& areaCenter,const Vector3&scale)
{
	EffectEmitter * effect = NewGO<EffectEmitter>(0, "secureeffect");
	effect->Init(static_cast<int>(EffectID::EffectID_StageSecure));
	/** 確保した場所にエフェクトを出す*/
	Vector3 pos = areaCenter;
	pos.y += SECURE_EFFECT_HEIGHT;
	effect->SetPosition(pos);
	effect->SetScale(scale);
	effect->Play();
	effect->Update();
}

void Stage::Render(RenderContext& rc)
{
	m_stageModelRender.Draw(rc);
}