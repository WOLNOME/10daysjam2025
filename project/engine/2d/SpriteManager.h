#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <unordered_map>
#include <string>

class Sprite;
class SpriteManager {
private://コンストラクタ等の隠蔽
	static SpriteManager* instance;

	SpriteManager() = default;//コンストラクタ隠蔽
	~SpriteManager() = default;//デストラクタ隠蔽
	SpriteManager(SpriteManager&) = delete;//コピーコンストラクタ封印
	SpriteManager& operator=(SpriteManager&) = delete;//コピー代入演算子封印
public:
	//シングルトンインスタンスの取得
	static SpriteManager* GetInstance();
public://メンバ関数
	//初期化
	void Initialize();
	//バック描画
	void BackDraw();
	//フロント描画
	void FrontDraw();
	//終了
	void Finalize();

	//デバッグ処理
	void DebugWithImGui();

	//スプライトをコンテナに登録
	void RegisterSprite(const std::string& name, Sprite* sprite);
	//登録されたスプライトを削除
	void DeleteSprite(const std::string& name);

	//名前生成関数
	std::string GenerateName(const std::string& name);

private://非公開メンバ関数
	//グラフィックスパイプラインの生成
	void GenerateGraphicsPipeline();

private://メンバ変数
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	//スプライトのコンテナ
	std::unordered_map<std::string, Sprite*> sprites_;
};

