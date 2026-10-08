#include "stdafx.h"
#include "TrashManager.h"

namespace
{
	/** ゴミの全体量*/
	const int TRASH_COUNT = 50;

	/** 床の範囲*/
	/** Xの最大範囲と最小範囲*/
	const float FLOOR_MIN_X = -400.0f;
	const float FLOOR_MAX_X = 400.0f;
	/** Xの最大範囲と最小範囲*/
	const float FLOOR_MIN_Z = -800.0f;
	const float FLOOR_MAX_Z = 0.0f;
	/** Yの範囲*/
	const float FLOOR_Y = 0.0f;


	/** 1個のゴミに対する座標の抽選やり直し上限*/
	const int MAX_RETRY = 100;

	/** レイの開始高さ(家具より高く、天井より低くする)*/
	const float RAY_TOP_Y = 300.0f;

	/**床とみなす高さの許容誤差*/
	const float FLOOR_TOLERANCE = 5.0f;
	/** ゴミの半径(周囲チェック用)*/
	const float TRASH_RADIUS = 20.0f;

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

}


void TrashManager::Render(RenderContext& rc)
{

}