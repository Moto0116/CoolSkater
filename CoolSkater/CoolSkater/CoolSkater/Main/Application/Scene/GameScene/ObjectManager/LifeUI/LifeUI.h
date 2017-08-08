#ifndef LIFEUI_H
#define LIFEUI_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

#include "LifeUIEventListener\LifeUIEventListener.h"
#include "..\..\Event\LifeUIEvent\LifeUIEvent.h"
#include "DirectX11\Font\Font.h"


class LifeUI : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	LifeUI();

	/**
	 * デストラクタ
	 */
	virtual ~LifeUI();
	
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

	static const D3DXVECTOR2 m_LifePos;
	static const D3DXVECTOR2 m_LifeSize;
	static const int m_DefaultLife;

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

	void(LifeUI::*m_pUpdateFunc)();
	void(LifeUI::*m_pDrawFunc)();

	STATE m_State;
	LifeUIEventListener* m_pEventListener;
	Lib::Font* m_pFont;
	int m_Life;
	int m_PrevLife;

	int m_FlashCounter;
	float m_LifeAlpha;
	bool m_IsFlash;

	LifeUIEvent* m_pEvent;

	int m_TextureIndex2;

};


#endif // !LIFEUI_H
