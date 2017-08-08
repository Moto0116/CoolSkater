#ifndef OVERTEXT_H
#define OVERTEXT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"
#include <DirectX11\Font\Font.h>
#include <vector>


class OverText : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	OverText();

	/**
	 * デストラクタ
	 */
	virtual ~OverText();

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


	bool m_IsTitle;
	bool m_IsTitleFlash;
	float m_TitleAlphaColor;

	bool m_IsGame;
	bool m_IsGameFlash;
	float m_GameAlphaColor;
	std::vector<int> m_ScoreData;

};


#endif // !OVERTEXT_H
