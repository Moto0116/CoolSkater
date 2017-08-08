/**
 * @file	ObstacleCar.h
 * @brief	障害物車クラス定義
 * @author	morimoto
 */

#ifndef OBSTACLECAR_H
#define OBSTACLECAR_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\GimmickBase\GimmickBase.h"

#include <vector>


class ObstacleCar : public GimmickBase
{
public:
	enum TYPE
	{
		CAR_TYPE = 0,	// 通常車タイプ.
		BUS_TYPE = 1,	// バスタイプ.
		LIMO_TYPE = 2,	// リムジンタイプ.
		CAR_SCRAPPED_TYPE = 3,	// 通常車(廃車)タイプ.
		BUS_SCRAPPED_TYPE = 4,	// バス(廃車)タイプ.
		CAR2_TYPE = 5,	// 通常車2タイプ.
		CAR3_TYPE = 6,	// 通常車3タイプ.
		LIMO_SCRAPPED_TYPE = 7,	// リムジン(廃車)タイプ.
		TYPE_MAX 
	};

	/**
	 * コンストラクタ
	 */
	ObstacleCar();

	/**
	 * デストラクタ
	 */
	virtual ~ObstacleCar();

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
	 * ギミックの削除
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

private:
	struct GIMMICK_DATA
	{
		GIMMICK_DATA(D3DXVECTOR2 _pos, int _type, float _angle, bool _isDestroy) :
			Pos(_pos),
			GimmickType(_type),
			Angle(_angle),
			IsDestroy(_isDestroy)
		{
		}

		bool operator == (GIMMICK_DATA _data)
		{
			if (_data.Pos == Pos &&
				_data.GimmickType == GimmickType)
			{
				return true;
			}

			return false;
		}

		D3DXVECTOR2 Pos;
		int GimmickType;
		float Angle;
		bool IsDestroy;
	};

	std::vector<GIMMICK_DATA> m_GimmickData;

	int m_TextureIndex2;
	int m_TextureIndex3;
	int m_TextureIndex4;
	int m_TextureIndex5;
	int m_TextureIndex6;
	int m_TextureIndex7;
	int m_TextureIndex8;

};


#endif // OBSTACLECAR_H
