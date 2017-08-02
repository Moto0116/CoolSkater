/**
 * @file	Vertex.h
 * @brief	頂点クラス定義
 * @author	morimoto
 */
#ifndef LIB_VERTEX_H
#define LIB_VERTEX_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\IVertex\IVertex.h"


namespace Lib
{
	/**
	 * 頂点クラス
	 */
	class Vertex : public IVertex
	{
	public:
		/**
		 * コンストラクタ
		 */
		Vertex();

		/**
		 * デストラクタ
		 */
		virtual ~Vertex();

	};
}


#endif // !LIB_VERTEX_H
