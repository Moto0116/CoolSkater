/**
 * @file	CollisionManager.h
 * @brief	当たり判定管理クラス定義
 * @author	morimoto
 */
#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <vector>

#include "SingletonBase\SingletonBase.h"
#include "..\ObjectManager\StageManager\GimmickManager\GimmickManager.h"
#include "..\ObjectManager\StageManager\GimmickManager\GimmickBase\GimmickBase.h"


class CollisionManager : public Lib::SingletonBase<CollisionManager>
{
public:
	friend SingletonBase<CollisionManager>;

	void AddGimmick(GimmickBase* _pGimmick);
	void RemoveGimmicks();

	GimmickManager::GIMMICK_TYPE GimmickCollision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize);
	GimmickManager::GIMMICK_TYPE GimmickCollision(
		D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
		D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize);

	GimmickManager::GIMMICK_TYPE DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize);

private:
	CollisionManager();
	~CollisionManager();

	std::vector<GimmickBase*> m_pGimmicks;

};


#endif // !COLLISIONMANAGER_H
