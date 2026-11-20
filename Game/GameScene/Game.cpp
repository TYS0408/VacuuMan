#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/Character/Player/Player.h"
#include"GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Stage.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"Source/Actor/Character/Dog/Dog.h"
#include"GameTimer/GameTimer.h"
#include"CleanRate/CleanRate.h"
#include"SoundManager/SoundManager.h"
#include"Pause/Pause.h"
#include"Score/Score.h"
#include"TrashBox/TrashBox.h"
#include"HeldTrashUI/HeldTrashUI.h"

Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_stage);
	DeleteGO(m_trashManager);
	DeleteGO(m_dog);
	DeleteGO(m_timer);
	DeleteGO(m_cleanRate);
	DeleteGO(m_score);
	DeleteGO(m_trashBox);
	DeleteGO(m_heldTrashUI);
	DeleteGO(m_gameBGM);
	if (m_pause != nullptr)
	{
		DeleteGO(m_pause);
	}
}
bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	m_stage = NewGO<Stage>(0, "stage");

	m_trashManager = NewGO<TrashManager>(0, "trashmanager");

	m_dog = NewGO<Dog>(0, "dog");

	m_pause = NewGO<Pause>(0, "pause");

	m_timer = NewGO<GameTimer>(0, "timer");

	m_cleanRate = NewGO<CleanRate>(0, "cleanrate");

	m_score = NewGO<Score>(0, "score");
	m_trashBox = NewGO<TrashBox>(0, "trashbox");
	m_heldTrashUI = NewGO<HeldTrashUI>(0, "heldtrashui");
	/** BGMを流す*/
	SoundManager* soundManager = FindGO<SoundManager>("soundmanager");
	m_gameBGM = soundManager->PlayingBGM(SoundBGM::enInGameBGM, true);
	return true;
}

void Game::Update()
{
	
}

