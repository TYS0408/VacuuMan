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
	/**ステージ確保用のエフェクトを再生する関数 */
	void PlayStageSecureEffect(const Vector3& areaCenter,const Vector3&scale);

	/** 分割エフェクトを表示*/
	void ShowStageDivisionEffect(int areaIndex,const Vector3&offset,const Vector3&scale);

private:
	/** ステージのモデル*/
	ModelRender m_stageModelRender;
	/** ステージの物理オブジェクト*/
	PhysicsStaticObject m_stagePhysicsObject;
	/** ステージのエフェクト*/
	EffectEmitter* m_stageEffect = nullptr;
	/** 現在表示しているエリア*/
	int m_currentDivisionArea = 01;
};



