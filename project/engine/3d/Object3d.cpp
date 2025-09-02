#include "Object3d.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "MainRender.h"
#include "Object3dManager.h"
#include "TextureManager.h"
#include "GPUDescriptorManager.h"
#include "ModelManager.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include <fstream>
#include <sstream>
#include <cassert>

Object3d::Object3d() {
	//フラッグリソースの作成
	flagResource_ = CreateFlagResource();
}

Object3d::~Object3d() {
	//マネージャーから削除
	Object3dManager::GetInstance()->DeleteObject(name_);
}

void Object3d::Initialize(ModelTag, const std::string& name, const std::string& filePath) {
	//名前
	name_ = name;
	//モデルマネージャーでモデルを生成
	ModelManager::GetInstance()->LoadModel(filePath, ModelFormat::OBJ);
	//モデルマネージャーから検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);

	objKind_ = ObjectKind::Model;

	//ワールドトランスフォームの初期化
	worldTransform.Initialize();

	//マネージャーに登録
	Object3dManager::GetInstance()->RegisterObject(name_, this);

}

void Object3d::Initialize(AnimationModelTag, const std::string& name, const std::string& filePath) {
	//名前
	name_ = name;
	//アニメーションモデルの生成と初期化
	animationModel_ = std::make_unique<AnimationModel>();
	animationModel_->Initialize(filePath, ModelFormat::GLTF);

	objKind_ = ObjectKind::AnimationModel;

	//ワールドトランスフォームの初期化
	worldTransform.Initialize();

	//マネージャーに登録
	Object3dManager::GetInstance()->RegisterObject(name_, this);

}

void Object3d::Initialize(ShapeTag, const std::string& name, Shape::ShapeKind kind) {
	//名前
	name_ = name;
	//形状の生成と初期化
	shape_ = std::make_unique<Shape>();
	shape_->Initialize(kind);

	objKind_ = ObjectKind::Shape;

	//ワールドトランスフォームの初期化
	worldTransform.Initialize();

	//マネージャーに登録
	Object3dManager::GetInstance()->RegisterObject(name_, this);

}

void Object3d::Update() {
	//ワールドトランスフォームの更新
	worldTransform.UpdateMatrix();
	//オブジェクトの種類ごとの処理
	switch (objKind_) {
	case Object3d::ObjectKind::Model:
		//何もなし
		break;
	case Object3d::ObjectKind::AnimationModel:
		//アニメーション反映処理
		animationModel_->Update();
		//CS前処理（スキニング）
		animationModel_->SettingCSPreDraw();
		break;
	case Object3d::ObjectKind::Shape:
		//形状の更新処理
		shape_->Update();
		break;
	default:
		break;
	}
}

void Object3d::Draw(BaseCamera* _camera, SceneLight* _sceneLight) {
	//オブジェクトの種類ごとの処理
	switch (objKind_) {
	case ObjectKind::Model:
		//通常モデル用共通描画の設定
		Object3dManager::GetInstance()->SettingCommonDrawing(Object3dManager::NameGPS::None);
		//シーンライト有無設定
		flagResource_.data->isActiveLights = (isLightProcess_) ? true : false;

		//lightFlagCbufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, flagResource_.resource->GetGPUVirtualAddress());

		//SceneLightCBufferの場所を設定
		if (flagResource_.data->isActiveLights) {
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, _sceneLight->GetSceneLightConstBuffer()->GetGPUVirtualAddress());
		}

		//WorldTransformCBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.GetConstBuffer()->GetGPUVirtualAddress());

		//CameraからビュープロジェクションCBufferの場所設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, _camera->GetViewProjectionConstBuffer()->GetGPUVirtualAddress());

		//Cameraからカメラ座標CBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, _camera->GetCameraPositionConstBuffer()->GetGPUVirtualAddress());

		//環境光テクスチャの設定
		if (environmentLightTextureHandle_ != EOF) {
			flagResource_.data->isActiveEnvironment = true;
			//PSにテクスチャ情報を送る
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU(environmentLightTextureHandle_));
		}
		else {
			flagResource_.data->isActiveEnvironment = false;
		}

		//モデルを描画する
		model_->Draw(0, 3, 1, textureHandle_);
		break;
	case ObjectKind::AnimationModel:
		//アニメーションモデル用共通描画の設定
		Object3dManager::GetInstance()->SettingCommonDrawing(Object3dManager::NameGPS::Animation);

		//シーンライト有無設定
		flagResource_.data->isActiveLights = (isLightProcess_) ? true : false;

		//lightFlagCbufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, flagResource_.resource->GetGPUVirtualAddress());

		//SceneLightCBufferの場所を設定
		if (flagResource_.data->isActiveLights) {
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, _sceneLight->GetSceneLightConstBuffer()->GetGPUVirtualAddress());
		}

		//WorldTransformCBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.GetConstBuffer()->GetGPUVirtualAddress());

		//CameraからビュープロジェクションCBufferの場所設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, _camera->GetViewProjectionConstBuffer()->GetGPUVirtualAddress());

		//Cameraからカメラ座標CBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, _camera->GetCameraPositionConstBuffer()->GetGPUVirtualAddress());

		//環境光テクスチャの設定
		if (environmentLightTextureHandle_ != EOF) {
			flagResource_.data->isActiveEnvironment = true;
			//PSにテクスチャ情報を送る
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU(environmentLightTextureHandle_));
		}
		else {
			flagResource_.data->isActiveEnvironment = false;
		}

		//モデルを描画する
		animationModel_->Draw(0, 3, 1, textureHandle_);

		//CS描画後処理(スキニング)
		animationModel_->SettingCSPostDraw();

		break;
	case ObjectKind::Shape:
		//描画前設定
		if (shape_->GetShapeKind() == Shape::kSkyBox) {
			//SkyBoxの描画設定
			Object3dManager::GetInstance()->SettingCommonDrawing(Object3dManager::NameGPS::SkyBox);
		}
		else {
			//通常の描画設定
			Object3dManager::GetInstance()->SettingCommonDrawing(Object3dManager::NameGPS::None);
		}

		//シーンライト有無設定
		flagResource_.data->isActiveLights = (isLightProcess_) ? true : false;
		//lightFlagCbufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, flagResource_.resource->GetGPUVirtualAddress());
		//SceneLightCBufferの場所を設定
		if (flagResource_.data->isActiveLights) {
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, _sceneLight->GetSceneLightConstBuffer()->GetGPUVirtualAddress());
		}
		//WorldTransformCBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.GetConstBuffer()->GetGPUVirtualAddress());
		//CameraからビュープロジェクションCBufferの場所設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, _camera->GetViewProjectionConstBuffer()->GetGPUVirtualAddress());
		//Cameraからカメラ座標CBufferの場所を設定
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, _camera->GetCameraPositionConstBuffer()->GetGPUVirtualAddress());
		//環境光テクスチャの設定
		if (environmentLightTextureHandle_ != EOF) {
			flagResource_.data->isActiveEnvironment = true;
			//PSにテクスチャ情報を送る
			MainRender::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU(environmentLightTextureHandle_));
		}
		else {
			flagResource_.data->isActiveEnvironment = false;
		}

		//形状を描画する
		shape_->Draw(0, 3, 1, textureHandle_);

		break;
	default:
		break;
	}
}

void Object3d::SetNewAnimation(const std::string& _name, const std::string& _filePath) {
	//アニメーションモデル以外のオブジェクトで初期化していた場合警告
	if (objKind_ != ObjectKind::AnimationModel) {
		assert(0 && "アニメーションモデル以外の初期化を確認しました。");
		return;
	}
	//AnimationModelに通達
	animationModel_->SetNewAnimation(_name, _filePath);
}

void Object3d::SetCurrentAnimation(const std::string& _name) {
	//アニメーションモデル以外のオブジェクトで初期化していた場合警告
	if (objKind_ != ObjectKind::AnimationModel) {
		assert(0 && "アニメーションモデル以外の初期化を確認しました。");
		return;
	}
	//AnimationModelに通達
	animationModel_->SetCurrentAnimation(_name);
}

Object3d::FlagResource Object3d::CreateFlagResource() {
	FlagResource result;
	//リソース作成
	result.resource = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(FlagForGPU));
	//リソースにマッピング
	result.resource->Map(0, nullptr, reinterpret_cast<void**>(&result.data));
	//データに書き込み
	result.data->isActiveLights = false;
	result.data->isActiveEnvironment = false;

	return result;
}
