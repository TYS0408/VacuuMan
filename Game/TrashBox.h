#pragma once
class TrashManager;
class TrashBox : public IGameObject
{
public:
	TrashBox() {};
	~TrashBox() {};

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

/** プレイヤーが捨てられる距離にいるか*/
	bool IsPlayerNear()const;
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	/** トラッシュマネージャー*/
	TrashManager* m_trashManager = nullptr;
};

