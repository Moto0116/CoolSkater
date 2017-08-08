#ifndef CLEARSCENE_H
#define CLEARSCENE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SceneManager\SceneBase\SceneBase.h"


class ClearBackground;
class ClearLogo;
class ClearText;
class ClearClip;


class ClearScene : public Lib::SceneBase
{
public:
	/**
	 * コンストラクタ
	 * @param[in] _sceneId シーンのID
	 */
	ClearScene(int _sceneId);

	/**
	 * デストラクタ
	 */
	virtual ~ClearScene();

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
	ClearBackground*	m_pClearBackground;
	ClearLogo*			m_pClearLogo;
	ClearText*			m_pClearText;
	ClearClip*			m_pClearClip;
	int m_Counter;
	bool m_IsTitle;
	int m_SoundIndex;
	int m_SelectIndex;

};


#endif // !CLEARSCENE_H
