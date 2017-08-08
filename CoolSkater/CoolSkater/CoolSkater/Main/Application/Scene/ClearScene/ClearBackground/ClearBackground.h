/**
 * @file	ClearBackground.h
 * @brief	クリア背景クラス定義
 * @author	morimoto
 */
#ifndef CLEARBACKGROUND_H
#define CLEARBACKGROUND_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class ClearBackground : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	ClearBackground();

	/**
	 * デストラクタ
	 */
	virtual ~ClearBackground();

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


#endif // !CLEARBACKGROUND_H
