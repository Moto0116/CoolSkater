/**
 * @file	ScoreUI.h
 * @brief	スコアUIクラス定義
 * @author	morimoto
 */
#ifndef SCOREUI_H
#define SCOREUI_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include "ScoreUIEventListener\ScoreUIEventListener.h"
#include "DirectX11\Font\Font.h"


class ScoreUI : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ScoreUI();

	/**
	 * デストラクタ
	 */
	virtual ~ScoreUI();

	/**
	 * 初期化処理
	 * @return 初期化に成功したか
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
	 * プレイヤーのステート 
	 */
	enum STATE
	{
		PAUSE_STATE,
		GAME_START_STATE,
		GAME_STATE,
		STATE_MAX
	};

	static const D3DXVECTOR2 m_ScorePos;
	static const D3DXVECTOR2 m_ScoreSize;

	// 更新関数.
	void PauseUpdate();
	void GameStartUpdate();
	void GameUpdate();

	// 描画関数.
	void PauseDraw();
	void GameStartDraw();
	void GameDraw();

	// イベント受信関数.
	void ReciveEvent(Lib::EventBase* _pEvent);

	void(ScoreUI::*m_pUpdateFunc)();
	void(ScoreUI::*m_pDrawFunc)();

	STATE m_State;
	ScoreUIEventListener* m_pEventListener;
	Lib::Font* m_pFont; 
	int m_Score;

};


#endif // !SCOREUI_H
