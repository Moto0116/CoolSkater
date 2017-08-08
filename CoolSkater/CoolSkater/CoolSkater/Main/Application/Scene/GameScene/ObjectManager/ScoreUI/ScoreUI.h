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
#include <vector>


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

	enum SCORE_PROGRESS
	{
		PROGRESS_ONE,
		PROGRESS_TWO,
		PROGRESS_THREE,
		PROGRESS_FOUR,
		PROGRESS_FIVE,
		PROGRESS_SIX,
		PROGRESS_SEVEN,
		PROGRESS_EIGHT,
		PROGRESS_NONE
	};

	struct SCORE_HISTORY
	{
		int Score;
		int Counter;
		bool IsGolden;
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

	void ProgressDraw();

	// イベント受信関数.
	void ReciveEvent(Lib::EventBase* _pEvent);

	void(ScoreUI::*m_pUpdateFunc)();
	void(ScoreUI::*m_pDrawFunc)();

	STATE m_State;
	ScoreUIEventListener* m_pEventListener;
	Lib::Font* m_pFont; 
	int m_Score;

	int				m_ProgressTextureIndex;
	D3DXVECTOR2		m_ProgressPos;
	D3DXVECTOR2		m_ProgressSize;
	SCORE_PROGRESS	m_ProgressType;
	int				m_ProgressTime;
	bool			m_IsProgressDraw;
	float			m_ProgressAlpha;

	int				m_ChimeSoundIndex;
	bool			m_IsSound;
	int				m_TextureIndex2;

	D3DXVECTOR2 m_StageProgressPos;
	D3DXVECTOR2 m_StageProgressSize;

	Lib::Font* m_pScoreHistoryFont;
	Lib::Font* m_pScoreHistoryFontY;
	std::vector<SCORE_HISTORY> m_History;
	int m_PrevScore;

};


#endif // !SCOREUI_H
