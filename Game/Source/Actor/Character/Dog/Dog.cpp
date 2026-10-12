#include "stdafx.h"
#include "Dog.h"

namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/Dog/Dog.tkm";
	/** 初期座標*/
	Vector3 POS = Vector3(0.0f, 0.0f, -200.0f);

	/** ステージの移動可能範囲*/
	/** 床の範囲*/
	/** Xの最大範囲と最小範囲*/
	constexpr float AREA_MIN_X = -400.0f;
	constexpr float AREA_MAX_X = 400.0f;
	/** Xの最大範囲と最小範囲*/
	constexpr float AREA_MIN_Z = -800.0f;
	constexpr float AREA_MAX_Z = -100.0f;
	/** 一秒あたりの移動量*/
	constexpr float MOVE_SPEED = 600.0f;
	/** この距離まで言ったら到達したとみなす*/
	constexpr float ARRIVE_DIST = 5.0f;
	/** ランダム移動した後の場所を最低でも200話した場所を目標にする*/
	constexpr float MIN_TARGET_DIST = 200.0f;
	/** 決め直しの最大回数 */
	constexpr int MAX_RETRY = 10;
	/** 到着後に立ち止まる秒数*/
	constexpr float WAIT_TIME = 0.5f;




}
bool Dog::Start()
{
	m_position = POS;
	m_modelRender.Init(FILEPATH);
	m_modelRender.SetPosition(POS);
	m_modelRender.Update();

	DecideNextTarget();
	return true;
}


float Dog::RandomRange(float minValue, float maxValue)
{
	/** RAND_MAXは「32767」からランダムで出た値を「32767」で割っている。*/
	float t = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
	/** その割った値に0.0〜1.0 を好きな範囲に引き伸ばして、ずらしている*/
	return minValue + (maxValue - minValue) * t;
}

void Dog::DecideNextTarget()
{
	for (int i = 0; i < MAX_RETRY; i++)
	{
		m_tagetPos.x = RandomRange(AREA_MIN_X, AREA_MAX_X);
		/** Y軸は高さを変えないのでランダムにしない*/
		m_tagetPos.y = m_position.y;
		m_tagetPos.z = RandomRange(AREA_MIN_Z, AREA_MAX_Z);

	/** 現在位置から距離を調べる*/
	Vector3 diff = m_tagetPos - m_position;
	/** Y軸は考慮しない*/
	diff.y = 0.0f;

	/** 十分離れていれば、この目標地点で決定*/
	if (diff.Length() >= MIN_TARGET_DIST)
	{
		break;
	}
}
	/** 進行方向を計算*/
	m_moveDir = m_tagetPos - m_position;
	m_moveDir.y = 0.0f;
	m_moveDir.Normalize();
}

void Dog::Update()
{
	/** 移動*/
	Move();
	/** 回転*/
	Rotation();
	m_modelRender.SetPosition(m_position);

	m_modelRender.SetRotation(m_rotation);

	m_modelRender.Update();
}


void Dog::Move()
{
	/** 到着後の待機中*/
	if (m_waitTimer > 0.0f)
	{
		m_waitTimer -= g_gameTime->GetFrameDeltaTime();
		if (m_waitTimer <= 0.0f)
		{
			/** 到着後のタイムが0以下になったら次の目標地点に移動する*/
			DecideNextTarget();
		}
		return;
	}


	/** 目標までの距離*/
	Vector3 toTaget = m_tagetPos - m_position;
	toTaget.y = 0.0f;

	/** 到着判定*/
	/**目標までの距離を計算して目標に到着したら*/
	if (toTaget.Length() < ARRIVE_DIST)
	{
		/** 到着した後は３秒間制止する。*/
		if (WAIT_TIME > 0.0f)
		{
			m_waitTimer = WAIT_TIME;
		}
		/** 終わったらまた目標まで歩き出す。*/
		else
		{
			DecideNextTarget();
		}
		return;
	}

	/** 目標地点まで移動*/
	m_moveDir = toTaget;
	m_moveDir.Normalize();
	m_position += m_moveDir * MOVE_SPEED * g_gameTime->GetFrameDeltaTime();
}


void Dog::Rotation()
{
	/** 待機中は向きを変えない*/
	if (m_waitTimer > 0.0f)
	{
		return;
	}

	/** 進行方向を向く*/
	/** XとZの値から何ラジアン回転した向きかという角度を求める関数*/
	float angle = atan2f(m_moveDir.x, m_moveDir.z);
	angle += Math::PI;
	m_rotation.SetRotationY(angle);
}

void Dog::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
