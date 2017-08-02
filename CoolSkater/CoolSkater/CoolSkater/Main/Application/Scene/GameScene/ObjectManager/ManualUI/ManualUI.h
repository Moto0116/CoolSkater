/**
 * @file	ManualUI.h
 * @brief	マニュアルUIクラス定義
 * @author	morimoto
 */
#ifndef MANUALUI_H
#define MANUALUI_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include "..\..\Event\ManualUIEvent\ManualUIEvent.h"


/**
 * マニュアルUIクラス
 */
class ManualUI : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ManualUI();

	/**
	 * デストラクタ
	 */
	virtual ~ManualUI();

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
	enum STATE
	{
		PAUSE_STATE,
		GAME_START_STATE,
		GAME_STATE,
		STATE_MAX
	};

	static const int m_ManualTime;

	// 更新関数.
	void PauseUpdate();
	void GameStartUpdate();
	void GameUpdate();

	// 描画関数.
	void PauseDraw();
	void GameStartDraw();
	void GameDraw();

	void(ManualUI::*m_pUpdateFunc)();
	void(ManualUI::*m_pDrawFunc)();
	int		m_Counter;
	STATE	m_State;

	ManualUIEvent*	m_pEvent;

};


#endif // !MANUALUI_H
