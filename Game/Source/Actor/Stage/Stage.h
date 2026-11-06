#pragma once
#include"Source/Actor/Actor.h"
class Stage : public Actor
{
public:
	Stage() {};
	~Stage();

public:
	virtual bool Start()override;
	virtual void Update()override;
	virtual void Render(RenderContext& rc)override;

	void PlayStageSecureEffect(const Vector3& areaCenter,const Vector3&scale);

private:
	/** ステージのモデル*/
	ModelRender m_stageModelRender;
	/** ステージの物理オブジェクト*/
	PhysicsStaticObject m_stagePhysicsObject;
	/** ステージのエフェクト*/
	EffectEmitter* m_stageEffect = nullptr;
};



