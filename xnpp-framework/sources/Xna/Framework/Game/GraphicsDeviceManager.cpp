#include "Xna/Framework/Game/GraphicsDeviceManager.hpp"
#include "Xna/Framework/Game/Game.hpp"
#include "Xna/Platform/Platform.hpp"
#include <algorithm>
#include "Xna/CSharp/Thread.hpp"
#include <cmath>

//Helpers
namespace Xna {
	static DisplayOrientation ChooseOrientation(DisplayOrientation orientation, int32_t width, int32_t height, bool allowLandscapeLeftAndRight) {
		if (((int)orientation & ((int)DisplayOrientation::LandscapeLeft | (int)DisplayOrientation::LandscapeRight | (int)DisplayOrientation::Portrait)) != 0)
			return orientation;

		if (width > height) {
			if (allowLandscapeLeftAndRight) {
				return static_cast<DisplayOrientation>((int)DisplayOrientation::LandscapeLeft | (int)DisplayOrientation::LandscapeRight);
			}

			return DisplayOrientation::LandscapeLeft;
		}

		return DisplayOrientation::Portrait;
	}
}

namespace Xna {
	GraphicsDeviceManager::GraphicsDeviceManager(Game const& game) {
		const auto currentService = game.Services().GetService<IGraphicsDeviceManager>();

		if (currentService != nullptr)
			throw CSharp::ArgumentException("GraphicsDeviceManagerAlreadyPresent");

		impl = std::make_shared<Implementation>();
		impl->game = game;
		impl->game->Services().AddService<IGraphicsDeviceManager, GraphicsDeviceManager>(*this);
		impl->game->Services().AddService<IGraphicsDeviceService, GraphicsDeviceManager>(*this);
		impl->game->Window()->ClientSizeChanged().Add([this](void* sender, CSharp::EventArgs const& e) {
			GameWindowClientSizeChanged(sender, e);
			});
		
		impl->game->Window()->ScreenDeviceNameChanged().Add([this](void* sender, CSharp::EventArgs const& e) {
			GameWindowScreenDeviceNameChanged(sender, e);
			});

		impl->game->Window()->OrientationChanged().Add([this](void* sender, CSharp::EventArgs const& e) {
			GameWindowOrientationChanged(sender, e);
			});

		impl->graphicsProfile = ReadDefaultGraphicsProfile();
	}

	void GraphicsDeviceManager::ApplyChanges() {
		if (impl->device.has_value() && !impl->isDeviceDirty)
			return;

		ChangeDevice(false);
	}

	void GraphicsDeviceManager::ToggleFullScreen() {
		const auto isFullScreen = IsFullScreen();
		IsFullScreen(!isFullScreen);

		//TODO: No directX11 não é necessário recriar o dispositivo gráfico
		ChangeDevice(false);
	}

	void GraphicsDeviceManager::GameWindowScreenDeviceNameChanged(void* sender, CSharp::EventArgs const& e) {
		if (impl->inDeviceTransition)
			return;
		ChangeDevice(false);
	}

	void GraphicsDeviceManager::GameWindowClientSizeChanged(void* sender, CSharp::EventArgs const& e) {
		if (impl->inDeviceTransition || impl->game->Window()->ClientBounds().Height == 0 && impl->game->Window()->ClientBounds().Width == 0)
			return;

		impl->resizedBackBufferWidth = impl->game->Window()->ClientBounds().Width;
		impl->resizedBackBufferHeight = impl->game->Window()->ClientBounds().Height;
		impl->useResizedBackBuffer = true;
		
		ChangeDevice(false);
	}
	void GraphicsDeviceManager::GameWindowOrientationChanged(void* sender, CSharp::EventArgs const& e) {
		if (impl->inDeviceTransition || impl->game->Window()->ClientBounds().Height == 0 && impl->game->Window()->ClientBounds().Width == 0 || impl->game->Window()->CurrentOrientation() == impl->currentWindowOrientation)
			return;
		
		ChangeDevice(false);
	}

	bool GraphicsDeviceManager::EnsureDevice() {
		return impl->device.has_value() && EnsureDevicePlatform();
	}

	void GraphicsDeviceManager::CreateDevice(GraphicsDeviceInformation& newInfo) {
		impl->device = std::nullopt;
		OnPreparingDeviceSettings(this, PreparingDeviceSettingsEventArgs(newInfo));		
		MassagePresentParameters(newInfo.PresentationParameters());

		ValidateGraphicsDeviceInformation(newInfo);
		impl->device = Xna::GraphicsDevice(newInfo.Adapter(), newInfo.GraphicsProfile(), newInfo.PresentationParameters());
		
		impl->device->DeviceResetting().Add([this](void* sender, CSharp::EventArgs const& args) {
			HandleDeviceResetting(sender, args);
			});
		impl->device->DeviceReset().Add([this](void* sender, CSharp::EventArgs const& args) {
			HandleDeviceReset(sender, args);
			});
		impl->device->DeviceLost().Add([this](void* sender, CSharp::EventArgs const& args) {
			HandleDeviceLost(sender, args);
			});
		impl->device->Disposing().Add([this](void* sender, CSharp::EventArgs const& args) {
			HandleDisposing(sender, args);
			});

		ConfigureTouchInput(newInfo.PresentationParameters());
		OnDeviceCreated(this, CSharp::EventArgs::Empty);
	}

	void GraphicsDeviceManager::ChangeDevice(bool forceCreate) {
		if (!impl->game.has_value())
			throw CSharp::InvalidOperationException("GraphicsComponentNotAttachedToGame");

		impl->inDeviceTransition = true;
		std::string screenDeviceName = impl->game->Window()->ScreenDeviceName();
		auto clientWidth = impl->game->Window()->ClientBounds().Width;
		int clientHeight = impl->game->Window()->ClientBounds().Height;
		bool flag1 = false;

		impl->game->Window()->SetSupportedOrientations(ChooseOrientation(impl->supportedOrientations, PreferredBackBufferWidth(), PreferredBackBufferHeight(), true));
		auto bestDevice = FindBestDevice(forceCreate);
		impl->game->Window()->BeginScreenDeviceChange(bestDevice.PresentationParameters().IsFullScreen);
		flag1 = true;
		bool flag2 = true;

		if (!forceCreate && impl->device.has_value()) {
			OnPreparingDeviceSettings(this, PreparingDeviceSettingsEventArgs(bestDevice));

			if (CanResetDevice(bestDevice)) {
				try {
					auto deviceInformation = bestDevice;
					MassagePresentParameters(bestDevice.PresentationParameters());
					ValidateGraphicsDeviceInformation(bestDevice);
					impl->device->Reset(deviceInformation.PresentationParameters(), deviceInformation.Adapter());
					ConfigureTouchInput(deviceInformation.PresentationParameters());
					flag2 = false;
				}
				catch (std::exception& ex) {
					//TODO: devo fazer alguma coisa?
				}
			}
		}

		if (flag2)
			CreateDevice(bestDevice);

		auto& presentationParameters = impl->device->PresentationParameters();
		screenDeviceName = impl->device->Adapter().DeviceName();
		impl->isReallyFullScreen = presentationParameters.IsFullScreen;

		if (presentationParameters.BackBufferWidth != 0)
			clientWidth = presentationParameters.BackBufferWidth;

		if (presentationParameters.BackBufferHeight != 0)
			clientHeight = presentationParameters.BackBufferHeight;

		impl->isDeviceDirty = false;

		if (flag1)
			impl->game->Window()->EndScreenDeviceChange(screenDeviceName, clientWidth, clientHeight);

		impl->currentWindowOrientation = impl->game->Window()->CurrentOrientation();
		impl->inDeviceTransition = false;
	}

	void GraphicsDeviceManager::MassagePresentParameters(PresentationParameters& pp) {
		auto flag1 = pp.BackBufferWidth == 0;
		auto flag2 = pp.BackBufferHeight == 0;

		if (pp.IsFullScreen)
			return;

		auto hWnd = pp.DeviceWindowHandle;
		if (hWnd == 0) {
			if (!impl->game.has_value())
				throw CSharp::InvalidOperationException("GraphicsComponentNotAttachedToGame");

			hWnd = impl->game->Window()->Handle();
		}

		int32_t right{ 0 };
		int32_t bottom{ 0 };

		const auto rect = Platform::System_ClientRect(hWnd);
		right = rect.Right;
		bottom = rect.Bottom;

		if (flag1 && right == 0)
			pp.BackBufferWidth = 1;
		if (!flag2 || bottom != 0)
			return;

		pp.BackBufferHeight = 1;
	}

	void GraphicsDeviceManager::ConfigureTouchInput(PresentationParameters const& pp) {
		//TODO: implementar
	}

	bool GraphicsDeviceManager::CanResetDevice(GraphicsDeviceInformation const& newDeviceInfo) {
		return impl->device->GraphicsProfile() == newDeviceInfo.GraphicsProfile();
	}

	GraphicsDeviceInformation GraphicsDeviceManager::FindBestDevice(bool anySuitableDevice) {
		return FindBestPlatformDevice(anySuitableDevice);
	}

	void GraphicsDeviceManager::RankDevices(std::vector<GraphicsDeviceInformation>& foundDevices) {
		RankDevicesPlatform(foundDevices);
	}

	void GraphicsDeviceManager::HandleDisposing(void* sender, CSharp::EventArgs  const& e) {
		OnDeviceDisposing(this, CSharp::EventArgs::Empty);
	}

	void GraphicsDeviceManager::HandleDeviceLost(void* sender, CSharp::EventArgs const& e) {}

	void GraphicsDeviceManager::HandleDeviceReset(void* sender, CSharp::EventArgs const& e) {
		OnDeviceReset(this, CSharp::EventArgs::Empty);
	}
	void GraphicsDeviceManager::HandleDeviceResetting(void* sender, CSharp::EventArgs const& e) {
		OnDeviceResetting(this, CSharp::EventArgs::Empty);
	}

	void GraphicsDeviceManager::OnDeviceCreated(void* sender, CSharp::EventArgs const& e) {
		impl->deviceCreated.Invoke(sender, e);
	}

	void GraphicsDeviceManager::OnDeviceDisposing(void* sender, CSharp::EventArgs const& e) {
		impl->deviceDisposing.Invoke(sender, e);
	}

	void GraphicsDeviceManager::OnDeviceReset(void* sender, CSharp::EventArgs  const& e) {
		impl->deviceReset.Invoke(sender, e);
	}

	void GraphicsDeviceManager::OnDeviceResetting(void* sender, CSharp::EventArgs  const& e) {
		impl->deviceResetting.Invoke(sender, e);
	}

	void GraphicsDeviceManager::OnPreparingDeviceSettings(void* sender, PreparingDeviceSettingsEventArgs const& e) {
		impl->preparingDeviceSettings.Invoke(sender, e);
	}

	bool GraphicsDeviceManager::BeginDraw() {
		if (!EnsureDevice())
			return false;

		impl->beginDrawOk = true;
		Platform::GraphicsDevice_SetRenderTargets(impl->device.value());

		return true;
	}

	void GraphicsDeviceManager::EndDraw() {
		if (!impl->beginDrawOk)
			return;

		if (!impl->device)
			return;

		impl->device->Present();
	}

	Xna::GraphicsProfile GraphicsDeviceManager::ReadDefaultGraphicsProfile() {
		return GraphicsProfile::HiDef;
	}

	void GraphicsDeviceManager::RankDevicesPlatform(std::vector<GraphicsDeviceInformation>& foundDevices) {		
		std::sort(foundDevices.begin(), foundDevices.end(), GraphicsDeviceInformationComparer(*this));
	}

	GraphicsDeviceInformation GraphicsDeviceManager::FindBestPlatformDevice(bool anySuitableDevice) {
		auto foundDevices = std::vector<GraphicsDeviceInformation>();
		AddDevices(anySuitableDevice, foundDevices);

		if (foundDevices.empty() && PreferMultiSampling())
		{
			PreferMultiSampling(false);
			AddDevices(anySuitableDevice, foundDevices);
		}

		if (foundDevices.empty())
			throw CSharp::InvalidOperationException("NoCompatibleDevices");

		RankDevices(foundDevices);

		if (foundDevices.size() == 0)
			throw CSharp::InvalidOperationException("NoCompatibleDevicesAfterRanking");

		return foundDevices[0];
	}

	void GraphicsDeviceManager::AddDevices(bool anySuitableDevice, std::vector<GraphicsDeviceInformation>& foundDevices) {
		auto handle = reinterpret_cast<HWND>(impl->game->Window()->Handle());
		auto adapters = GraphicsAdapter::Adapters();

		for (auto& adapter : adapters) {
			if (!anySuitableDevice)
			{
				if (!IsWindowOnAdapter(reinterpret_cast<intptr_t>(handle), *adapter))
					continue;
			}

			if (adapter->IsProfileSupported(impl->graphicsProfile))
			{
				auto baseDeviceInfo = GraphicsDeviceInformation();
				baseDeviceInfo.Adapter(*adapter);
				baseDeviceInfo.GraphicsProfile(impl->graphicsProfile);
				baseDeviceInfo.PresentationParameters().DeviceWindowHandle = reinterpret_cast<intptr_t>(handle);
				baseDeviceInfo.PresentationParameters().MultiSampleCount = 0;
				baseDeviceInfo.PresentationParameters().IsFullScreen = IsFullScreen();
				baseDeviceInfo.PresentationParameters().PresentationInterval = SynchronizeWithVerticalRetrace() ? PresentInterval::One : PresentInterval::Immediate;

				AddDevices(*adapter, adapter->CurrentDisplayMode(), baseDeviceInfo, foundDevices);

				if (IsFullScreen())
				{
					auto modes = adapter->SupportedDisplayModes();
					for (auto& supportedDisplayMode : modes)
					{
						if (supportedDisplayMode->Width() >= 640 && supportedDisplayMode->Height() >= 480)
							AddDevices(*adapter, *supportedDisplayMode, baseDeviceInfo, foundDevices);
					}
				}
			}
		}
	}

	void GraphicsDeviceManager::AddDevices(GraphicsAdapter const& adapter, DisplayMode const& mode, GraphicsDeviceInformation const& baseDeviceInfo, std::vector<GraphicsDeviceInformation>& foundDevices) {
		auto deviceInformation = baseDeviceInfo;

		if (IsFullScreen())
		{
			deviceInformation.PresentationParameters().BackBufferWidth = mode.Width();
			deviceInformation.PresentationParameters().BackBufferHeight = mode.Height();
		}
		else if (impl->useResizedBackBuffer)
		{
			deviceInformation.PresentationParameters().BackBufferWidth = impl->resizedBackBufferWidth;
			deviceInformation.PresentationParameters().BackBufferHeight = impl->resizedBackBufferHeight;
		}
		else
		{
			deviceInformation.PresentationParameters().BackBufferWidth = PreferredBackBufferWidth();
			deviceInformation.PresentationParameters().BackBufferHeight = PreferredBackBufferHeight();
		}

		SurfaceFormat selectedFormat{};
		DepthFormat selectedDepthFormat{};
		int32_t selectedMultiSampleCount{ 0 };			

		adapter.QueryBackBufferFormat(
			deviceInformation.GraphicsProfile(),
			mode.Format(), 
			PreferredDepthStencilFormat(), 
			PreferMultiSampling() ? 16 : 0,
			selectedFormat, selectedDepthFormat, selectedMultiSampleCount);

		deviceInformation.PresentationParameters().BackBufferFormat = selectedFormat;
		deviceInformation.PresentationParameters().DepthStencilFormat = selectedDepthFormat;
		deviceInformation.PresentationParameters().MultiSampleCount = selectedMultiSampleCount;

		if (std::find(foundDevices.begin(), foundDevices.end(), deviceInformation) != foundDevices.end())
			return;

		foundDevices.push_back(deviceInformation);
	}

	bool GraphicsDeviceManager::IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter) {
		const auto screenFromAdapter = GameWindow::ScreenFromAdapter(adapter);
		const auto screenFromHandle = GameWindow::ScreenFromHandle(windowHandle);
		
		return screenFromAdapter == screenFromHandle;
	}

	bool GraphicsDeviceManager::EnsureDevicePlatform() {		
		if (impl->isReallyFullScreen && !impl->game->IsActiveIgnoringGuide())
			return false;

		switch (impl->device->GraphicsDeviceStatus()) {
		case GraphicsDeviceStatus::Lost:
			CSharp::Thread::Sleep(impl->deviceLostSleepTime.TotalMilliseconds());
			return false;
		case GraphicsDeviceStatus::NotReset:
			CSharp::Thread::Sleep(impl->deviceLostSleepTime.TotalMilliseconds());
			
			try {
				ChangeDevice(false);
			}
			//TODO: catch (DeviceLostException ex) return false;
			catch (std::exception& ex) {
				ChangeDevice(true);
				break;
			}

		}
		return true;
	}

	void GraphicsDeviceManager::ValidateGraphicsDeviceInformation(GraphicsDeviceInformation const& devInfo) {
		auto adapter = devInfo.Adapter();
		const auto& presentationParameters = devInfo.presentationParameters;

		if (!presentationParameters.IsFullScreen)
			return;

		if (presentationParameters.BackBufferWidth == 0 || presentationParameters.BackBufferHeight == 0)
			throw CSharp::ArgumentException("ValidateBackBufferDimsFullScreen");

		bool flag = true;
		DisplayMode currentDisplayMode = adapter.CurrentDisplayMode();
		if (currentDisplayMode.Format() != presentationParameters.BackBufferFormat && currentDisplayMode.Width() != presentationParameters.BackBufferWidth && currentDisplayMode.Height() != presentationParameters.BackBufferHeight)
		{
			flag = false;
			auto displayModes = adapter.SupportedDisplayModes()[presentationParameters.BackBufferFormat];

			for (auto& displayMode : displayModes) {
				if (displayMode->Width() == presentationParameters.BackBufferWidth && displayMode->Height() == presentationParameters.BackBufferHeight)
				{
					flag = true;
					break;
				}
			}
		}

		if (!flag)
			throw CSharp::ArgumentException("ValidateBackBufferDimsModeFullScreen");
	}

	bool GraphicsDeviceInformationComparer::operator()(GraphicsDeviceInformation const& d1, GraphicsDeviceInformation const& d2) const {
		if (d1.GraphicsProfile() != d2.GraphicsProfile())
			return d1.GraphicsProfile() < d2.GraphicsProfile();

		const auto presentationParameters1 = d1.presentationParameters;
		const auto presentationParameters2 = d2.presentationParameters;

		if (presentationParameters1.IsFullScreen != presentationParameters2.IsFullScreen)
			return graphics->IsFullScreen() == presentationParameters1.IsFullScreen;

		const auto num1 = RankFormat(presentationParameters1.BackBufferFormat);
		const auto num2 = RankFormat(presentationParameters2.BackBufferFormat);

		if (num1 != num2)
			return num1 < num2;

		if (presentationParameters1.MultiSampleCount != presentationParameters2.MultiSampleCount)
			return presentationParameters1.MultiSampleCount < presentationParameters2.MultiSampleCount;

		const auto num3 = graphics->PreferredBackBufferWidth() == 0
			|| graphics->PreferredBackBufferHeight() == 0
			? static_cast<float>(GraphicsDeviceManager::DefaultBackBufferWidth) / static_cast<float>(GraphicsDeviceManager::DefaultBackBufferHeight)
			: static_cast<float>(graphics->PreferredBackBufferWidth()) / static_cast<float>(graphics->PreferredBackBufferHeight());

		const auto num4 = (float)presentationParameters1.BackBufferWidth / (float)presentationParameters1.BackBufferHeight;
		const auto num5 = (float)presentationParameters2.BackBufferWidth / (float)presentationParameters2.BackBufferHeight;
		const auto num6 = std::abs(num4 - num3);
		const auto num7 = std::abs(num5 - num3);

		if ((double)std::abs(num6 - num7) > 0.20000000298023224)
			return num6 < num7 ? 1 : -1;

		int num8{ 0 };
		int num9{ 0 };

		if (graphics->IsFullScreen())
		{
			if (graphics->PreferredBackBufferWidth() == 0 || graphics->PreferredBackBufferHeight() == 0)
			{
				const auto adapter1 = d1.Adapter();
				num8 = adapter1.CurrentDisplayMode().Width() * adapter1.CurrentDisplayMode().Height();
				const auto adapter2 = d2.Adapter();
				num9 = adapter2.CurrentDisplayMode().Width() * adapter2.CurrentDisplayMode().Height();
			}
			else
				num8 = num9 = graphics->PreferredBackBufferWidth() * graphics->PreferredBackBufferHeight();
		}
		else
			num8 = graphics->PreferredBackBufferWidth() == 0
			|| graphics->PreferredBackBufferHeight() == 0
			? (num9 = GraphicsDeviceManager::DefaultBackBufferWidth * GraphicsDeviceManager::DefaultBackBufferHeight)
			: (num9 = graphics->PreferredBackBufferWidth() * graphics->PreferredBackBufferHeight());

		const auto num10 = std::abs(presentationParameters1.BackBufferWidth * presentationParameters1.BackBufferHeight - num8);
		const auto num11 = std::abs(presentationParameters2.BackBufferWidth * presentationParameters2.BackBufferHeight - num9);
		if (num10 != num11)
			return num10 < num11;

		if (d1.Adapter() != d2.Adapter())
		{
			if (d1.Adapter().IsDefaultAdapter())
				return true;
			if (d2.Adapter().IsDefaultAdapter())
				return false;
		}

		return false;
	}
}