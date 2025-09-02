#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "MyMath.h"
#include "Handle.h"

class Sprite {
	//スプライトマネージャーに公開
	friend class SpriteManager;
public://列挙体
	//描画順タグ(各レイヤーは今のところ3つずつ←必要に応じて増やす)
	enum class Order {
		Back0,
		Back1,
		Back2,
		Front0,
		Front1,
		Front2,

		kMaxOrderNum,
	};
	//テクスチャの種類
	enum class TextureKind {
		Normal,
		Text,

		kMaxTextureKindNum,
	};

private://構造体
	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//マテリアルデータ
	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
		int32_t isTexture;
	};
	//座標変換行列データ
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	//GPU送信用リソース
	struct ResourceForGPU {
		//頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		VertexData* vertexData = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
		//インデックスリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
		uint32_t* indexData = nullptr;
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
		//マテリアルリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
		Material* materialData = nullptr;
		//座標変換行列リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
		TransformationMatrix* transformationMatrixData = nullptr;
		//テクスチャ用SRV番号
		uint32_t srvIndex = 0u;
	};

public://メンバ関数
	~Sprite();
	//初期化
	void Initialize(const std::string& _name, const Order& _order, uint32_t _textureHandle);
	void Initialize(const std::string& _name, const Order& _order);
	//描画
	void Draw();

	//デバッグ用ImGui
	void DebugWithImGui();

public://getter
	const Vector2& GetPosition()const { return position; }
	float GetRotation()const { return rotation; }
	const Vector2& GetSize()const { return size; }
	const Vector4& GetColor()const { return resource_.materialData->color; }
	const Vector2& GetAnchorPoint()const { return anchorPoint; }
	bool GetFlipX()const { return isFlipX_; }
	bool GetFlipY()const { return isFlipY_; }
	const Vector2& GetTextureLeftTop()const { return textureLeftTop; }
	const Vector2& GetTextureSize()const { return textureSize; }


public://setter
	void SetPosition(const Vector2& position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const Vector2& size) { this->size = size; }
	void SetColor(const Vector4& color) { resource_.materialData->color = color; }
	void SetAnchorPoint(const Vector2& anchorPoint) { this->anchorPoint = anchorPoint; }
	void SetFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetFlipY(bool isFlipY) { isFlipY_ = isFlipY; }
	void SetTextureLeftTop(const Vector2& leftTop) { textureLeftTop = leftTop; }
	void SetTextureSize(const Vector2& size) { textureSize = size; }
	void SetShakeOffset(const Vector2& _offset) { shakeOffset_ = _offset; }
	void SetIsDisplay(bool _isDisplay) { isDisplay_ = _isDisplay; }
	void SetTexture(Handle _textureHandle) { textTextureHandle_ = _textureHandle; }

private://非公開メンバ変数
	ResourceForGPU CreateResourceForGPU();

	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private://メンバ変数
	//名前
	std::string name_;

	//リソース
	ResourceForGPU resource_;

	//描画順タグ
	Order order_ = Order::kMaxOrderNum;

	//テクスチャ
	uint32_t textureHandle_ = 0u;
	Handle textTextureHandle_;
	//テクスチャの種類
	TextureKind textureKind_ = TextureKind::kMaxTextureKindNum;

	//パラメーター
	Vector2 position = { 0.0f,0.0f };
	float rotation = 0.0f;
	Vector2 size = { 640.0f,360.0f };
	Vector2 anchorPoint = { 0.0f,0.0f };
	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;
	//テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };

	Vector2 shakeOffset_ = {};

	bool isDisplay_ = true;

};

