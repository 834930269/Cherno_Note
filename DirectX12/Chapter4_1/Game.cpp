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
	//Ϊ���԰汾��������ʱ�ڴ��⣬����ල�ڴ�й©�����
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try {
		//�����Զ���D3DApp
		InitDirect3DApp thisApp(hInstance);
		if (!thisApp.Initialize())
			return 0;
		return thisApp.Run();//������Ⱦѭ��
	}
	catch (DxException e) {
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

/// <summary>
/// ��ʼ��
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
	//�ظ�ʹ�ü�¼������ڴ�
	//ֻ�е�GPU�����������б�ִ�����ʱ�����ǲ��ܽ�������
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	//��ͨ��ExecuteCommandList������ĳ�������б����������к�
	//���Ǳ�������ø������б��Դ������������б����ڴ�
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	//����Դ��״̬����ת��,����Դ�ӳ���״̬ת��Ϊ��ȾĿ��״̬
	mCommandList->ResourceBarrier(
		1,&CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		));

	//�����ӿںͲü�����������Ҫ���������б�����ö�����
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	//�����̨����������Ȼ�����
	mCommandList->ClearRenderTargetView(
		CurrentBackBufferView(),
		Colors::LightSteelBlue, 0, nullptr
	);
	mCommandList->ClearDepthStencilView(
		DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH |
		D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr
	);

	//ָ��Ҫ��Ⱦ�Ļ�����
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	//�ٴζ���Դ״̬����ת��������Դ����ȾĿ��״̬ת���س���״̬
	mCommandList->ResourceBarrier(
		1, &CD3DX12_RESOURCE_BARRIER::Transition(
			CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		));

	//�������ļ�¼
	ThrowIfFailed(mCommandList->Close());

	//����ִ�е������б�����������
	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	//�����б����,�����б�
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	//������̨��������ǰ̨������
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//�ȴ���֡������ִ����ϡ���ǰ��ʵ��û��ʲôЧ�ʣ�Ҳ���ڼ�
	//�����������֯,����ÿ֡�ĵȴ�
	FlushCommandQueue();//�ȴ�
}