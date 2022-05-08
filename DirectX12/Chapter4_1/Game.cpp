//***************************************************************************************
// BoxApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
//
// Shows how to draw a box in Direct3D 12.
//
// Controls:
//   Hold the left mouse button down and move the mouse to rotate.
//   Hold the right mouse button down and move the mouse to zoom in and out.
//***************************************************************************************

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"
#include<DirectXColors.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class InitDirect3DApp : public D3DApp {
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	virtual bool Initialize()override;
private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;
};

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd) {
	//为调试版本开启运行时内存检测，方便监督内存泄漏的情况
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try {
		//创建自定义D3DApp
		InitDirect3DApp thisApp(hInstance);
		if (!thisApp.Initialize())
			return 0;
		return thisApp.Run();//进入渲染循环
	}
	catch (DxException e) {
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

/// <summary>
/// 初始化
/// </summary>
/// <param name="hInstance"></param>
InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance):D3DApp(hInstance) {

}

InitDirect3DApp::~InitDirect3DApp() {

}

bool InitDirect3DApp::Initialize() {
	if (!D3DApp::Initialize())
		return false;
	return true;
}

void InitDirect3DApp::OnResize() {
	D3DApp::OnResize();
}

void InitDirect3DApp::Update(const GameTimer& gt) {

}

void InitDirect3DApp::Draw(const GameTimer& gt) {
	//重复使用记录命令的内存
	//只有当GPU关联的命令列表执行完成时，我们才能将其重置
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	//在通过ExecuteCommandList方法将某个命令列表加入命令队列后
	//我们便可以重置该命令列表。以此来复用命令列表及其内存
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	//对资源的状态进行转换,将资源从呈现状态转换为渲染目标状态
	mCommandList->ResourceBarrier(
		1,&CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		));

	//设置视口和裁剪矩阵。它们需要随着命令列表的重置而重置
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	//清除后台缓冲区和深度缓冲区
	mCommandList->ClearRenderTargetView(
		CurrentBackBufferView(),
		Colors::LightSteelBlue, 0, nullptr
	);
	mCommandList->ClearDepthStencilView(
		DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH |
		D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr
	);

	//指定要渲染的缓冲区
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	//再次对资源状态进行转换，将资源从渲染目标状态转换回呈现状态
	mCommandList->ResourceBarrier(
		1, &CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		));

	//完成命令的记录
	ThrowIfFailed(mCommandList->Close());

	//将待执行的命令列表加入命令队列
	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	//命令列表个数,命令列表
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	//交换后台缓冲区和前台缓冲区
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//等待此帧的命令执行完毕。当前的实现没有什么效率，也过于简单
	//后面会重新组织,避免每帧的等待
	FlushCommandQueue();//等待
}