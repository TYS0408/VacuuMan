#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/Character/Player/Player.h"
#include"GameCamera/GameCamera.h"
bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	return true;
}

void Game::Update()
{
	
	
}

