#include<Windows.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#define DIRECTiNPUT_VERSION 

#include<DirectXTex.h>
#include<wrl.h>
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"Object3d.h"
#include"Model.h"
#include"SoundManager.h"
#include"Texture.h"


using namespace DirectX;
using namespace Microsoft::WRL;
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//テクスチャの最大枚数
const int SpriteSRVCount = 512;
#pragma region//構造体

#pragma endregion
#pragma region//関数


#pragma endregion
#pragma region//WinMain
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region//ポインタ置き場
	//ポインタ置き場
	Input* input = nullptr;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Object3d* object3d_1 = nullptr;
	Object3d* object3d_2 = nullptr;
	Object3d* object3d_3 = nullptr;

	winApp = new WinApp();
	winApp->Initialize();
	//DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//サウンド
	SoundManager* soundManager = nullptr;

	MSG msg{};
	//でバッグレイヤーのやつ
#ifdef _DEBUG
#endif
	//directx初期化
	HRESULT result;
#pragma endregion
#pragma region//定数バッファ
#pragma endregion
#pragma region//ビュー行列
	//ビュー変換行列
	XMMATRIX matview;
	XMFLOAT3 eye(0, 0, -100);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matview = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	//モデルデータ読み込み
	Model* model_1 = Model::LordFromOBJ("ground");
	Model* model_2 = Model::LordFromOBJ("triangle_mat");
	Model* model_3= Model::LordFromOBJ("Player");
#pragma endregion

	//3Dオブジェクト生成
	object3d_1 = Object3d::Create();
	object3d_2 = Object3d::Create();
	object3d_3 = Object3d::Create();


	object3d_1->SetModel(model_1);
	object3d_2->SetModel(model_2);
	object3d_3->SetModel(model_2);

	object3d_1->SetPosition({ -5,0,-5 });
	object3d_2->SetPosition({ -5,0,-5 });
	object3d_3->SetPosition({ +5,0,+5 });

	object3d_2->SetScale({ 20,20,20});
	object3d_3->SetScale({ 20,20,20 });

	//object3d->SetScale({ 20,20,20 });




#pragma region//初期化
	//入力の初期化
	input = new Input();
	input->Initialize(winApp);
	//サウンド初期化
	soundManager = new SoundManager();
	soundManager->Initialize();

	//サウンド読み込み
	soundManager->LoadWave(0, "Resources/Alarm01.wav");
#pragma region//ループ処理
	while (true) {
#pragma region//更新処理
		//ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}

		if (input->TriggerKey(DIK_0))
		{
			soundManager->PlayWave(0);
		}

		if (input->PushKey(DIK_1)) {
			object3d_2->SetModel(model_3);
			object3d_3->SetModel(model_3);
			object3d_2->SetScale({ 2,2,2 });
			object3d_3->SetScale({ 2,2,2 });
		}
		else {
			object3d_2->SetModel(model_2);
			object3d_3->SetModel(model_2);
			object3d_2->SetScale({ 20,20,20 });
			object3d_3->SetScale({ 20,20,20 });
		}



		//キーの更新
		input->Update();
		object3d_1->Update();
		object3d_2->Update();
		object3d_3->Update();

#pragma region//タイトル

#pragma endregion
		//毎フレーム処理
		dxCommon->PreDraw();
		Object3d::PreDraw(dxCommon->GetCmdList());
		object3d_1->Draw();


		object3d_2->Draw();
		object3d_3->Draw();
		Object3d::PostDraw();
		dxCommon->PostDraw();
#pragma endregion
	}
#pragma endregion
#pragma region//開放
	winApp->Finalize();

	//入力開放
	delete soundManager;
	delete input;
	delete winApp;
	delete dxCommon;
	delete object3d_1;
	delete object3d_2;
	delete object3d_3;
	delete model_1;
	delete model_2;
	delete model_3;

	winApp = nullptr;
	return 0;
#pragma endregion
}
#pragma endregion
