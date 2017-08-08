/**
 * @file	OverScene.h
 * @brief	ゲームオーバーシーンクラス定義
 * @author	morimoto
 */
#ifndef OVERSCENE_H
#define OVERSCENE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SceneManager\SceneBase\SceneBase.h"


class OverBackground;
class OverLogo;
class OverText;
class OverClip;


class OverScene : public Lib::SceneBase
{
public:
	/**
	 * コンストラクタ
	 * @param[in] _sceneId シーンのID
	 */
	OverScene(int _sceneId);

	/**
	 * デストラクタ
	 */
	virtual ~OverScene();

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
	OverBackground* m_pOverBackground;
	OverLogo*		m_pOverLogo;
	OverText*		m_pOverText;
	OverClip*		m_pOverClip;

	int m_OverSoundIndex;
	int m_SelectIndex;

	int m_Counter;
	bool m_IsSceneChange;

};


#endif // !OVERSCENE_H
