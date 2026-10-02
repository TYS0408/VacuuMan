#include "stdafx.h"
#include "Game.h"
#include"Source/Actor/Character/Player/Player.h"

bool Game::Start()
{
	m_player = NewGO<Player>(0, "Player");
	return true;
}

void Game::Update()
{
	
	
}

