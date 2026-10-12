#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/Character/Player/Player.h"
#include"GameCamera/GameCamera.h"
#include"Source/Actor/Stage/Stage.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"Source/Actor/Character/Dog/Dog.h"
bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	m_stage = NewGO<Stage>(0, "stage");

	m_trashManager = NewGO<TrashManager>(0, "trashmanager");

	m_dog = NewGO<Dog>(0, "dog");

	return true;
}

void Game::Update()
{
	
}

