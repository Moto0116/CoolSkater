/**
 * @file	Handrail.h
 * @brief	手すりクラス定義
 * @author	morimoto
 */
#ifndef HANDRAIL_H
#define HANDRAIL_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\GimmickBase\GimmickBase.h"


class Handrail : public GimmickBase
{
public:
	/**
	 * コンストラクタ
	 */
	Handrail();

	/**
	 * デストラクタ
	 */
	virtual ~Handrail();

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


#endif // !HANDRAIL_H
