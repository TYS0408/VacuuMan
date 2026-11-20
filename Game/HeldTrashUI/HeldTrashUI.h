#pragma once
class TrashManager;
class TrashBox;
class Pause;
class HeldTrashUI : public IGameObject
{
public:
	HeldTrashUI() {};
	~HeldTrashUI() {};

public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
	


private:
	/** スロット数(TrashManagerの上限と同じ値)*/
	static const int SLOT_NUM = 10;
	/** トラッシュマネージャー*/
	TrashManager* m_trashManager = nullptr;
	/** トラッシュボックス*/
	TrashBox* m_trashBox = nullptr;
	/**  ポーズ*/
	Pause* m_pause = nullptr;

	int m_heldCount = 0;
	bool m_isMaxHeld = false;
	/** 枠*/
	SpriteRender m_frameSpriteRender;
	/** 「ゴミ所持数」のラベル*/
	SpriteRender m_wordSpriteRender;
	/** 大きい袋のアイコン*/
	SpriteRender m_iconSpriteRender;
	/** 空きスロット(グレー)*/
	SpriteRender m_emptySpriteRender[SLOT_NUM];
	/** 埋まったスロット(ブルー)*/
	SpriteRender m_fillSpriteRender[SLOT_NUM];
	/** ゴミ箱に近づいたときに出すAボタン*/
	SpriteRender m_abuttanSpriteRender;
	/** Aボタンを描画するかどうか*/
	bool m_isShowAbuttan = false; 
	/** Aボタンの上下移動の経過時間*/
	float m_abuttanTimer = 0.0f;
	
};

