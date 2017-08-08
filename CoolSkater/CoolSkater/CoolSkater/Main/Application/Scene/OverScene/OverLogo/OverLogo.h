#ifndef OVERLOGO_H
#define OVERLOGO_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class OverLogo : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	OverLogo();

	/**
	 * デストラクタ
	 */
	virtual ~OverLogo();

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


#endif // !OVERLOGO_H
