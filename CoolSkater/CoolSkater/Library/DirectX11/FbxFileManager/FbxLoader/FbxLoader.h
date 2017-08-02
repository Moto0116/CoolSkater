﻿/**
 * @file   FbxLoader.h
 * @brief  FbxLoaderクラスの定義
 * @author morimoto
 */
#ifndef LIB_FBXLOADER_H
#define LIB_FBXLOADER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <D3D11.h>
#include <fbxsdk.h>

#include "DirectX11\GraphicsDevice\GraphicsDevice.h"
#include "../FbxModel/FbxModel.h"


namespace Lib
{
	/**
	 * Fbxの読み込み処理を管理するクラス
	 */
	class FbxLoader
	{
	public:
		/**
		 * FbxLoaderクラスのコンストラクタ
		 * @param[in] _pGraphicsDevice テクスチャ読み込みに使用するグラフィックデバイス
		 */
		FbxLoader(GraphicsDevice* _pGraphicsDevice);

		/**
		 * FbxLoaderクラスのデストラクタ
		 */
		~FbxLoader();

		/**
		 * FbxLoaderを初期化する
		 */
		bool Initialize();

		/**
		 * FbxLoaderを解放する
		 */
		void Finalize();

		/**
		 * Fbxモデルを読み込む
		 * @param[out] _pModel 読み込みに成功した場合はモデルデータが格納される、失敗したらnullptr
		 * @param[in] _pFileName 読み込むモデルのファイルパス
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadFbxModel(FbxModel* _pModel, LPCTSTR _pFileName);

	private:
		static const D3DXCOLOR m_DefaultAmbient;

		/**
		 * fbxsdkの管理オブジェクトを初期化する
		 * @return 初期化に成功したらtrue
		 */
		bool InitFbxManager();

		/**
		* Fbxのシーンをまとめるオブジェクトを初期化する
		* @return 初期化に成功したらtrue
		*/
		bool InitFbxScene();

		/**
		 * Fbxのインポーターを初期化する
		 * @return 初期化に成功したらtrue
		 */
		bool InitFbxImporter();

		/**
		 * Fbxの入出力を設定するオブジェクトを初期化する
		 * @return 初期化に成功したらtrue
		 */
		bool InitFbxIOSettings();

		/**
		 * fbxsdkの管理オブジェクトを解放する
		 */
		void ReleaseFbxManager();

		/**
		 * Fbxのシーンをまとめるオブジェクトを解放する
		 */
		void ReleaseFbxScene();

		/**
		 * Fbxのインポーターを解放する
		 */
		void ReleaseFbxImporter();

		/**
		 * Fbxの入出力を設定するオブジェクトを解放する
		 */
		void ReleaseFbxIOSettings();

		/**
		 * Fbxのノードを再帰的に読み込む関数
		 * @param[in] _pFbxNode 読み込むノード
		 */
		void RecursiveNode(FbxNode* _pFbxNode);

		/**
		 * メッシュを読み込む関数
		 * @param[in] _pAttribute メッシュの情報が格納されているFbxNodeAttribute
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMesh(FbxNodeAttribute* _pAttribute);

		/**
		 * メッシュの頂点情報を読み込む
		 * @param[in] _pMesh 頂点情報を読み込むメッシュ
		 * @param[out] _pMeshData 読み込んだ頂点情報を出力するMESH_DATA構造体のポインタ
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMeshVertexData(FbxMesh* _pMesh, FbxModel::MESH_DATA* _pMeshData);

		/**
		 * メッシュの法線情報を読み込む
		 * @param[in] _pMesh 法線情報を読み込むメッシュ
		 * @param[out] _pMeshData 読み込んだ法線情報を出力するMESH_DATA構造体のポインタ
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMeshNormalData(FbxMesh* _pMesh, FbxModel::MESH_DATA* _pMeshData);

		/**
		 * メッシュのテクスチャ情報を読み込む
		 * @param[in] _pMesh テクスチャ情報を読み込むメッシュ
		 * @param[out] _pMeshData 読み込んだテクスチャ情報を出力するMESH_DATA構造体のポインタ
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMeshTextureData(FbxMesh* _pMesh, FbxModel::MESH_DATA* _pMeshData);

		/**
		 * メッシュのマテリアル情報を読み込む
		 * @param[in] _pMesh マテリアル情報を読み込むメッシュ
		 * @param[out] _pMeshData 読み込んだマテリアル情報を出力するMESH_DATA構造体のポインタ
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMeshMaterialData(FbxMesh* _pMesh, FbxModel::MESH_DATA* _pMeshData);

		/**
		 * メッシュのアニメーション情報を読み込む
		 * @param[in] _pMesh アニメーション情報を読み込むメッシュ
		 * @param[out] _pMeshData 読み込んだアニメーション情報を出力するMESH_DATA構造体のポインタ
		 * @return 読み込みに成功したらtrue
		 */
		bool LoadMeshAnimationData(FbxMesh* _pMesh, FbxModel::MESH_DATA* _pMeshData);

		/**
		 * ファイルから取得したメッシュデータを描画に使用できるようにする
		 */
		bool MeshDataConvert(FbxModel::MESH_DATA* _pMeshData);

		/**
		 * テクスチャの名前をマテリアルから取得する関数
		 * @param[in] _pMaterial テクスチャ名を取得するマテリアル
		 * @param[in] _pMatFlag 取得するテクスチャのフラグ
		 * @param[out] _pOutFileName 取得したテクスチャ名が格納される
		 * @param[out] _pOutUvSetName 取得したテクスチャに紐づくUVセットネームが入る
		 * @param[out] _pOutCount 読み込んだテクスチャの数が格納される
		 */
		void GetTextureName(
			fbxsdk::FbxSurfaceMaterial* _pMaterial,
			LPCTSTR _pMatFlag,
			std::vector<LPCTSTR>* _pOutFileName,
			std::vector<fbxsdk::FbxString>* _pOutUvSetName,
			int* _pOutCount);


		GraphicsDevice*			m_pGraphicsDevice;
		fbxsdk::FbxManager*		m_pFbxManager;
		fbxsdk::FbxScene*		m_pFbxScene;
		fbxsdk::FbxImporter*	m_pFbxImporter;
		fbxsdk::FbxIOSettings*	m_pFbxIOSettings;
		FbxModel*				m_pModel;

	};
}


#endif // !LIB_FBXMODEL_H
