#pragma once

#include "Level3DRender/LevelRender.h"
class Player;
class GameCamera;
class Stage;
class TrashManager;
class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
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

};

