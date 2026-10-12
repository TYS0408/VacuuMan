#pragma once
#include"Source/Actor/Actor.h"
/** ゴミの種類*/
enum class EnTrashType
{
	enBottle,    /** ペットボトル*/
	enBox,       /** 段ボールボックス*/
	enPizzaBox,  /** ピザボックス*/
	enDrinkCan,  /** 飲料缶*/
	enFoodCan,   /** 食品缶*/
	enNum     /** ゴミの種類の数*/
};
class Trash : public Actor
{

public:
	Trash() {};
	~Trash() {};

public:

	bool Start ()override;
	void Update()override;
	void Render(RenderContext& rc)override;


	/** 種類と位置を設定する*/
	void Setup(EnTrashType type, const Vector3& pos)
	{
		m_type = type;
		m_position = pos;
	}


	/** 座標を取得*/
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	/** 座標を設定(モデルの座標も更新)*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_modelRender.SetPosition(m_position);
	}
	/** 拾われた時の綺麗さ*/
	int GetCleanPoint()const;

private:
	ModelRender m_modelRender;
	EnTrashType m_type = EnTrashType::enBottle;
	Vector3 m_position = Vector3::Zero;

};

