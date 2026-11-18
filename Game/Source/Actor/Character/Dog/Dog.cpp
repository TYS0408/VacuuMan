#include "stdafx.h"
#include "Dog.h"
#include"FakeTrash.h"

namespace
{
	/** ファイルパス*/
	const char* FILEPATH = "Assets/modelData/Dog/Dog.tkm";

	/** アニメーションのファイルパス*/
	const char* ANIM_IDLE_PATH = "Assets/animData/Dog/Idle.tka";
	const char* ANIM_RUN_PATH = "Assets/animData/Dog/Run.tka";
	/** 初期座標*/
	Vector3 POS = Vector3(0.0f, 0.0f, 0.0f);

	/** 大きさ*/
	Vector3 Scale = { 2.0f,2.0f,2.0f };
	/** キャラコンの大きさ*/
	constexpr float COLLISIOM_W = 50.0f;
	constexpr float COLLISIOM_H = 25.0f;
	/** ステージの移動可能範囲*/
	/** 床の範囲*/
	/** Xの最大範囲と最小範囲*/
	constexpr float AREA_MIN_X = -400.0f;
	constexpr float AREA_MAX_X = 400.0f;
	/** Xの最大範囲と最小範囲*/
	constexpr float AREA_MIN_Z = -100.0f;
	constexpr float AREA_MAX_Z = 800.0f;
	/** 区画の分割線(TrashManagerと同じ)*/
	constexpr float DIVIDE_X = -20.0f;
	constexpr float DIVIDE_Z = 400.0f;
	/** 分割線から離す距離*/
	constexpr float DIVIDE_MARGIN = 50.0f;
	/** 犬が向かう区画範囲*/
	struct DogAreaInfo
	{
		float minX;
		float maxX;
		float minZ;
		float maxZ;
	};
	constexpr int DOG_AREA_NUM = 4;
	/** 順番は左上、右上、左下、右下の順番*/
	constexpr DogAreaInfo DOG_AREAS[DOG_AREA_NUM]
	{
		/** 「左上」minX = -400,maxX = -70,minZ = 450,maxZ = 800*/
	   { AREA_MIN_X, DIVIDE_X - DIVIDE_MARGIN, DIVIDE_Z + DIVIDE_MARGIN, AREA_MAX_Z },
	   /** 「右上」minX = 30,maxX = 400,minZ = 450,maxZ = 800*/
	   { DIVIDE_X + DIVIDE_MARGIN, AREA_MAX_X, DIVIDE_Z + DIVIDE_MARGIN, AREA_MAX_Z },
	   /** 「左下」minX = -400,maxX = 30,minZ = -100,maxZ = 350*/
	   { AREA_MIN_X, DIVIDE_X - DIVIDE_MARGIN, AREA_MIN_Z, DIVIDE_Z - DIVIDE_MARGIN },
	   /**「右下」 minX = 30,maxX = 400,minZ = -100,maxZ = 350*/
	   { DIVIDE_X + DIVIDE_MARGIN, AREA_MAX_X, AREA_MIN_Z, DIVIDE_Z - DIVIDE_MARGIN },
	};


	/** 一秒あたりの移動量*/
	constexpr float MOVE_SPEED = 400.0f;
	/** この距離まで言ったら到達したとみなす*/
	constexpr float ARRIVE_DIST = 5.0f;
	/** ランダム移動した後の場所を最低でも300話した場所を目標にする*/
	constexpr float MIN_TARGET_DIST = 300.0f;
	/** 到着後に立ち止まる秒数*/
	constexpr float WAIT_TIME = 1.0f;
	/** 決め直しの数*/
	constexpr int MAX_RETRY = 40;
	/** 偽のごみを落とす間隔*/
	constexpr float DROP_INTERVAL = 1.5f;
	/** 犬の何メートル後ろに偽のごみを出すか*/
	constexpr float DROP_OFFSET = 40.0f;
	/** 本来進む距離のこの割合未満しか進めなかったら「詰まっている」*/
	constexpr float STUCK_MOVE_RATE = 0.5f;
	/** この秒数詰まったら目標を決め直す*/
	constexpr float STUCK_TIME = 0.3f;

	/** 床判定用のレイ*/
	constexpr float RAY_TOP_Y = 300.0f;
	constexpr float FLOOR_Y_MIN = -15.0f;
	constexpr float FLOOR_Y_MAX = 5.0f;
	/** 目標地点の周囲も調べる*/
	constexpr float CHECK_RADIUS = 20.0f;



}
Dog::~Dog()
{
	/** 落としたごみをすべて削除する*/
	for (FakeTrash * trash : m_fakeTrashList)
	{
		DeleteGO(trash);
	}
	m_fakeTrashList.clear();
}

bool Dog::Start()
{
	m_position = POS;

	/** キャラコンの初期化*/
	//m_characterController.Init(COLLISIOM_W, COLLISIOM_H, m_position);
	/** 立ちアニメーションクリップを読み込む*/
	m_animationClips[enAnimationClip_Idle].Load(ANIM_IDLE_PATH);
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	/** 走りアニメーションを読み込む*/
	m_animationClips[enAnimatioinClip_Run].Load(ANIM_RUN_PATH);
	m_animationClips[enAnimatioinClip_Run].SetLoopFlag(true);
	/** モデルにアニメーションを渡す*/
	m_modelRender.Init(FILEPATH,m_animationClips,enAnimationClip_Num,enModelUpAxisZ);
	m_modelRender.SetPosition(POS);
	m_modelRender.SetScale(Scale);
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

bool Dog::IsFloorOnly(float x, float z)const
{
	/** 家具より高い位置から、床の少し下までレイを飛ばす*/
	const Vector3 start(x, RAY_TOP_Y, z);
	const Vector3 end(x, FLOOR_Y_MIN, z);
	Vector3 hitPos;
	if (!PhysicsWorld::GetInstance()->RayTest(start, end, hitPos))
	{
		/** 何も当たらない*/
		return false;
	}
	/** 最初に当たった点が床の高さなら上には何もない*/
	return hitPos.y >= FLOOR_Y_MIN && hitPos.y <= FLOOR_Y_MAX;

}


bool Dog::IsFreeFloor(float x, float z)const
{
	const float r = CHECK_RADIUS;
	    /** 中心*/
	return IsFloorOnly(x, z)
		/** 右*/
		&& IsFloorOnly(x + r, z)
		/** 左*/
		&& IsFloorOnly(x - r, z)
		/** 奥*/
		&& IsFloorOnly(x, z + r)
		/** 手前*/
		&& IsFloorOnly(x, z - r);
}
void Dog::UpdateDropFakeTrash()
{
	/** 走っていなかったらタイマーをリセット*/
	if (m_state != 1)
	{
		m_DropTimer = 0.0f;
		return;
	}

	m_DropTimer += g_gameTime->GetFrameDeltaTime();
	if (m_DropTimer > DROP_INTERVAL)
	{
		m_DropTimer = 0.0f;
		/** ゴミを生成する*/
		DropFakeTrash();
	}
}

void Dog::DropFakeTrash()
{
	/** 犬の進行方向の後ろにゴミを落とす*/
	const Vector3 dropPos = m_position - m_moveDir * DROP_OFFSET;

	FakeTrash * faketrash = NewGO<FakeTrash>(0, "faketrash");
	faketrash->SetPosition(dropPos);
	/** 破棄する時に消せるようにする*/
	m_fakeTrashList.push_back(faketrash);
}


void Dog::DecideNextTarget()
{
	/** 全区画を回り終えていたら順番を引き直す*/
	if (m_areaRandomIndex >= DOG_AREA_NUM)
	{
		ShuffleAreaRandom();
		m_areaRandomIndex = 0;
	}
	/** 向かう区画を取り出して、読み出し位置を決める*/
	const int areaIndex = m_areaRandom[m_areaRandomIndex];
	m_areaRandomIndex++;
	m_lastArea = areaIndex;
	const DogAreaInfo& area = DOG_AREAS[areaIndex];

	bool found = false;
	for (int i = 0; i < MAX_RETRY; i++)
	{
		Vector3 candidate;
		candidate.x = RandomRange(area.minX, area.maxX);
		/** Y軸は高さを変えないのでランダムにしない*/
		candidate.y = m_position.y;
		candidate.z = RandomRange(area.minZ, area.maxZ);

		/** 家具の上や中は目標にしない*/
		if (!IsFreeFloor(candidate.x, candidate.z))
		{
			continue;
		}
		/** 現在位置から距離を調べる*/
		Vector3 diff = candidate - m_position;
		/** Y軸は考慮しない*/
		diff.y = 0.0f;

		/** 十分離れていれば、この目標地点で決定*/
		if (diff.Length() < MIN_TARGET_DIST)
		{
			continue;
		}
		/** 進行方向を計算*/
		m_tagetPos = candidate;
		found = true;
		break;
	}
	if (!found)
	{
		/** 見つからなければ、今の位置を目標にして短く待ち、次の区画で再抽選する*/
		m_tagetPos = m_position;
		m_waitTimer = 0.1f;
		return;
	}

	/** 進行方向を計算*/
	m_moveDir = m_tagetPos - m_position;
	m_moveDir.y = 0.0f;
	m_moveDir.Normalize();
}

void Dog::ShuffleAreaRandom()
{
	m_areaRandom = { 0,1,2,3,};
	for (int i = DOG_AREA_NUM - 1; i > 0; i--)
	{
		/** 「重複なしで全員そろった、ランダムな順番」を作るために値を入れ替える*/
		const int j = rand() % (i + 1);

		int temp = m_areaRandom[i];
		m_areaRandom[i] = m_areaRandom[j];
		m_areaRandom[j] = temp;
	}
		/** 最後に行ったエリアが先頭に来たら、先頭と最後を入れ替えて同じエリアにならないようにする。*/
		if (m_areaRandom[0] == m_lastArea)
		{
			int temp = m_areaRandom[0];
			m_areaRandom[0] = m_areaRandom[DOG_AREA_NUM - 1];
			m_areaRandom[DOG_AREA_NUM - 1] = temp;
		}
		m_areaRandomIndex = 0;
	}


void Dog::Update()
{
	/** 移動*/
	Move();
	/** 回転*/
	Rotation();

	/** ステート管理*/
	ManageState();
	/** アニメーション*/
	PlayAnimation();
	/** 走っている間はゴミを落とす処理*/
	UpdateDropFakeTrash();
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}


void Dog::Move()
{
	const float dt = g_gameTime->GetFrameDeltaTime();
	/** 到着後の待機中*/
	if (m_waitTimer > 0.0f)
	{
		m_waitTimer -=dt;
		/** 待機中も重力などを更新するため、速度ゼロでExecuteを呼ぶ*/
		m_moveSpeed = Vector3::Zero;
		//m_position = m_characterController.Execute(m_moveSpeed, dt);
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
	const float distance = toTaget.Length();
	const float step = MOVE_SPEED * dt;

	/** 到着判定*/
	/**目標までの距離を計算して目標に到着したら*/
	if (distance < ARRIVE_DIST||distance < step)
	{
		m_position.x = m_tagetPos.x;
		m_position.z = m_tagetPos.z;
		m_waitTimer = WAIT_TIME;
		return;
	}

	/** 目標地点に向かう「速度」を作る*/
	m_moveDir = toTaget / distance;
	m_position += m_moveDir * step;
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
	//angle += Math::PI;
	m_rotation.SetRotationY(angle);
}


void Dog::ManageState()
{
	/** 犬の待機時間が０になるまで*/
	if (m_waitTimer > 0.0f)
	{
		m_state = 0;
	}
	/** 移動してたら*/
	else
	{
		m_state = 1;
	}
}

void Dog::PlayAnimation()
{
	switch (m_state)
	{
	case 0:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case 1:
		m_modelRender.PlayAnimation(enAnimatioinClip_Run);
		break;
	default:
		break;
	}

}
void Dog::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
