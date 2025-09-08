#include "RedoUndoSystem.h"

void RedoUndoSystem::Initialize(const CsvMapData& initialMap) {
	//初期マップの読み込み
	redoUndoList_.push_back(initialMap);

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
