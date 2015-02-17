#include "pch.h"
#include "SampleWindow.h"
#include "RAWinRT.WRL.h"
#include <Windows.ApplicationModel.Core.h>
#include <Windows.ApplicationModel.background.h>
#include <windows.foundation.collections.h>
#pragma comment(lib, "RuntimeObject.lib")

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[] = TEXT("Message from first process.");

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	using namespace Microsoft::WRL::Wrappers;
	using namespace Microsoft::WRL;
	using namespace Windows::Foundation;
	using namespace ABI::RAWinRT::WRL;
	using namespace ABI::Windows::ApplicationModel::Core;
	using namespace ABI::Windows::ApplicationModel::Background;
	using namespace ABI::Windows::Foundation::Collections;

	RoInitialize(RO_INIT_MULTITHREADED);

	ComPtr<ICoreApplication> coreApplication;
	GetActivationFactory(
		HStringReference(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication).Get(),
		&coreApplication
		);

	ComPtr<IBackgroundTaskRegistrationStatics> backgroundTaskRegistrationStatics;
	GetActivationFactory(
		HStringReference(RuntimeClass_Windows_ApplicationModel_Background_BackgroundTaskRegistration).Get(),
		&backgroundTaskRegistrationStatics
		);

	ComPtr<IMapView<GUID, IBackgroundTaskRegistration*>> registrations;
	backgroundTaskRegistrationStatics->get_AllTasks(
		registrations.GetAddressOf()
		);

	ComPtr<__FIIterable_1___FIKeyValuePair_2_GUID_Windows__CApplicationModel__CBackground__CIBackgroundTaskRegistration_t> iterable;
	registrations.As(&iterable);

	ComPtr<__FIIterator_1___FIKeyValuePair_2_GUID_Windows__CApplicationModel__CBackground__CIBackgroundTaskRegistration_t> iterator;
	iterable->First(&iterator);

	boolean hasCurrent;
	iterator->get_HasCurrent(&hasCurrent);
	ComPtr<__FIKeyValuePair_2_GUID_Windows__CApplicationModel__CBackground__CIBackgroundTaskRegistration_t> current;
	ComPtr<IBackgroundTaskRegistration> value;
	while (hasCurrent)
	{
		iterator->get_Current(current.GetAddressOf());
		current->get_Value(&value);
		value->Unregister(true);
		iterator->MoveNext(&hasCurrent);
	}

	ComPtr<ISystemTriggerFactory> systemTriggerFactory;
	GetActivationFactory(
		HStringReference(RuntimeClass_Windows_ApplicationModel_Background_SystemTrigger).Get(),
		&systemTriggerFactory
		);

	ComPtr<ISystemTrigger> systemTrigger;
	systemTriggerFactory->Create(
		SystemTriggerType_TimeZoneChange,
		true,
		systemTrigger.GetAddressOf()
		);

	ComPtr<IBackgroundTaskBuilder> backgroundTaskBuilder;
	ActivateInstance(
		HStringReference(RuntimeClass_Windows_ApplicationModel_Background_BackgroundTaskBuilder).Get(),
		&backgroundTaskBuilder
		);

	ComPtr<IBackgroundTrigger> backgroundTrigger;
	systemTrigger.As<IBackgroundTrigger>(
		&backgroundTrigger
		);

	backgroundTaskBuilder->put_Name(HStringReference(L"WRL.TestTask").Get());
	backgroundTaskBuilder->put_TaskEntryPoint(HStringReference(L"RAWinRT.WRL.TestTask").Get());
	backgroundTaskBuilder->SetTrigger(backgroundTrigger.Get());

	ComPtr<IBackgroundTaskRegistration> taskRegistration;
	backgroundTaskBuilder->Register(taskRegistration.GetAddressOf());

	ComPtr<ITestTask> testTask;
	auto hr = ActivateInstance(
		HStringReference(RuntimeClass_RAWinRT_WRL_TestTask).Get(),
		&testTask
		);

	SampleWindow window;
	coreApplication->Run(&window);
}