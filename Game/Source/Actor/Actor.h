/** Actor.h*/
/* 基底クラス*/
#pragma once
#include "Transform/Transform.h"
class Actor : public IGameObject
{
protected:
	/*
	*トランスフォームクラス
	*/
	Transform m_transform;

public:
	const Transform& GetTransform()const
	{
		return m_transform;
	}

	Actor();
	~Actor();

public:
	virtual bool Start()override;
	virtual void Update()override;
	virtual void Render(RenderContext& rc)override;
};
