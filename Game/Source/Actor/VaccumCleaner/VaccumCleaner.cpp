#include "stdafx.h"
#include "VaccumCleaner.h"
#include"EffectManager/EffectManager.h"

namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/VaccumCleaner/VaccumCleaner.tkm";
	Vector3 SCALE = Vector3(1.0f, 1.0f, 1.0f);
	/** 吸い込みエフェクトの大きさ*/
	const Vector3 SUCTIOM_EFFECT_SCALE = Vector3(20.0f, 10.0f, 20.0f);
}

VaccumCleaner::~VaccumCleaner()
{
	StopSuction();
}
bool VaccumCleaner::Start()
{
	m_modelRender.Init(FILEPATH);
	m_modelRender.SetScale(SCALE);
	m_modelRender.Update();

	
	return true;
}


void VaccumCleaner::StartSuction()
{
	if (m_isSucking)
	{
		return;
	}
	/** 吸い込みエフェクトを生成*/
	m_vaccumEffect = NewGO<EffectEmitter>(0, "vaccumsuction");
	m_vaccumEffect->Init(static_cast<int>(EffectID::EffectID_VaccumSuction));
	m_vaccumEffect->SetScale(SUCTIOM_EFFECT_SCALE);
	m_vaccumEffect->SetPosition(GetSuctionPosition());
	m_vaccumEffect->SetRotation(m_rotation);
	m_vaccumEffect->Play();
	m_vaccumEffect->Update();
	m_isSucking = true;
}

void VaccumCleaner::StopSuction()
{
	if (m_vaccumEffect != nullptr)
	{
		if (!m_vaccumEffect->IsDead())
		{
			m_vaccumEffect->Stop();
			m_vaccumEffect = nullptr;
		}
		
	}
	m_isSucking = false;
}


void VaccumCleaner::UpdateSuctionEffect()
{
	if (m_vaccumEffect == nullptr)
	{
		return;
	}

	/** エフェクトが勝手に消えていたら状態をもとに戻す*/
	if (m_vaccumEffect->IsDead())
	{
		m_vaccumEffect = nullptr;
		m_isSucking = false;
		return;
	}
	m_vaccumEffect->SetPosition(GetSuctionPosition());
	m_vaccumEffect->SetRotation(m_rotation);
}

void VaccumCleaner::Update()
{
	
	/** ポジションの更新*/
	m_modelRender.SetPosition(m_position);
	/** 回転の更新*/
	m_modelRender.SetRotation(m_rotation);

	/** モデルの更新*/
	m_modelRender.Update();
	/** 吸い込みエフェクトの更新*/
	UpdateSuctionEffect();

}

Vector3 VaccumCleaner::GetSuctionPosition()const
{
	Vector3 offset = m_nozzleOffset;
	/** 掃除機の向きに合わせて回転させる*/
	m_rotation.Apply(offset);
	/** 掃除機の位置を足す*/
	return m_position + offset;
}



Vector3 VaccumCleaner::GetSuctionDirection()const
{
	/** Z方向(奥方向に吸い込む)*/
	Vector3 dir = Vector3(0.0f, 0.0f, 1.0f);
	m_rotation.Apply(dir);
	return dir;
}

void VaccumCleaner::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
