#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/event.h>
#include "DataBinding.h"
#include "Microsoft.Xaml.Interactivity.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Microsoft::Xaml::Interactivity;
using namespace ABI::Windows::UI::Xaml::Data;
using namespace ABI::DataBinding;

class NumberImpl : public RuntimeClass < RuntimeClassFlags<WinRt>, INumber, INumberOverrides, INotifyPropertyChanged >
{
	InspectableClass(RuntimeClass_DataBinding_Number, BaseTrust);
private:
	INT32 _value;
	ComPtr<INumber> _baseNumber;
	EventSource<IPropertyChangedEventHandler> _notifyEventSource;
	ComPtr<IPropertyChangedEventArgs> _valueChangedEventArgs;
	void InitValueChangedEventArgs()
	{
		ComPtr<IPropertyChangedEventArgsFactory> propertyChangedEventArgsFactory;
		RoGetActivationFactory(
			HStringReference(RuntimeClass_Windows_UI_Xaml_Data_PropertyChangedEventArgs).Get(),
			ABI::Windows::UI::Xaml::Data::IID_IPropertyChangedEventArgsFactory,
			reinterpret_cast<void**>(propertyChangedEventArgsFactory.GetAddressOf()));

		propertyChangedEventArgsFactory->CreateInstance(
			HStringReference(L"Value").Get(),
			nullptr,
			nullptr,
			_valueChangedEventArgs.GetAddressOf());
	}
public:
	NumberImpl()
		: _value(0)
	{
		InitValueChangedEventArgs();
	}

	explicit NumberImpl(INT32 value)
		: _value(value)
	{
		InitValueChangedEventArgs();
	}

	virtual HRESULT STDMETHODCALLTYPE get_Value(INT32* value) override
	{
		*value = _value;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE put_Value(INT32 value) override
	{
		_value = value;
		_notifyEventSource.InvokeAll(reinterpret_cast<IInspectable*>(this), _valueChangedEventArgs.Get());
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE add_PropertyChanged(IPropertyChangedEventHandler* handler, EventRegistrationToken* token) override
	{
		return _notifyEventSource.Add(handler, token);
	}

	virtual HRESULT STDMETHODCALLTYPE remove_PropertyChanged(EventRegistrationToken token) override
	{
		return _notifyEventSource.Remove(token);
	}

	virtual HRESULT STDMETHODCALLTYPE GetValue(INT32* value) override
	{
		*value = _value;
		return S_OK;
	}
};

class NumberFactory : public ActivationFactory < INumberFactory >
{
	InspectableClassStatic(RuntimeClass_DataBinding_Number, BaseTrust);

public:
	virtual HRESULT STDMETHODCALLTYPE CreateInstance0(
		IInspectable* outer,
		IInspectable** inner,
		INumber** result
		) override
	{
		INumber*  number = Make<NumberImpl>().Detach();
		if (nullptr == outer || FAILED(outer->QueryInterface(ABI::DataBinding::IID_INumber, reinterpret_cast<void**>(result))))
		{
			*result = number;
		}
		else
		{
			*inner = number;
		}
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateInstance1(
		INT32 value,
		IInspectable* outer,
		IInspectable** inner,
		INumber** result
		) override
	{
		INumber* number = Make<NumberImpl>(value).Detach();
		if (nullptr == outer || FAILED(outer->QueryInterface(ABI::DataBinding::IID_INumber, reinterpret_cast<void**>(result))))
		{
			*result = number;
		}
		else
		{
			*inner = number;
		}
		return S_OK;
	}
};

class DependencyPropertyChangedHelperFactory : public ActivationFactory < IDependencyPropertyChangedHelperStatics >
{
	InspectableClassStatic(RuntimeClass_DataBinding_DependencyPropertyChangedHelper, BaseTrust);

public:

	STDMETHODIMP SubscribeToEvent(
		ABI::Windows::UI::Xaml::IDependencyObject* sourceObject
		)
		override
	{
		HRESULT hr;
		ComPtr<__IInteractionStatics> interaction;
		{
			hr = GetActivationFactory(HStringReference(RuntimeClass_Microsoft_Xaml_Interactivity_Interaction).Get(), interaction.GetAddressOf());
			if (FAILED(hr))
			{
				return hr;
			}
		}

		ComPtr<__IBehaviorCollectionPublicNonVirtuals> behaviorsCollection;
		{
			hr = interaction->GetBehaviors(sourceObject, behaviorsCollection.GetAddressOf());
			if (FAILED(hr))
			{
				return hr;
			}
		}
		return S_OK;
	}
};

ActivatableClassWithFactory(NumberImpl, NumberFactory);

ActivatableStaticOnlyFactory(DependencyPropertyChangedHelperFactory);