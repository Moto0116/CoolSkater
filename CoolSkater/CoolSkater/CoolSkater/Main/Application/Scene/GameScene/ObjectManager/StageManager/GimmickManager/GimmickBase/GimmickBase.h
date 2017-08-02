/**
 * @file	GimmickBase.h
 * @brief	ギミック基底クラス定義
 * @author	morimoto
 */
#ifndef GIMMICKBASE_H
#define GIMMICKBASE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Main\Object2DBase\Object2DBase.h"


class GimmickBase : public Object2DBase
{
public:
	/**
	 * コンストラクタ
	 */
	GimmickBase();

	/**
	 * デストラクタ
	 */
	virtual ~GimmickBase();

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

	/**
	 * ギミックの追加
	 * @param[in] _pos 削除するギミックの座標
	 */
	virtual void AddGimmick(int _pos);

	/**
	 * ギミックの削除
	 * @param[in] _pos 削除するギミックの座標
	 */
	virtual void RemoveGimmick(int _pos);

};


#endif // !GIMMICKBASE_H
