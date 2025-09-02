#pragma once
#include "MyMath.h"
#include <wrl.h>
#include <d3d12.h>
#include <list>

class BaseCamera;
class LineManager {
private://構造体
	//頂点データ
	struct VertexForGPU {
		Vector4 position;
		float vertexIndex;
	};
	//座標変換行列データ(インスタンスごとに変えられるデータ)
	struct LineForGPU {
		Vector4 start;
		Vector4 end;
		Vector4 color;
	};
	//線リソース作成用データ型
	struct LineResource {
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		VertexForGPU* vertexData;
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
		LineForGPU* instancingData;
		uint32_t srvIndex;
	};
	//ライン情報(外から書き換える用)
	struct Line {
		Vector4 start;
		Vector4 end;
		Vector4 color;
	};
private://コンストラクタ等の隠蔽
	static LineManager* instance;

	LineManager() = default;//コンストラクタ隠蔽
	~LineManager() = default;//デストラクタ隠蔽
	LineManager(LineManager&) = delete;//コピーコンストラクタ封印
	LineManager& operator=(LineManager&) = delete;//コピー代入演算子封印
public:
	//シングルトンインスタンスの取得
	static LineManager* GetInstance();
public://メンバ関数
	//初期化
	void Initialize();
	//描画
	void Draw();
	//終了
	void Finalize();

	//ライン作成関数
	void CreateLine(Vector3 start, Vector3 end, Vector4 color);

public://setter
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }

private://非公開メンバ関数
	//グラフィックスパイプライン
	void GenerateGraphicsPipeline();

	//ラインリソース作成関数
	LineResource MakeLineResource();
	//インスタンシングリソースにSRV用の設定をする
	void SettingSRV();

private://借用インスタンス
	BaseCamera* camera_ = nullptr;

private://メンバ変数
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	//線のコンテナ
	std::list<Line> lines_;
	//ライン用リソース
	LineResource lineResource_;

};

