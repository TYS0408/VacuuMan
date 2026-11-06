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
	const Vector3 EFFECT_SCALE = Vector3(70.0f, 70.0f, 70.0f);

	/** 確保エフェクトを床から離す位置*/
	constexpr float SECURE_EFFECT_HEIGHT = 20.0f;
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

	/** ステージエフェクトを作成*/
	m_stageEffect = NewGO<EffectEmitter>(0, "stageeffect");
	m_stageEffect->Init(static_cast<int>(EffectID::EffectID_StageDivision));
	m_stageEffect->SetPosition(EFFECT_POS);
	m_stageEffect->SetScale(EFFECT_SCALE);
	m_stageEffect->Play();
	m_stageEffect->Update();
	return true;
}


void Stage::Update()
{

	m_stageModelRender.Update();

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