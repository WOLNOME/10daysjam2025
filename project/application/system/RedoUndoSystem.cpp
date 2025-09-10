#include "RedoUndoSystem.h"
#include <SpriteManager.h>
#include <TextTextureManager.h>

void RedoUndoSystem::Initialize(const CsvMapData& initialMap) {
	//初期マップの読み込み
	redoUndoList_.push_back(initialMap);

	//テキスト
	TextParam textParam;
	textParam.text = L"[Esc]:ポーズ";
	textParam.font = Font::UDDegitalN_B;
	textParam.fontStyle = FontStyle::Normal;
	textParam.size = 24.0f;
	textParam.color = { 1.0f,1.0f,1.0f,1.0f };
	textTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
	textSprite_ = std::make_unique<Sprite>();
	textSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("redoUndoText"), Sprite::Order::Front2);
	textSprite_->SetTexture(textTexture_);
	textSprite_->SetPosition({ 10,10 });
	textSprite_->SetAnchorPoint({ 0.0f,0.0f });
	textSprite_->SetIsDisplay(true);

	//操作UI
	TextParam operationParam;
	operationParam.text = L"[WASD]:移動  \n[Z]:ひとつ戻る\n[Y]:ひとつ進む\n[Q]:犬を操作\n[E]猿を操作";
	operationParam.font = Font::UDDegitalN_B;
	operationParam.fontStyle = FontStyle::Normal;
	operationParam.size = 24.0f;
	operationParam.color = { 1.0f,1.0f,1.0f,1.0f };
	operationTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(operationParam);
	operationSprite_ = std::make_unique<Sprite>();
	operationSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("operationUI"), Sprite::Order::Front2);
	operationSprite_->SetTexture(operationTexture_);
	operationSprite_->SetPosition({ 10,560 });
	operationSprite_->SetAnchorPoint({ 0.0f,0.0f });
	operationSprite_->SetIsDisplay(true);

}

bool RedoUndoSystem::Redo() {
	//現在の変更履歴を下げる
	if (currentHistoryIndex_ > 0) {
		currentHistoryIndex_--;
		return true;
	}
	return false;
}

bool RedoUndoSystem::Undo() {
	//現在の変更履歴を上げる
	if (currentHistoryIndex_ < redoUndoList_.size() - 1) {
		currentHistoryIndex_++;
		return true;
	}
	return false;
}

CsvMapData RedoUndoSystem::reflectionMapState() {
	return redoUndoList_[currentHistoryIndex_];
}

void RedoUndoSystem::AddNewHistory(const CsvMapData& addMap) {
	//現在の変更履歴に合わせてサイズをそろえる
	redoUndoList_.resize(currentHistoryIndex_ + 1);
	//新しいマップ情報を入力
	redoUndoList_.push_back(addMap);
	//現在の変更履歴位置を更新
	currentHistoryIndex_++;
}
