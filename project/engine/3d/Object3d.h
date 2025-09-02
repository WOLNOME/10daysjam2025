#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include "MyMath.h"
#include "Model.h"
#include "AnimationModel.h"
#include "Shape.h"
#include "ModelFormat.h"
#include "WorldTransform.h"


//初期化用のタグ
struct ModelTag {};
struct AnimationModelTag {};
struct ShapeTag {};

class BaseCamera;
class SceneLight;
class Object3d {
	//オブジェクト3dマネージャーに公開
	friend class Object3dManager;
public://列挙型
	enum class ObjectKind {
		Model,				//通常モデル
		AnimationModel,		//アニメーションモデル
		Shape,				//単純形状

		kMaxNumObjectKind,
	};

public://構造体
	struct FlagForGPU {
		uint32_t isActiveLights;
		uint32_t isActiveEnvironment;
	};
	struct FlagResource {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		FlagForGPU* data;
	};

public://メンバ関数
	Object3d();
	~Object3d();

	//モデル初期化
	void Initialize(ModelTag, const std::string& name, const std::string& filePath);
	//アニメーションモデル初期化
	void Initialize(AnimationModelTag, const std::string& name, const std::string& filePath);
	//形状初期化
	void Initialize(ShapeTag, const std::string& name, Shape::ShapeKind kind);

private://マネージャーへの委託処理
	//更新処理
	void Update();
	//描画
	void Draw(BaseCamera* _camera, SceneLight* _sceneLight);

public://setter
	//テクスチャ
	void SetTexture(int32_t _textureHandle) { textureHandle_ = _textureHandle; }
	//環境光テクスチャ
	void SetEnvironmentLightTextureHandle(int32_t _textureHandle) { environmentLightTextureHandle_ = _textureHandle; }
	//表示するか
	void SetIsDisplay(bool _isDisplay) { isDisplay_ = _isDisplay; };
	//ライトの処理をするか
	void SetIsLightProcess(bool _isLightProcess) { isLightProcess_ = _isLightProcess; }

	//新しいアニメーションをセット
	void SetNewAnimation(const std::string& _name, const std::string& _filePath);
	//現在のアニメーションを変更
	void SetCurrentAnimation(const std::string& _name);

private://非公開メンバ関数
	FlagResource CreateFlagResource();

private://描画に利用するリソース
	//テクスチャ
	int32_t textureHandle_ = EOF;
	//環境光用のテクスチャ
	int32_t environmentLightTextureHandle_ = EOF;
public://外部編集用メンバ変数
	WorldTransform worldTransform;

private://メンバ変数
	//名前
	std::string name_;

	//モデル
	Model* model_ = nullptr;
	//アニメーションモデル
	std::unique_ptr<AnimationModel> animationModel_ = nullptr;
	//形状
	std::unique_ptr<Shape> shape_ = nullptr;

	//オブジェクトの種類
	ObjectKind objKind_;

	//ライト有無用リソース
	FlagResource flagResource_;

	//描画するか
	bool isDisplay_ = true;
	//ライトの処理をするか
	bool isLightProcess_ = true;

};

