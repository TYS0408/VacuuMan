#pragma once
#include "Level3DRender/LevelRender.h"
#include"sound/SoundSource.h"
class Player;
class GameCamera;
class Stage;
class TrashManager;
class Dog;
class GameTimer;
class CleanRate;
class Pause;
class Score;
class TrashBox;
class HeldTrashUI;
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override {};

private:
	/** プレイヤー */
	Player* m_player = nullptr;
	/** ゲームカメラ*/
	GameCamera* m_gameCamera = nullptr;

	/** ステージ*/
	Stage* m_stage = nullptr;

	/** トラッシュマネージャー*/
	TrashManager* m_trashManager = nullptr;

	/** ドッグ*/
	Dog* m_dog = nullptr;

	/** タイマー*/
	GameTimer* m_timer = nullptr;

	/** 達成率を表すUI*/
	CleanRate* m_cleanRate = nullptr;

	/** ポーズ*/
	Pause* m_pause = nullptr;
	/* インゲームBGM*/
	SoundSource* m_gameBGM = nullptr;
	/** スコア*/
	Score* m_score = nullptr;
	/** トラッシュボックス*/
	TrashBox* m_trashBox = nullptr;
	/** あとどれくらいごみを吸えるかのUI*/
	HeldTrashUI* m_heldTrashUI = nullptr;

};

