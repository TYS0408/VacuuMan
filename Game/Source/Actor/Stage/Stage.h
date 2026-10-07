#pragma once
#include"Source/Actor/Actor.h"
class Stage : public Actor
{
public:
	Stage() {};
	~Stage() {};

public:
	virtual bool Start()override;
	virtual void Update()override;
	virtual void Render(RenderContext& rc)override;

private:
	/** ステージのモデル*/
	ModelRender m_stageModelRender;
	/** ステージの物理オブジェクト*/
	PhysicsStaticObject m_stagePhysicsObject;
	
};



