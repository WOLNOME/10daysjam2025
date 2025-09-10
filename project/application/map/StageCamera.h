// StageCamera.h
#pragma once
#include "Vector3.h"

enum class CameraType {
    A = 0, // 8x6
    B = 1, // 6x5
    C = 2, // 6x3
    D = 3, // 5x5
    E = 4, // 5x4
    kMaxCameraType
};

struct CameraTransform {
    Vector3 position;
    Vector3 rotate;
};
struct StageCamera {
    CameraTransform overLooking;
    CameraTransform above;
};

// 追加：数値コード→列挙（範囲外は0= A に丸め）
inline CameraType CameraTypeFromCode(int code) {
    if (code < 0) code = 0;
    if (code > 4) code = 4;
    return static_cast<CameraType>(code);
}
// 追加：列挙→数値コード
inline int CameraCode(CameraType t) {
    return static_cast<int>(t);
}

// 既存：列挙で取得
inline StageCamera GetStageCamera(CameraType type) {
    switch (type) {
    case CameraType::A: return { {{7.0f,25.0f,-12.0f},{0.95f,0.0f,0.0f}}, {{7.0f,30.0f,4.7f},{1.561f,0.0f,0.0f}} };
    case CameraType::B: return { {{4.8f,22.3f,-10.0f},{1.0f,0.0f,0.0f}}, {{4.8f,25.0f,3.75f},{1.561f,0.0f,0.0f}} };
    case CameraType::C: return { {{4.7f,17.0f,-8.3f},{1.0f,0.0f,0.0f}}, {{4.7f,21.0f,1.7f},{1.561f,0.0f,0.0f}} };
    case CameraType::D: return { {{3.6f,20.5f,-8.8f},{1.0f,0.0f,0.0f}}, {{3.6f,25.0f,3.78f},{1.561f,0.0f,0.0f}} };
    case CameraType::E: return { {{4.0f,20.0f,-9.5f},{1.0f,0.0f,0.0f}}, {{4.0f,22.0f,2.9f},{1.561f,0.0f,0.0f}} };
    default:            return { {{0,0,0},{0,0,0}}, {{0,0,0},{0,0,0}} };
    }
}

// 追加：数値コードで直接取れるオーバーロード
inline StageCamera GetStageCamera(int cameraCode) {
    return GetStageCamera(CameraTypeFromCode(cameraCode));
}
