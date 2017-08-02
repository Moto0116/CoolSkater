/**
 * @file	TitleScene.h
 * @brief	タイトルシーンクラス定義
 * @author	morimoto
 */
#ifndef TITLESCENE_H
#define TITLESCENE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SceneManager\SceneBase\SceneBase.h"


class TitleBackground;
class TitleLogo;
class TitleText;


class TitleScene : public Lib::SceneBase
{
public:
	/**
	 * コンストラクタ
	 * @param[in] _sceneId シーンのID
	 */
	TitleScene(int _sceneId);

	/**
	 * デストラクタ
	 */
	virtual ~TitleScene();

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
	TitleBackground*	m_pBackground;
	TitleLogo*			m_pTitleLogo;
	TitleText*			m_pTitleText;

};


#endif // !TITLESCENE_H
