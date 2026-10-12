#include "stdafx.h"
#include "TrashManager.h"
#include"Source/Actor/VaccumCleaner/VaccumCleaner.h"
namespace
{
	/** ゴミの全体量*/
	const int TRASH_COUNT = 50;

	/** 床の範囲*/
	/** Xの最大範囲と最小範囲*/
	constexpr float FLOOR_MIN_X = -400.0f;
	constexpr float FLOOR_MAX_X = 400.0f;
	/** Xの最大範囲と最小範囲*/
	constexpr float FLOOR_MIN_Z = -800.0f;
	constexpr float FLOOR_MAX_Z = -100.0f;
	/** Yの範囲*/
	constexpr float FLOOR_Y = 0.0f;


	/** 1個のゴミに対する座標の抽選やり直し上限*/
	constexpr int MAX_RETRY = 100;

	/** レイの開始高さ(家具より高く、天井より低くする)*/
	constexpr float RAY_TOP_Y = 300.0f;

	/**床とみなす高さの許容誤差*/
	constexpr float FLOOR_TOLERANCE = 5.0f;
	/** ゴミの半径(周囲チェック用)*/
	constexpr float TRASH_RADIUS = 20.0f;

	/** 吸引関連*/
	/* 吸引が届く距離**/
	constexpr float SUCTION_RANGE = 100.0f;
	/** 吸引の扇形の半角(度)*/
	constexpr float SUCTION_HALF_ANGLE_DEG = 90.0f;
	/** この距離まで近づいたら吸い込み完了*/
	constexpr float ABSORB_DISTANCE = 40.0f;
	/** 吸い寄せ速度(遠い時と近い時)*/
	constexpr float SUCTION_SPEED_MIN = 50.0f;
	constexpr float SUCTION_SPEED_MAX = 300.0f;

}


bool TrashManager::IsFloorOnly(float x, float z)const
{
	/** 家具より高い空中からレイを打つ*/
	Vector3 start(x, RAY_TOP_Y, z);
	/** 床に当てるために少し下までレイを伸ばすために-10している*/
	Vector3 end(x, FLOOR_Y - 10.0f, z);

	Vector3 hitPos;
	/** ここでレイを飛ばす*/
	if (!PhysicsWorld::GetInstance()->RayTest(start, end, hitPos))
	{
		/** レイが何にも当たらない = 床のコリジョンがない場所だからゴミを置かない*/
		return false;
	}

	/** 最初に当たった点が床の高さなら、上には何もない*/
	return fabsf(hitPos.y - FLOOR_Y) <= FLOOR_TOLERANCE;
}

bool TrashManager::Start()
{
	/** 乱数のもとになるシード値を設定する*/
	m_random.seed(std::random_device{}());

	/** どんな範囲の乱数を出すか*/
	/** X座標*/
	std::uniform_real_distribution<float> rx(FLOOR_MIN_X, FLOOR_MAX_X);
	/** Z座標*/
	std::uniform_real_distribution<float> rz(FLOOR_MIN_Z, FLOOR_MAX_Z);
	/** ゴミの種類*/
	std::uniform_int_distribution<int> rt(0, static_cast<int>(EnTrashType::enNum) - 1);

	/** ここでゴミの総数(現在は50)だけ繰り返す*/
	for (int i = 0; i < TRASH_COUNT; i++)
	{

		float x = 0.0f;
		float z = 0.0f;
		bool found = false;

		/** 床のみの座標が出るまで引き直し*/
		for (int retry = 0; retry < MAX_RETRY; retry++)
		{
			x = rx(m_random);
			z = rz(m_random);
			if (CanPlaceTrash(x, z))
			{
				found = true;
				break;
			}
		}

		/** 上限まで試しても見つからなければ、ゴミを出さない*/
		if (!found)
		{
			continue;
		}
			Trash* trash = NewGO<Trash>(0, "trash");
			trash->Setup(static_cast<EnTrashType>(rt(m_random)), Vector3(x, FLOOR_Y, z));
			m_trashList.push_back(trash);
		}
		return true;
	}

bool TrashManager::CanPlaceTrash(float x, float z)const
{
	/** 中心と上下左右の5点が全て床のみならOK*/
	const float r = TRASH_RADIUS;
	return IsFloorOnly(x, z)
		&& IsFloorOnly(x + r, z)
		&& IsFloorOnly(x - r, z)
		&& IsFloorOnly(x, z + r)
		&& IsFloorOnly(x, z - r);
}


void TrashManager::Update()
{
	UpdateSuction();
}

void TrashManager::UpdateSuction()
{
	VaccumCleaner* vaccum = FindGO<VaccumCleaner>("vaccumcleaner");
	if (vaccum == nullptr)
	{
		return;
	}

	const Vector3 nozzlePos = vaccum->GetSuctionPosition();
	const Vector3 suctionDir = vaccum->GetSuctionDirection();
	float dt = g_gameTime->GetFrameDeltaTime();

	for (auto it = m_trashList.begin(); it != m_trashList.end();)
	{
		Trash* trash = *it;
		const Vector3 toTrash = trash->GetPosition() - nozzlePos;
		const float distance = toTrash.Length();

		/** 吸い込み完了*/
		if (distance <= ABSORB_DISTANCE)
		{
			DeleteGO(trash);
			it = m_trashList.erase(it);
			continue;
		}
		/** 範囲内なら引き寄せる*/
		if (IsInSuctionRange(toTrash, distance, suctionDir))
		{
			const float speed = CalcSuctionSpeed(distance);
			PullTrash(trash, nozzlePos, speed, dt);
		}
		++it;
	}
}


bool TrashManager::IsInSuctionRange(const Vector3& toTrash, float distance, const Vector3& suctionDir)const
{
	/** 距離チェック*/
	if (distance > SUCTION_RANGE)
	{
		return false;
	}

	/** 角度チェック*/
	Vector3 toTrashDir = toTrash;
	toTrashDir.Normalize();
	const float cosLimit = cosf(Math::DegToRad(SUCTION_HALF_ANGLE_DEG));
	return toTrashDir.Dot(suctionDir) >= cosLimit;

}


float TrashManager::CalcSuctionSpeed(float distance)const
{
	/** 近いほど速くする*/
	const float rate = 1.0f - (distance / SUCTION_RANGE);
	return SUCTION_SPEED_MIN + (SUCTION_SPEED_MAX - SUCTION_SPEED_MIN) * rate;
}

void TrashManager::PullTrash(Trash* trash, const Vector3& nozzlePos, float speed, float deltaTime)const
{
	const Vector3 pos = trash->GetPosition();
	Vector3 moveDir = nozzlePos - pos;
	moveDir.Normalize();
	trash->SetPosition(pos + moveDir * speed * deltaTime);
}

void TrashManager::Render(RenderContext& rc)
{

}