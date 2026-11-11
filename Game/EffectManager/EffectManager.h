#pragma once
/** エフェクトのID*/
enum class EffectID
{
	/** ステージ分割エフェクト*/
	/** 右上*/
	EffectID_StageDivision_UpperRight,
	/**左上*/
	EffectID_StageDivision_UpperLeft,
	/** 右下*/
	EffectID_StageDivision_LowerRight,
	/** 左下*/
	EffectID_StageDivision_LowerLeft,
	/** ゴミ確保エフェクト*/
	EffectID_StageSecure,
};
class EffectManager : public IGameObject
{
public:
	EffectManager() {};
	~EffectManager() {};

	/** エフェクトの初期化*/
	bool Start()override;

	/** エフェクトの読み込む関数*/
	void LoadEffects();
};

