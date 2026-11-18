#pragma once
#include"Source/Actor/Character/Character.h"
#include<vector>
#include<array>
class FakeTrash;
class Dog : public Character
{
public:
	Dog() {};
	~Dog();

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

private:
	/** 落としたごみを全て覚えておくための配列*/
	std::vector<FakeTrash*>m_fakeTrashList;
	/** 移動*/
	void Move()override;
	/** 回転*/
	void Rotation()override;
	/** 新しいランダムな目標地点を決める*/
	void DecideNextTarget();
	/** 範囲内のランダムなfloat値を返す*/
	float RandomRange(float minValue, float maxValue);
	/** アニメーション再生*/
	void PlayAnimation();
	/** ステート管理*/
	void ManageState();
	/** 偽ゴミを落とす処理*/
	void DropFakeTrash();
	/** 走っている間に行って間隔でゴミを落とす処理*/
	void UpdateDropFakeTrash();
	/** 区画をランダムに移動する処理*/
	void ShuffleAreaRandom();
	/** 区画をめぐる順番(これは4個で固定なのでarrayを使う)*/
	std::array<int, 4> m_areaRandom = { 0,1,2,3 };

	/** 指定のX,Zかの真上からレイを飛ばして、床かどうかを調べる*/
	bool IsFloorOnly(float x, float z)const;
	/** 目標地点として使えるか(周囲も含めて家具が無いかどうか)*/
	bool IsFreeFloor(float x, float z)const;
	/** 次に向かう順番の位置(終わった後もまたランダムにそれぞれの区画に向かうので4になる*/
	int m_areaRandomIndex = 4;
	/**直前に向かったエリア(シャッフル後に同じエリアが出ないようにする)*/
	int m_lastArea = -1;

private:
	ModelRender m_modelRender;

	//CharacterController m_characterController;
	/** 現在の位置*/
	Vector3 m_position = Vector3::Zero;
	/** 目標地点*/
	Vector3 m_tagetPos = Vector3::Zero;
	/** キャラコンに渡す移動速度*/
	Vector3 m_moveSpeed = Vector3::Zero;
	/** 進行方向*/
	Vector3 m_moveDir = Vector3::Zero;
	/** 回転*/
	Quaternion m_rotation = Quaternion::Identity;
	/** 到着後の待機時間*/
	float m_waitTimer = 0.0f;
	/** 次にゴミを落とすまでのタイマー*/
	float m_DropTimer = 0.0f;
	/** 引っかかっている時間*/
	float m_stuckTimer = 0.0f;
	/** 現在のステート*/
	uint8_t m_state = 0;
	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimatioinClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];
};

