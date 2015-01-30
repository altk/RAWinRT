#include <string>
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include "TestComponentWRL.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;
using namespace ABI::TestComponentWRL;

class TestClassImpl sealed : public RuntimeClass < ITestClass >
{
	InspectableClass(RuntimeClass_TestComponentWRL_TestClass, BaseTrust);
private:
	INT32 _int;
	HString _str;
public:
	explicit TestClassImpl(INT32 value, HString str)
		: _int(value)
		, _str(std::move(str))
	{
	}

	STDMETHODIMP get_Int(INT32* result) override
	{
		*result = _int;
		return S_OK;
	}

	STDMETHODIMP get_Str(HSTRING* result) override
	{
		*result = _str.Get();
		return S_OK;
	}
};

class TestClasFactoryImpl : public ActivationFactory < ITestClassFactory >
{
	InspectableClassStatic(RuntimeClass_TestComponentWRL_TestClass, BaseTrust);
private:
	wchar_t buffer[15];
public:
	STDMETHODIMP ActivateInstance(IInspectable** result) override
	{
		HString zeroStr;
		zeroStr.Set(L"0");
		*result = Make<TestClassImpl>(0, std::move(zeroStr)).Detach();
		return S_OK;
	}

	STDMETHODIMP CreateInstance(INT32 value, ITestClass** result) override
	{
		_itow_s(value, buffer, 10);
		HString str;
		str.Set(buffer, wcsnlen_s(buffer, _countof(buffer)));
		*result = Make<TestClassImpl>(value, std::move(str)).Detach();
		return S_OK;
	}
};

ActivatableClassWithFactory(TestClassImpl, TestClasFactoryImpl);