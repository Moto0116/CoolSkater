#ifndef OVERBACKGROUND_H
#define OVERBACKGROUND_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class OverBackground : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	OverBackground();

	/**
	 * デストラクタ
	 */
	virtual ~OverBackground();

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


#endif // !OVERBACKGROUND_H
