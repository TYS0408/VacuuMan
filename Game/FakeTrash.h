#pragma once
class FakeTrash : public IGameObject
{
public:
	FakeTrash() {};
	~FakeTrash() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	
private:
	ModelRender m_modelRender;
	/** ゴミの位置*/
	Vector3 m_position = Vector3::Zero;
	/** 回転*/
	Quaternion m_rotation = Quaternion::Identity;
};

