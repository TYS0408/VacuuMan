#include "stdafx.h"
#include "HeldTrashUI.h"
#include"Source/Actor/Stage/TrashManager.h"
#include"Pause/Pause.h"
#include"TrashBox/TrashBox.h"
namespace
{
	/** ファイルパス*/
	/** 枠*/
	const char* FRAME_FILEPATH = "Assets/Sprite/TrashUI/TrashFream.dds";
	/** 文字*/
	const char* WORD_FILEPATH = "Assets/Sprite/TrashUI/TrashNumber_Word.dds";
	/** アイコン*/
	const char* ICON_FILEPATH = "Assets/Sprite/TrashUI/TrashIcon.dds";
	/**空(灰色)のアイコン */
	const char* EMPTY_FILEPATH = "Assets/Sprite/TrashUI/NotTrashUI.dds";
	/** 所持(青色)のアイコン*/
	const char* FILLED_FILEPATH = "Assets/Sprite/TrashUI/TrashUI.dds";
	/**ゴミ箱に近づいたときに出すAボタン*/
	const char* ABUTTAN_FILEPATH = "Assets/Sprite/TrashUI/Abuttan.dds";

	/** 枠*/
	constexpr float FRAME_W = 700.0f;
	constexpr float FRAME_H = 200.0f;
	const Vector3 FRAME_POS = Vector3(640.0f, -450.0f, 0.0f);

	/** スロット*/
	constexpr float SLOT_SIZE = 100.0f;
	constexpr float SLOT_SPACING = 60.0f;

	/**	文字*/
	constexpr float WORD_W = 300.0f;
	constexpr float WORD_H = 50.0f;
	const Vector3 WORD_POS = Vector3(640.0f, -370.0f, 0.0f);

	/** 大きい目印アイコン*/
	constexpr float ICON_W = 100.0f;
	constexpr float ICON_H = 100.0f;
	const Vector3 ICON_POS = Vector3(270.0f, -450.0f, 0.0f);
	/** Aボタン*/
	constexpr float ABUTTAN_W = 100.0f;
	constexpr float ABUTTAN_H = 75.0f;
	const Vector3 ABUTTAN_POS = Vector3(-550.0f, 250.0f, 0.0f);
	/** Aボタンの上下移動の幅*/
	constexpr float ABUTTAN_MOVE_RANGE = 10.0f;
	/** Aボタンの上下移動の速さ*/
	constexpr float ABUTTAN_MOVE_SPEED = 5.0f;

}

bool HeldTrashUI::Start()
{
	/** それぞれの情報を取得*/
	m_trashManager = FindGO<TrashManager>("trashmanager");
	m_trashBox = FindGO<TrashBox>("trashbox");
	m_pause = FindGO<Pause>("pause");
	/** 枠の初期化*/
	m_frameSpriteRender.Init(FRAME_FILEPATH, FRAME_W, FRAME_H);
	m_frameSpriteRender.SetPosition(FRAME_POS);
	m_frameSpriteRender.Update();

	/** 文字*/
	m_wordSpriteRender.Init(WORD_FILEPATH, WORD_W, WORD_H);
	m_wordSpriteRender.SetPosition(WORD_POS);
	m_wordSpriteRender.Update();

	/** アイコン*/
	m_iconSpriteRender.Init(ICON_FILEPATH, ICON_W, ICON_H);
	m_iconSpriteRender.SetPosition(ICON_POS);
	m_iconSpriteRender.Update();

	/** Aボタン*/
	m_abuttanSpriteRender.Init(ABUTTAN_FILEPATH, ABUTTAN_W, ABUTTAN_H);
	m_abuttanSpriteRender.SetPosition(ABUTTAN_POS);
	m_abuttanSpriteRender.Update();

	/** 枠の中心から左右対称に10個並べる*/
	const float startX = FRAME_POS.x - SLOT_SPACING * (SLOT_NUM - 1) * 0.5f;
	for (int i = 0; i < SLOT_NUM; i++)
	{
		const Vector3 pos(startX + SLOT_SPACING * i, FRAME_POS.y +5.0f, 0.0f);
		/** まだ吸える空きスロットがあるときのアイコン*/
		m_emptySpriteRender[i].Init(EMPTY_FILEPATH, SLOT_SIZE, SLOT_SIZE);
		m_emptySpriteRender[i].SetPosition(pos);
		m_emptySpriteRender[i].Update();

		/** 吸えるスロットが残っていないときのアイコン*/
		m_fillSpriteRender[i].Init(FILLED_FILEPATH, SLOT_SIZE, SLOT_SIZE);
		m_fillSpriteRender[i].SetPosition(pos);
		m_fillSpriteRender[i].Update();

	}
	return true;
}

void HeldTrashUI::Update()
{
	if (m_pause != nullptr && m_pause->GetIsPause())
	{
		return;
	}
	if (m_trashManager == nullptr)
	{
		return;
	}
	/** 持っている個数を*heldCountに教える*/
	m_heldCount = m_trashManager->GetHeldTrashCount();
	
	/** ゴミを一個以上持っていて、ゴミ箱の近くに居たらAボタンのUIを出すようにする*/
	m_isShowAbuttan = m_heldCount > 0 && m_trashBox != nullptr && m_trashBox->IsPlayerNear();

	if (m_isShowAbuttan)
	{
		/** 表示している間だけ時間を進める*/
		m_abuttanTimer += g_gameTime->GetFrameDeltaTime();
		/** sinfは-1～1を返すので、幅をかけて基準位置に足すことで
		上下に動く距離にする*/
		Vector3 pos = ABUTTAN_POS;
		/** m_abuttanTimer * ABUTTAN_MOVE_SPEED　=経過時間に速さをかけた値*/

		pos.y += sinf(m_abuttanTimer * ABUTTAN_MOVE_SPEED) * ABUTTAN_MOVE_RANGE;
		m_abuttanSpriteRender.SetPosition(pos);
		m_abuttanSpriteRender.Update();
	}
	else
	{
		/** 非表示になったらリセット。次に出す時は基準位置から動き始める*/
		m_abuttanTimer = 0.0f;
	}
}



void HeldTrashUI::Render(RenderContext& rc)
{
	if (m_pause != nullptr && m_pause->GetIsPause())
	{
		return;
	}
	/** 奥から手前の順に描画*/
	m_frameSpriteRender.Draw(rc);
	for (int i = 0; i < SLOT_NUM; i++)
	{
		/** 持っている数まで青、残りはグレー*/
		if (i < m_heldCount)
		{
			m_fillSpriteRender[i].Draw(rc);
		}
		else
		{
			m_emptySpriteRender[i].Draw(rc);
		}
	}
	/** 文字の描画*/
	m_wordSpriteRender.Draw(rc);
	/** アイコンの描画*/
	m_iconSpriteRender.Draw(rc);
	if (m_isShowAbuttan)
	{
		m_abuttanSpriteRender.Draw(rc);
	}
}