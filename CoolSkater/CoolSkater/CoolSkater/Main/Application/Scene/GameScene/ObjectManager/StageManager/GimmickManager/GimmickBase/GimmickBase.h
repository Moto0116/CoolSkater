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

#include "..\GimmickManager.h"


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
	 * @param[in] _pos 追加するギミックの座標
	 */
	virtual void AddGimmick(D3DXVECTOR2 _pos);

	/**
	 * ギミックの追加
	 * @param[in] _pos 追加するギミックの座標
	 */
	virtual void AddGimmick(D3DXVECTOR2 _pos, int _type);

	/**
	 * ギミックの追加
	 */
	virtual void RemoveGimmick();

	/**
	 * ギミックの削除
	 * @param[in] _pos 削除するギミックの座標
	 */
	virtual void RemoveGimmick(D3DXVECTOR2 _pos);

	/**
	 * ギミックの追加
	 * @param[in] _pos 追加するギミックの座標
	 */
	virtual void RemoveGimmick(D3DXVECTOR2 _pos, int _type);

	/**
	 * 矩形との衝突判定
	 * @param[in] _pPos 矩形座標
	 * @param[in] _pSize 矩形サイズ
	 * @todo 名前が思いつかなかったのでとりあえずこのまま
	 */
	virtual GimmickManager::GIMMICK_TYPE Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize);

	/**
	 * 矩形との衝突判定
	 * @param[in] _pPos 矩形座標
	 * @param[in] _pSize 矩形サイズ
	 * @param[in] _pOutPos 衝突したギミックの座標
	 * @param[in] _pOutSize 衝突したギミックのサイズ
	 * @todo 名前が思いつかなかったのでとりあえずこのまま
	 */
	virtual GimmickManager::GIMMICK_TYPE Collision(
		D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
		D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize);

	virtual GimmickManager::GIMMICK_TYPE DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize);


};


#endif // !GIMMICKBASE_H
