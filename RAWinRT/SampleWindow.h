#pragma once
#include "dx.h"
#include <Windows.ApplicationModel.Core.h>
#include <Windows.Graphics.Display.h>

typedef ::ABI::Windows::Foundation::ITypedEventHandler < ::ABI::Windows::ApplicationModel::Core::CoreApplicationView *, ABI::Windows::ApplicationModel::Activation::IActivatedEventArgs * > IActivatedEventHandler;
typedef ::ABI::Windows::Foundation::ITypedEventHandler < ::ABI::Windows::UI::Core::CoreWindow *, ::ABI::Windows::UI::Core::VisibilityChangedEventArgs * > IVisibilityChangedEventHandler;
typedef ::ABI::Windows::Foundation::ITypedEventHandler < ::ABI::Windows::UI::Core::CoreWindow *, ::ABI::Windows::UI::Core::WindowSizeChangedEventArgs * > IWindowSizeChangedEventHandler;

struct SampleWindow :
	::ABI::Windows::ApplicationModel::Core::IFrameworkViewSource,
	::ABI::Windows::ApplicationModel::Core::IFrameworkView,
	::ABI::Windows::Graphics::Display::IDisplayPropertiesEventHandler,
	IActivatedEventHandler,
	IVisibilityChangedEventHandler,
	IWindowSizeChangedEventHandler
{
private:
	::Microsoft::WRL::ComPtr<::ABI::Windows::UI::Core::ICoreWindow> m_window;
	::Microsoft::WRL::ComPtr<::ABI::Windows::Graphics::Display::IDisplayPropertiesStatics> m_displayProperties;
	::dxdoth::Direct2D::Factory1 m_factory;
	::dxdoth::Direct2D::DeviceContext m_target;
	::dxdoth::Dxgi::SwapChain1 m_swapChain;
	bool m_visible;
public:
	SampleWindow() : m_visible(true) {}

	auto __stdcall QueryInterface(IID const & id,
		void ** result) -> HRESULT override
	{
		ASSERT(result);
		if (id == __uuidof(IFrameworkViewSource) ||
			id == __uuidof(IInspectable) ||
			id == __uuidof(IUnknown))
		{
			*result = static_cast<IFrameworkViewSource *>(this);
		}
		else if (id == __uuidof(IFrameworkView))
		{
			*result = static_cast<IFrameworkView *>(this);
		}
		else if (id == __uuidof(IDisplayPropertiesEventHandler))
		{
			*result = static_cast<IDisplayPropertiesEventHandler *>(this);
		}
		else if (id == __uuidof(IActivatedEventHandler))
		{
			*result = static_cast<IActivatedEventHandler *>(this);
		}
		else if (id == __uuidof(IVisibilityChangedEventHandler))
		{
			*result = static_cast<IVisibilityChangedEventHandler *>(this);
		}
		else if (id == __uuidof(IWindowSizeChangedEventHandler))
		{
			*result = static_cast<IWindowSizeChangedEventHandler *>(this);
		}
		else
		{
			*result = nullptr;
			return E_NOINTERFACE;
		}
		// static_cast<IUnknown *>(*result)->AddRef();
		return S_OK;
	}

	auto __stdcall AddRef()  -> ULONG { return 2; }

	auto __stdcall Release() -> ULONG { return 1; }

	auto __stdcall GetIids(ULONG *,
		IID **) -> HRESULT
	{
		return E_NOTIMPL;
	}

	auto __stdcall GetRuntimeClassName(HSTRING *) -> HRESULT
	{
		return E_NOTIMPL;
	}

	auto __stdcall GetTrustLevel(TrustLevel *) -> HRESULT
	{
		return E_NOTIMPL;
	}

	auto __stdcall CreateView(IFrameworkView ** result) -> HRESULT override
	{
		ASSERT(result);
		*result = this;
		// (*result)->AddRef();
		return S_OK;
	}

	auto __stdcall Initialize(::ABI::Windows::ApplicationModel::Core::ICoreApplicationView * view) -> HRESULT override
	{
		m_displayProperties = ::dxdoth::GetActivationFactory<::ABI::Windows::Graphics::Display::IDisplayPropertiesStatics>(
			RuntimeClass_Windows_Graphics_Display_DisplayProperties);
		EventRegistrationToken token;
		::dxdoth::HR(view->add_Activated(this, &token));
		return S_OK;
	}

	auto __stdcall SetWindow(::ABI::Windows::UI::Core::ICoreWindow * window) -> HRESULT override
	{
		m_window = window;
		return S_OK;
	}

	auto __stdcall Load(HSTRING) -> HRESULT override
	{
		EventRegistrationToken token;
		::dxdoth::HR(m_window->add_VisibilityChanged(this, &token));
		::dxdoth::HR(m_window->add_SizeChanged(this, &token));
		::dxdoth::HR(m_displayProperties->add_LogicalDpiChanged(this, &token));
		m_factory = ::dxdoth::Direct2D::CreateFactory();
		return S_OK;
	}

	void ReleaseDevice()
	{
		m_target.Reset();
		m_swapChain.Reset();
	}

	void CreateDeviceSwapChainBitmap()
	{
		::dxdoth::Direct2D::BitmapProperties1 props(::dxdoth::Direct2D::BitmapOptions::Target | ::dxdoth::Direct2D::BitmapOptions::CannotDraw,
			::dxdoth::PixelFormat(::dxdoth::Dxgi::Format::B8G8R8A8_UNORM, ::dxdoth::AlphaMode::Ignore));
		auto bitmap =
			m_target.CreateBitmapFromDxgiSurface(m_swapChain, props);
		m_target.SetTarget(bitmap);
	}

	void ResizeSwapChainBitmap()
	{
		m_target.SetTarget();
		if (S_OK == m_swapChain.ResizeBuffers())
		{
			CreateDeviceSwapChainBitmap();
		}
		else
		{
			ReleaseDevice();
		}
	}

	void Render()
	{
		if (!m_target)
		{
			// Подготавливаем мишень рендеринга...
			auto device = ::dxdoth::Direct3D::CreateDevice();
			m_target = m_factory.CreateDevice(device).CreateDeviceContext();
			auto dxgi = device.GetDxgiFactory();
			m_swapChain = dxgi.CreateSwapChainForCoreWindow(device, m_window.Get());
			CreateDeviceSwapChainBitmap();
			float dpi;
			::dxdoth::HR(m_displayProperties->get_LogicalDpi(&dpi));
			m_target.SetDpi(dpi);
		}
		m_target.BeginDraw();
		Draw();
		m_target.EndDraw();
		auto const hr = m_swapChain.Present();
		if (S_OK != hr && DXGI_STATUS_OCCLUDED != hr)
		{
			ReleaseDevice();
		}
	}

	void Draw()
	{
		wchar_t const text[] = L"съешь ещё этих мягких французских булок";

		auto clearColor = ::dxdoth::Color(1.0f, 1.0f, 1.0f);
		auto brushColor = ::dxdoth::Color(0.0f, 0.0f, 0.0f, 0.5f);

		m_target.SetAntialiasMode(::dxdoth::Direct2D::AntialiasMode::PerPrimitive);
		m_target.SetTextAntialiasMode(::dxdoth::Direct2D::TextAntialiasMode::ClearType);

		auto brush = m_target.CreateSolidColorBrush(brushColor);
		auto factory = ::dxdoth::DirectWrite::CreateFactory();
		auto fonts = factory.GetSystemFontCollection();
		auto textFormat = factory.CreateTextFormat(L"Segoe UI", ::dxdoth::DirectWrite::FontWeight::Bold, ::dxdoth::DirectWrite::FontStyle::Normal, ::dxdoth::DirectWrite::FontStretch::Normal, 40.0f);

		m_target.Clear(clearColor);
		
		m_target.SetTextAntialiasMode(::dxdoth::Direct2D::TextAntialiasMode::Aliased);
		m_target.DrawText(text, _countof(text) - 1, textFormat, ::dxdoth::RectF(10.0f, 0, 1250.0f, 40), brush);

		m_target.SetTextAntialiasMode(::dxdoth::Direct2D::TextAntialiasMode::Grayscale);
		m_target.DrawText(text, _countof(text) - 1, textFormat, ::dxdoth::RectF(10.0f, 50, 1250.0f, 90), brush);

		m_target.SetTextAntialiasMode(::dxdoth::Direct2D::TextAntialiasMode::ClearType);
		m_target.DrawText(text, _countof(text) - 1, textFormat, ::dxdoth::RectF(10.0f, 100, 1250.0f, 150), brush);

		/*
		*/

		brush.Reset();
		factory.Reset();
		fonts.Reset();
		textFormat.Reset();
	}

	auto __stdcall Run() -> HRESULT
	{
		::Microsoft::WRL::ComPtr<::ABI::Windows::UI::Core::ICoreDispatcher> dispatcher;
		::dxdoth::HR(m_window->get_Dispatcher(dispatcher.GetAddressOf()));
		Render();
		::dxdoth::HR(dispatcher->ProcessEvents(::ABI::Windows::UI::Core::CoreProcessEventsOption_ProcessUntilQuit));
		return S_OK;
	}

	auto __stdcall Invoke(::ABI::Windows::ApplicationModel::Core::ICoreApplicationView *,
		::ABI::Windows::ApplicationModel::Activation::IActivatedEventArgs *) -> HRESULT override
	{
		::dxdoth::HR(m_window->Activate());
		return S_OK;
	}

	auto __stdcall Invoke(::ABI::Windows::UI::Core::ICoreWindow *,
		::ABI::Windows::UI::Core::IVisibilityChangedEventArgs * args) -> HRESULT override
	{
		unsigned char visible;
		::dxdoth::HR(args->get_Visible(&visible));
		m_visible = 0 != visible;
		return S_OK;
	}

	auto __stdcall Invoke(IInspectable *) -> HRESULT override
	{
		if (m_target)
		{
			float dpi;
			::dxdoth::HR(m_displayProperties->get_LogicalDpi(&dpi));
			m_target.SetDpi(dpi);
			Render();
		}
		return S_OK;
	}

	auto __stdcall Invoke(::ABI::Windows::UI::Core::ICoreWindow *,
		::ABI::Windows::UI::Core::IWindowSizeChangedEventArgs *) -> HRESULT override
	{
		if (m_target)
		{
			ResizeSwapChainBitmap();
			Render();
		}
		return S_OK;
	}

	auto __stdcall Uninitialize() -> HRESULT
	{
		return S_OK;
	}
};