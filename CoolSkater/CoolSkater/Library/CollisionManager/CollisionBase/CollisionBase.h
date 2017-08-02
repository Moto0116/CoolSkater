﻿/**
 * @file	CollisionBase.h
 * @brief	衝突判定オブジェクト基底クラス定義
 * @author	morimoto
 */
#ifndef COLLISIONBASE_H
#define COLLISIONBASE_H


namespace Lib
{
	/**
	 * 衝突判定オブジェクト基底クラス
	 */
	class CollisionBase
	{
	public:
		/**
		 * 衝突判定オブジェクトのフィルタ
		 */
		enum COLLISION_FILTER
		{
			CHARACTER	= 1,	//!< キャラクターオブジェクト
			STAGE		= 2,	//!< ステージオブジェクト
			BACKGROUND  = 4		//!< バックグラウンドオブジェクト
		};


		/**
		 * コンストラクタ
		 */
		CollisionBase();

		/**
		 * デストラクタ
		 */
		virtual ~CollisionBase();

		/**
		 * IDを取得する
		 * @return あたり判定オブジェクトのID
		 */
		inline unsigned int GetID() const
		{
			return m_CollisionID;
		}

		/**
		 * フィルタを取得する
		 * @return あたり判定オブジェクトのフィルタ
		 */
		inline unsigned int GetFilter() const
		{
			return m_Filter;
		}


	protected:
		unsigned int	m_CollisionID;	//!< あたり判定オブジェクトのID
		unsigned int	m_Filter;		//!< あたり判定を行うオブジェクトの判別するフィルタ

	private:
		static unsigned int m_CollisionBaseCount;	//!< あたり判定オブジェクトの生成数のカウンタ

	};
}


#endif // !COLLISIONBASE_H
