#pragma once
#include "PmdData.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class DescriptorMane;
class Device;

class PmdLoader
{
	// 読み込みデータ
	struct Data {
		//頂点情報
		std::shared_ptr<std::vector<pmd::Vertex>>vertex;
		//インデックス情報
		std::shared_ptr<std::vector<unsigned short>>index;
		//マテリアル情報
		std::shared_ptr<std::vector<pmd::Material>>material;
		//ボーン情報
		std::shared_ptr<std::vector<pmd::Born>>born;

		//頂点リソース
		int vRsc;
		//頂点送信データ
		void* vertexData;
		//インデックスリソース
		int iRsc;
		//マテリアル用リソース
		int mRsc;
		//ボーン用リソース
		int bRsc;
	};

public:
	// デストラクタ
	~PmdLoader();

	// インスタンス変数の取得
	static PmdLoader& Get(void) {
		static PmdLoader instance;
		return instance;
	}

	// 読み込み
	int Load(std::weak_ptr<Device>dev, const std::string& fileName);

	// 頂点情報の取得
	std::shared_ptr<std::vector<pmd::Vertex>> GetVertex(const std::string& fileName) {
		return data[fileName].vertex;
	}
	// インデックス情報の取得
	std::shared_ptr<std::vector<unsigned short>> GetIndex(const std::string& fileName) {
		return data[fileName].index;
	}
	// マテリアル情報の取得
	std::shared_ptr<std::vector<pmd::Material>> GetMaterial(const std::string& fileName) {
		return data[fileName].material;
	}
	// ボーン情報の取得
	std::shared_ptr<std::vector<pmd::Born>> GetBorn(const std::string& fileName) {
		return data[fileName].born;
	}
	// 頂点リソースIDの取得
	int& GetVertexRsc(const std::string& fileName) {
		return data[fileName].vRsc;
	}

private:
	// コンストラクタ
	PmdLoader();
	PmdLoader(const PmdLoader&)      = delete;
	void operator=(const PmdLoader&) = delete;

	// 頂点リソースの生成
	long CreateVertexRsc(std::weak_ptr<Device>dev, const std::string& fileName);

	// マップ
	long Map(const std::string& fileName);


	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// 読み込みデータ
	std::unordered_map<std::string, Data>data;
};
