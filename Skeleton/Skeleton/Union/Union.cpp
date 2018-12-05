#include "Union.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Fence/Fence.h"
#include "../Root/RootMane.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"
#include "../Pipe/Pipe.h"
#include "../Texture/Texture.h"
#include "../Primitive/Point.h"
#include "../Primitive/Line.h"
#include "../Primitive/Triangle.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// コンストラクタ
Union::Union(std::weak_ptr<Window>win) : 
	root(RootMane::Get()), pipe(PipeMane::Get()), win(win)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	rootNo.clear();
	pipeNo.clear();
	point.clear();
	line.clear();
	triangle.clear();

	Create();
}

// デストラクタ
Union::~Union()
{
}

// ルートシグネチャの生成
void Union::CreateRoot(const std::string & name, const std::tstring & fileName)
{
	if (rootNo.find(name) != rootNo.end())
	{
		return;
	}

	rootNo[name] = 0;
	root.CreateRoot(rootNo[name], dev, fileName);
}

// ルートシグネチャの生成
void Union::CreateRoot(void)
{
	CreateRoot("texture", L"Shader/Texture.hlsl");
	CreateRoot("primitive", L"Shader/Primitive.hlsl");
}

// パイプラインの生成
void Union::CreatePipe(const std::string & name, const std::string & rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type,
	const std::initializer_list<int> & index, const bool & depth)
{
	if (pipeNo.find(name) != pipeNo.end())
	{
		return;
	}

	pipeNo[name] = 0;
	pipe.CreatePipe(pipeNo[name], dev, swap, root.Get(rootNo[rootName]), type, index, depth);
}

// パイプラインの生成
void Union::CreatePipe(void)
{
	CreatePipe("texture",  "texture",   D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 }, false);
	CreatePipe("point",    "primitive", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,    { 0, 3 }, false);
	CreatePipe("line",     "primitive", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,     { 0, 3 }, false);
	CreatePipe("triangle", "primitive", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 3 }, false);
}

// クラスの生成
void Union::Create(void)
{
	dev   = std::make_shared<Device>();
	queue = std::make_shared<Queue>(dev);
	list  = std::make_shared<List>(dev);
	swap  = std::make_shared<Swap>(win, queue);
	ren   = std::make_unique<Render>(dev, swap);
	depth = std::make_unique<Depth>(dev, win.lock()->GetX(), win.lock()->GetY());
	fence = std::make_unique<Fence>(dev, queue);

	CreateRoot();
	CreatePipe();

	tex = std::make_unique<Texture>(win, dev, root.Get(rootNo["texture"]), pipe.Get(pipeNo["texture"]));
}

// 画像の読み込み
void Union::LoadImg(const std::string & fileName, int & i)
{
	tex->Load(fileName, i);
}

// 配列データから画像を生成
void Union::CreateImg(const std::vector<unsigned char>& data, const unsigned int & width, const unsigned int & height, int & i)
{
	tex->CreateImg(data, width, height, i);
}

// 画像の描画
void Union::DrawImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, const float & rectX, const float & rectY, 
	const float & rectSizexX, const float & rectSizeY, const float & alpha, const bool & turnX, const bool & turnY)
{
	tex->Draw(list, i, { x, y }, { sizeX, sizeY }, { rectX, rectY }, { rectSizexX, rectSizeY }, alpha, turnX, turnY);
}

// 点の描画
void Union::DrawPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	point.push_back(std::make_shared<Point>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["point"])));
	point.back()->SetVertex({ x, y }, { r, g, b }, alpha);
	point.back()->Draw(list);
}

// 線の描画
void Union::DrawLine(const float & x1, const float & y1, const float & x2, const float & y2, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	line.push_back(std::make_shared<Line>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["line"])));
	line.back()->SetVertex({ x1, y1 }, { x2, y2 }, { r, g, b }, alpha);
	line.back()->Draw(list);
}

// 三角形の描画
void Union::DrawTriangle(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & r, const float & g, const float & b, const float & alpha)
{
	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x1, y1 }, { x2, y2 }, { x3, y3 }, { r, g, b }, alpha);
	triangle.back()->Draw(list);
}

// 四角形の描画
void Union::DrawBox(const float & x, const float & y, const float & sizeX, const float & sizeY, const float & r, const float & g, const float & b, const float & alpha)
{
	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x, y }, { x + sizeX, y }, { x, y + sizeY }, { r, g, b }, alpha);
	triangle.back()->Draw(list);

	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x + sizeX, y }, { x + sizeX, y + sizeY }, { x, y + sizeY }, { r, g, b }, alpha);
	triangle.back()->Draw(list);
}

// 画面クリア
void Union::Clear(void)
{
	list->Reset();

	list->SetView(win.lock()->GetX(), win.lock()->GetY());
	list->SetScissor(win.lock()->GetX(), win.lock()->GetY());

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, ren->Get());

	depth->Clear(list);
	ren->Clear(list, depth->GetHeap());
}

// 実行
void Union::Execution(void)
{
	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, ren->Get());

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Get()->ExecuteCommandLists(_countof(com), com);

	swap->Get()->Present(1, 0);

	fence->Wait();

	point.clear();
	line.clear();
	triangle.clear();
}
