#ifndef SCORESCENE_H
#define SCORESCENE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "SceneManager\SceneBase\SceneBase.h"

class ScoreText;
class ScoreClip;

class ScoreScene : public Lib::SceneBase
{
public:
	/**
	 * コンストラクタ
	 * @param[in] _sceneId シーンのID
	 */
	ScoreScene(int _sceneId);

	/**
	 * デストラクタ
	 */
	virtual ~ScoreScene();

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
	int m_ScoreSceneSoundIndex;
	int m_SelectSoundIndex;
	ScoreText* m_pScoreText;
	ScoreClip* m_pScoreClip;
	bool m_IsTitle;
	int m_Counter;

};


#endif // !SCORESCENE_H
