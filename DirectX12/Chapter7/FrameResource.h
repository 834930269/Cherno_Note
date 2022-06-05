#pragma once


#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"


struct ObjectConstants {
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
};

struct PassConstants {
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f,0.0f,0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f,0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f,0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
};

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

/// <summary>
/// 存有CPU为构建每帧命令列表所需要的资源
/// 其中的数据将依照程序而异,这取决于实际绘制所需要的资源
/// </summary>
class FrameResource
{
public:
	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount);
	FrameResource(const FrameResource& rhs) = delete;
	FrameResource& operator = (const FrameResource& rhs) = delete;
	~FrameResource();

	//在GPU处理完与此命令分配器相关的命令之前,我们不能对它进行重置
	//所以每一帧都要有它们自己的命令分配器
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CmdListAlloc;

	//在GPU执行完引用此常量缓冲区的命令之前,我们不能对它进行更新。
	//因此每一帧都要有它们自己的常量缓冲器
	std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

	//Fence Value to mark commands to this fence point. This lets us
	//check if these frame resources are still use by the GPU.
	UINT64 Fence = 0;
};

