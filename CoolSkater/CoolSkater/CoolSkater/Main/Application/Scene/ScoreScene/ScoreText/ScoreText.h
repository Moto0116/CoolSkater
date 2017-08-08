/**
 * @file	ScoreText.h
 * @brief	スコアテキストクラス定義
 * @author	morimoto
 */
#ifndef SCORETEXT_H
#define SCORETEXT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"
#include "DirectX11\Font\Font.h"
#include <vector>


class ScoreText : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ScoreText();

	/**
	 * デストラクタ
	 */
	virtual ~ScoreText();

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
	Lib::Font* m_pFont;
	int m_TextureIndex2;
	int m_TextureIndex3;
	int m_TextureIndex4;
	std::vector<int> m_ScoreData;

	bool m_IsScoreBack;
	bool m_IsScoreFlash;
	float m_ScoreAlphaColor;

};


#endif // !SCORETEXT_H
