#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/event.h>
#include "DataBinding_h.h"

using ABI::DataBinding::INumber;
using ABI::DataBinding::INumberOverrides;
using ABI::DataBinding::INumberFactory;
using ABI::Windows::UI::Xaml::Data::IPropertyChangedEventArgsFactory;
using ABI::Windows::UI::Xaml::Data::INotifyPropertyChanged;
using ABI::Windows::UI::Xaml::Data::IPropertyChangedEventHandler;
using ABI::Windows::UI::Xaml::Data::IPropertyChangedEventArgs;
using ABI::Windows::UI::Xaml::Data::PropertyChangedEventArgs;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::RuntimeClassType;
using Microsoft::WRL::EventSource;
using Microsoft::WRL::Make;
using Microsoft::WRL::MakeAndInitialize;
using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::ActivationFactory;
using Microsoft::WRL::SimpleActivationFactory;
using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Wrappers::HStringReference;

class Number : public RuntimeClass < RuntimeClassFlags<RuntimeClassType::WinRt>, INumber, INumberOverrides, INotifyPropertyChanged >
{
	InspectableClass(RuntimeClass_DataBinding_Number, BaseTrust);
private:
	INT32 _value;
	ComPtr<INumber> _baseNumber;
	EventSource<IPropertyChangedEventHandler> _notifyEventSource;
	ComPtr<IPropertyChangedEventArgs> _valueChangedEventArgs;
public:
	void InitValueChangedEventArgs()
	{
		{
			ComPtr<IPropertyChangedEventArgsFactory> propertyChangedEventArgsFactory;
			RoGetActivationFactory(
				HStringReference(RuntimeClass_Windows_UI_Xaml_Data_PropertyChangedEventArgs).Get(),
				ABI::Windows::UI::Xaml::Data::IID_IPropertyChangedEventArgsFactory,
				reinterpret_cast<void**>(propertyChangedEventArgsFactory.GetAddressOf()));

			ComPtr<IInspectable> inner;
			propertyChangedEventArgsFactory->CreateInstance(
				HStringReference(L"Value").Get(),
				nullptr,
				reinterpret_cast<IInspectable**>(_valueChangedEventArgs.GetAddressOf()),
				_valueChangedEventArgs.GetAddressOf());
		}
	}
	Number()
		: _value(0)
	{
		InitValueChangedEventArgs();
	}

	Number(INT32 value)
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

	virtual HRESULT STDMETHODCALLTYPE GetValue(INT32* value)
	{
		*value = _value;
		return S_OK;
	}
};

class NumberFactory : public Microsoft::WRL::ActivationFactory < INumberFactory >
{
	InspectableClassStatic(RuntimeClass_DataBinding_Number, BaseTrust);

public:
	virtual HRESULT STDMETHODCALLTYPE CreateInstance0(IInspectable* outer, IInspectable** inner, INumber** result) override
	{
		*inner = reinterpret_cast<IInspectable*>(Make<Number>().Detach());
		*result = reinterpret_cast<INumber*>(outer);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateInstance1(INT32 value, IInspectable* outer, IInspectable** inner, INumber** result) override
	{
		*inner = reinterpret_cast<IInspectable*>(Make<Number>(value).Detach());
		*result = reinterpret_cast<INumber*>(outer);
		return S_OK;
	}
};

ActivatableClassWithFactory(Number, NumberFactory);