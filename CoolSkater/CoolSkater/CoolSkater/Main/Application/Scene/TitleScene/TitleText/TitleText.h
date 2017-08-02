/**
 * @file	TitleText.h
 * @brief	タイトルテキストクラス定義
 * @author	morimoto
 */
#ifndef TITLETEXT_H
#define TITLETEXT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class TitleText : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	TitleText();

	/**
	 * デストラクタ 
	 */
	virtual ~TitleText();

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
	int	m_TextureIndex2;

};


#endif // TITLETEXT_H
