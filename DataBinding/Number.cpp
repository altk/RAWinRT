#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/event.h>
#include "DataBinding_h.h"

using ABI::DataBinding::INumber;
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

class Number : public RuntimeClass < RuntimeClassFlags<RuntimeClassType::WinRt>, INumber >
{
	InspectableClass(RuntimeClass_DataBinding_Number, BaseTrust);
private:
	INT32 _value;
	ComPtr<INumber> _baseNumber;
public:
	Number(INT32 value)
		: _value(value)
	{	}

	virtual HRESULT STDMETHODCALLTYPE get_Value(INT32* value) override
	{
		*value = _value;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE put_Value(INT32 value) override
	{
		_value = value;
		return S_OK;
	}
};

class NumberFactory : public Microsoft::WRL::ActivationFactory < INumberFactory >
{
	InspectableClassStatic(RuntimeClass_DataBinding_Number, BaseTrust);

public:
	virtual HRESULT STDMETHODCALLTYPE CreateInstance1(INT32 value, IInspectable* outer, IInspectable** inner, INumber** result) override
	{
		*inner = Make<Number>(value).Detach();
		*result = reinterpret_cast<INumber*>(outer);
		return S_OK;
	}
};

ActivatableClassWithFactory(Number, NumberFactory);