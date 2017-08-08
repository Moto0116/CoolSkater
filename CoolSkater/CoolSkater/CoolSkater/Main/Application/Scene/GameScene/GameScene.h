/**
 * @file	GameScene.h
 * @brief	ゲームシーンクラス定義
 * @author	morimoto
 */
#ifndef GAMESCENE_H
#define GAMESCENE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SceneManager\SceneBase\SceneBase.h"

#include "ObjectManager\ObjectManager.h"
#include "DirectX11\Font\Font.h"
#include "GameSceneEventListener\GameSceneEventListener.h"


/**
 * ゲームシーンクラス
 */
class GameScene : public Lib::SceneBase
{
public:
	/**
	 * イベントID
	 */
	enum EVENT_ID
	{
		EMPTY_EVENT = 0,
		GAMEPAUSE_EVENT,
		GAMESTART_EVENT,
		STAGEBACKGROUND_EVENT,
		PLAYERDEAD_EVENT,	// プレイヤーのライフが0になった際のイベント
		PLAYER_EVENT,		// プレイヤーのイベント
		CLIP_EVENT
	};

	/**
	 * コンストラクタ
	 * @param[in] _sceneId シーンのID
	 */
	GameScene(int _sceneId);

	/**
	 * デストラクタ
	 */
	virtual ~GameScene();

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
	 * シーンの更新
	 */
	virtual void Update();

private:
	ObjectManager*	m_pObjectManager;	//!< シーン内オブジェクト管理クラス.

	GameSceneEventListener* m_pEventListener;

	int m_GameSoundIndex;

	// イベント受信関数.
	void ReciveEvent(Lib::EventBase* _pEvent);
	void ScoreSave();
	void NewScoreSave();

	bool m_IsClear;

};


#endif // !GAMESCENE_H
