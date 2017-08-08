/**
 * @file	StageBackground.h
 * @brief	ステージ背景クラス定義
 * @author	morimoto
 */
#ifndef STAGEBACKGROUND_H
#define STAGEBACKGROUND_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include "StageBackgroundEventListener\StageBackgroundEventListener.h"
#include "..\..\..\Event\StageBackgroundEvent\StageBackgroundEvent.h"


/** 
 * ステージ背景クラス
 */
class StageBackground : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	StageBackground();

	/**
	 * デストラクタ
	 */
	virtual ~StageBackground();

	/**
	 * 初期化処理
	 * @return 初期化に成功したらtrue 失敗したらfalse
	 */
	virtual bool Initialize();

	/**
	 * 終了処理
	 */
	virtual void Finalize();

	/**
	 * オブジェクトの更新
	 */
	virtual void Update();

	/**
	 * オブジェクトの描画
	 */
	virtual void Draw();

private:
	/**
	 * ステージ背景のステート
	 */
	enum STATE
	{
		PAUSE_STATE,
		GAME_START_STATE,
		GAME_STATE,
		STATE_MAX
	};

	enum STAGE_TYPE
	{
		STAGE_1,
		STAGE_2,
		STAGE_3,
		STAGE_4
	};

	static const D3DXVECTOR2 m_DefaultPos;	//!< デフォルト座標.

	// 更新関数.
	void PauseUpdate();
	void GameStartUpdate();
	void GameUpdate();

	// 描画関数.
	void PauseDraw();
	void GameStartDraw();
	void GameDraw();

	void ReciveEvent(Lib::EventBase* _pEvent);

	void(StageBackground::*m_pUpdateFunc)();
	void(StageBackground::*m_pDrawFunc)();
	int	m_TextureIndex2;
	int	m_TextureIndex3;
	int	m_TextureIndex4;
	int	m_Stage12TextureIndex;
	int	m_Stage23TextureIndex;
	int m_CurrentTextureIndex;
	int m_NextTextureIndex;
	float m_AlphaColor;

	STATE m_State;
	StageBackgroundEventListener*	m_pEventListener;
	StageBackgroundEvent*			m_pEvent;

	STAGE_TYPE m_StageType;
	int Stage1Length;
	int Stage2Length;
	int Stage3Length;

	bool m_IsStage12;
	bool m_IsStage23;

	D3DXVECTOR2 m_Stage12Pos;
	D3DXVECTOR2 m_Stage12Size;
	D3DXVECTOR2 m_Stage23Pos;
	D3DXVECTOR2 m_Stage23Size;

};


#endif // !STAGEBACKGROUND_H