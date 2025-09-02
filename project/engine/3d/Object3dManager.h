#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include <array>
#include <unordered_map>

class BaseCamera;
class SceneLight;
class Object3d;
class Object3dManager {
public:
	enum class NameGPS {
		None,			//通常
		Animation,		//アニメーション
		SkyBox,			//スカイボックス

		kMaxNumNameGPS,	//最大数
	};

private://シングルトン
	static Object3dManager* instance;

	Object3dManager() = default;//コンストラクタ隠蔽
	~Object3dManager() = default;//デストラクタ隠蔽
	Object3dManager(Object3dManager&) = delete;//コピーコンストラクタ封印
	Object3dManager& operator=(Object3dManager&) = delete;//コピー代入演算子封印
public://シングルトン
	static Object3dManager* GetInstance();
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//終了
	void Finalize();

	//オブジェクトをコンテナに登録
	void RegisterObject(const std::string& name, Object3d* object);
	//登録されたオブジェクトを削除
	void DeleteObject(const std::string& name);

	//名前を決める関数
	std::string GenerateName(const std::string& name);

	//描画前設定
	void SettingCommonDrawing(NameGPS index = NameGPS::None);
	//アニメーション専用コンピュートシェーダー前設定
	void SettingAnimationCS();

public://setter
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }
	void SetSceneLight(SceneLight* _light) { light_ = _light; }

private://非公開メンバ関数
	//グラフィックスパイプライン
	void GenerateGraphicsPipeline();
	//コンピュートパイプライン
	void GenerateComputePipeline();

	//通常のPSO設定
	void NormalPSOOption();
	//アニメーション用のPSO設定
	void AnimationPSOOption();
	//スカイボックス用のPSO設定
	void SkyBoxPSOOption();

private://借用インスタンス
	BaseCamera* camera_ = nullptr;
	SceneLight* light_ = nullptr;

private://メンバ変数
	//ルートシグネチャ
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, (int)NameGPS::kMaxNumNameGPS> rootSignature_;
	//グラフィックスパイプライン
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, (int)NameGPS::kMaxNumNameGPS> graphicsPipelineState_;

	//コンピュートルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> computeRootSignature_ = nullptr;
	//コンピュートパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> computePipelineState_ = nullptr;

	//オブジェクトのコンテナ
	std::unordered_map<std::string, Object3d*> objects_;
};

