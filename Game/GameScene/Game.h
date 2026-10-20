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
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc) {};

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

};

