/**
 * @file	TitleLogo.h
 * @brief	タイトルロゴクラス定義
 * @author	morimoto
 */
#ifndef TITLELOGO_H
#define TITLELOGO_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"

class TitleLogo : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	TitleLogo();

	/**
	 * デストラクタ
	 */
	virtual ~TitleLogo();

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

};


#endif // !TITLELOGO_H
