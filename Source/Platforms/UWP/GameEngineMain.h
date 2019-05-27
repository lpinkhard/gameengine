#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"

#include "../../Game/Game.hpp"

// Renders Direct2D and 3D content on the screen.
namespace GameEngine
{
	class GameEngineMain : public DX::IDeviceNotify
	{
	public:
		GameEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~GameEngineMain();
		void CreateWindowSizeDependentResources();
		void Update();
		void Suspend();
		void Resume();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Access to actual game code
		std::unique_ptr<Game> m_game;
	};
}