#include "stdafx.h"
#include "TrashManager.h"
#include"Source/Actor/VaccumCleaner/VaccumCleaner.h"
namespace
{
	/** ゴミの全体量*/
	const int TRASH_COUNT = 60;

	/** 床の範囲*/
	/** Xの最大範囲と最小範囲*/
	constexpr float FLOOR_MIN_X = -500.0f;
	constexpr float FLOOR_MAX_X = 500.0f;
	/** Xの最大範囲と最小範囲*/
	constexpr float FLOOR_MIN_Z = 0.0f;
	constexpr float FLOOR_MAX_Z = 1000.0f;
	/** Yの範囲*/
	constexpr float FLOOR_Y_MIN = -15.0f;
	constexpr float FLOOR_Y_MAX = 5.0f;

	/** 床の中心X座標*/
	constexpr float FLOOR_CENTER_X = (FLOOR_MIN_X + FLOOR_MAX_X) * 0.5f;
	/** 床の中心Z座標*/
	constexpr float FLOOR_CENTER_Z = (FLOOR_MIN_Z + FLOOR_MAX_Z) * 0.5f;

	struct AreaInfo
	{
		/** 区画のX座標の最小値*/
		float minX;
		/** 区画のX座標の最大値*/
		float maxX;
		/** 区画のZ座標の最小値*/
		float minZ;
		/** 区画のZ座標の最大値*/
		float maxZ;
	};

	/** 区画の範囲情報*/
	constexpr AreaInfo AREA_RANGES[] =
	{
		/** 左上*/
		{ FLOOR_MIN_X,FLOOR_CENTER_X, FLOOR_CENTER_Z, FLOOR_MAX_Z },
		/** 右上*/
		{ FLOOR_CENTER_X,FLOOR_MAX_X, FLOOR_CENTER_Z, FLOOR_MAX_Z },
		/** 左下*/
		{ FLOOR_MIN_X,FLOOR_CENTER_X, FLOOR_MIN_Z, FLOOR_CENTER_Z },
		/** 右下*/
		{ FLOOR_CENTER_X,FLOOR_MAX_X, FLOOR_MIN_Z, FLOOR_CENTER_Z }
	};

	/** 区画ごとに出すゴミの種類*/
	const  std::vector<EnTrashType> AREA_TRASH_TYPES[] =
	{
		/** 左上*/
		{ EnTrashType::enBottle, EnTrashType::enDrinkCan, EnTrashType::enFoodCan },
		/** 右上*/
		{ EnTrashType::enBox,EnTrashType::enPizzaBox },
		/** 左下*/
		{ EnTrashType::enBottle, EnTrashType::enDrinkCan,EnTrashType::enFoodCan },
		/** 右下*/
		{ EnTrashType::enBox, EnTrashType::enPizzaBox,}
	};

	/** 1個のゴミに対する座標の抽選やり直し上限*/
	constexpr int MAX_RETRY = 100;

	/** レイの開始高さ(家具より高く、天井より低くする)*/
	constexpr float RAY_TOP_Y = 300.0f;

	/** ゴミの半径(周囲チェック用)*/
	constexpr float TRASH_RADIUS = 20.0f;
	/** 吸引関連*/
	/* 吸引が届く距離**/
	constexpr float SUCTION_RANGE = 100.0f;
	/** 吸引の扇形の半角(度)*/
	constexpr float SUCTION_HALF_ANGLE_DEG = 45.0f;
	/** この距離まで近づいたら吸い込み完了*/
	constexpr float ABSORB_DISTANCE = 40.0f;
	/** 吸い寄せ速度(遠い時と近い時)*/
	constexpr float SUCTION_SPEED_MIN = 50.0f;
	constexpr float SUCTION_SPEED_MAX = 300.0f;

}

TrashManager::TrashManager()
{

}


TrashManager::~TrashManager()
{
	/** 吸い込まれずに残っているゴミを全て削除*/
	for (TrashEntry&entry : m_trashList)
	{
		DeleteGO(entry.trash);
	}
	m_trashList.clear();
}


bool TrashManager::IsFloorOnly(float x, float z,float &outY)const
{
	/** 家具より高い空中からレイを打つ*/
	Vector3 start(x, RAY_TOP_Y, z);
	/** 床に当てるために少し下までレイを伸ばすために-10している*/
	Vector3 end(x, FLOOR_Y_MIN - 10.0f, z);

	Vector3 hitPos;
	/** ここでレイを飛ばす*/
	if (!PhysicsWorld::GetInstance()->RayTest(start, end, hitPos))
	{
		/** レイが何にも当たらない = 床のコリジョンがない場所だからゴミを置かない*/
		return false;
	}
	outY = hitPos.y;
	/** 最初に当たった点が床～カーペットの高さなら、上には何もない*/
	return hitPos.y >= FLOOR_Y_MIN && hitPos.y <= FLOOR_Y_MAX;
	
}

bool TrashManager::SpawnTrash(EnArea area)
{
	/** static_castをつけてenum classを整数に変換*/
	const int index = static_cast<int>(area);
	/** 区画の範囲情報を取得*/
	/** 配列の要素をコピー無しで別名で受け取るために「&」をつける*/
	const AreaInfo &range = AREA_RANGES[index];

	/** この区画に配置するゴミの種類*/
	const std::vector<EnTrashType>& types = AREA_TRASH_TYPES[index];
	/**uniform_real_distributionは指定した範囲から偏りなく少数を引くための部品 */
	std::uniform_real_distribution<float>rx(range.minX, range.maxX);
	std::uniform_real_distribution<float>rz(range.minZ, range.maxZ);
	/** この区画だけ範囲で乱数を出す*/
	/** types.sizeはsize_t（符号なしの整数)を返してしまうのでstatic_castでintに変換してから使う*/
	std::uniform_int_distribution<int>rt(0, static_cast<int>(types.size()) - 1);

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	bool found = false;
	/** 床のみの座標が出るまで引き直し*/
	for (int retry = 0; retry < MAX_RETRY; retry++)
	{
		x = rx(m_random);
		z = rz(m_random);
		if (CanPlaceTrash(x, z, y))
		{
			found = true;
			break;
		}
	}

	/** 置ける場所が見つからなければ、ゴミを出さない*/
	if (!found)
	{
		return false;
	}

	Trash* trash = NewGO<Trash>(0, "trash");
	/** ゴミの種類と位置を記録*/
	trash->Setup(types[rt(m_random)], Vector3(x, y, z));

	/** ゴミと区画をペアで記録し、区画の残り数を増やす*/
	m_trashList.push_back({ trash, area });
	m_areaTrashCount[index]++;
	m_totalCleanPoint += trash->GetCleanPoint();
	return true;

}

bool TrashManager::Start()
{

	/** 乱数のもとになるシード値を設定する*/
	m_random.seed(std::random_device{}());

	/** 一区画あたりの配置数*/
	const int trasPerArea = TRASH_COUNT / AREA_NUM;
	/** ここで各区画にゴミを配置する*/
	/** ここでゴミの総数(現在は50)だけ繰り返す*/
	for (int i = 0; i < AREA_NUM; i++)
	{
		/** 床のみの座標が出るまで引き直し*/
		for (int n = 0; n < trasPerArea; n++)
		{
			SpawnTrash(static_cast<EnArea>(i));
		}
	}
	return true;
}

bool TrashManager::CanPlaceTrash(float x, float z,float&outY)const
{
	/** 中心と上下左右の5点が全て床のみならOK*/
	const float r = TRASH_RADIUS;
	float dummy = 0.0f;
	return IsFloorOnly(x, z,outY)
		&& IsFloorOnly(x + r, z,dummy)
		&& IsFloorOnly(x - r, z,dummy)
		&& IsFloorOnly(x, z + r,dummy)
		&& IsFloorOnly(x, z - r,dummy);
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
	/** 掃除機に吸い込み口の位置と向きを教えてもらう*/
	const Vector3 nozzlePos = vaccum->GetSuctionPosition();
	const Vector3 suctionDir = vaccum->GetSuctionDirection();
	float dt = g_gameTime->GetFrameDeltaTime();

	for (auto it = m_trashList.begin(); it != m_trashList.end();)
	{
		/** ゴミと距離を求める*/
		Trash* trash = it->trash;
		const Vector3 toTrash = trash->GetPosition() - nozzlePos;
		const float distance = toTrash.Length();

		/** 吸い込み完了*/
		/** ゴミとの距離が40以下になったら*/
		if (distance <= ABSORB_DISTANCE)
		{
			/** ポイントを加算*/
			m_currentCleanPoint += trash->GetCleanPoint(); 
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
	/** ノズルからゴミへの矢印で長さは距離そのもの*/
	Vector3 toTrashDir = toTrash;
	/** 長さを1にそろえる*/
	toTrashDir.Normalize();
	/** cosfはラジアンを受け取る*/
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
