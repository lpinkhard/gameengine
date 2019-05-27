#include "pch.h"
#include "GameEngineMain.h"
#include "Common\DirectXHelper.h"

using namespace GameEngine;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
GameEngineMain::GameEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	m_game = std::unique_ptr<Game>(new Game());
	m_game->getHAL()->setDX(m_deviceResources);
	m_game->init();
}

GameEngineMain::~GameEngineMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void GameEngineMain::CreateWindowSizeDependentResources() 
{
	Size outputSize = m_deviceResources->GetOutputSize();
	m_game->getHAL()->reshape(outputSize.Width, outputSize.Height, 1);
}

// Updates the application state once per frame.
void GameEngineMain::Update() 
{
	// Update scene objects.
	m_game->step();
}

void GameEngineMain::Suspend()
{
	m_game->suspend();
}

void GameEngineMain::Resume()
{
	m_game->resume();
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool GameEngineMain::Render() 
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_game->redraw();

	return true;
}

// Notifies renderers that device resources need to be released.
void GameEngineMain::OnDeviceLost()
{
	m_game->getHAL()->resetDDR();
}

// Notifies renderers that device resources may now be recreated.
void GameEngineMain::OnDeviceRestored()
{
	HAL *hal = m_game->getHAL();
	hal->resetDDR();
	CreateWindowSizeDependentResources();
}
