/**
 * @file	TitleBackground.h
 * @brief	タイトルバックグラウンドクラス定義
 * @author	morimoto
 */
#ifndef TITLEBACKGROUND_H
#define TITlEBACKGROUND_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class TitleBackground : public Object2DBase
{
public:
	/**
	 * コンストラクタ 
	 */
	TitleBackground();

	/**
	 * デストラクタ
	 */
	virtual ~TitleBackground();

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
	enum BACKGROUND_ID
	{
		BACKGROUND_1,
		BACKGROUND_2,
		BACKGROUND_3,
		BACKGROUND_MAX
	};
};


#endif // !TITlEBACKGROUND_H
