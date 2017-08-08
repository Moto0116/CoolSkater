/**
 * @file	ClearText.h
 * @brief	クリアテキストクラス定義
 * @author	morimoto
 */
#ifndef CLEARTEXT_H
#define CLEARTEXT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"
#include <DirectX11\Font\Font.h>
#include <vector>


class ClearText : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ClearText();

	/**
	 * デストラクタ
	 */
	virtual ~ClearText();

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

	bool m_IsTitle;
	bool m_IsTitleFlash;
	float m_TitleAlphaColor;
	std::vector<int> m_ScoreData;

};


#endif // !CLEARTEXT_H
