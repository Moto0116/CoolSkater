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
	float m_AlphaColor;

	STATE m_State;
	StageBackgroundEventListener*	m_pEventListener;
	StageBackgroundEvent*			m_pEvent;

};


#endif // !STAGEBACKGROUND_H