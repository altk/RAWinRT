#include "pch.h"
#include "RAWinRT.WRL.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;
using namespace ABI::RAWinRT::WRL;
using namespace ABI::Windows::ApplicationModel::Background;

class TestTaskImpl : public RuntimeClass < RuntimeClassFlags<WinRt>, ITestTask, IBackgroundTask >
{
	InspectableClass(RuntimeClass_RAWinRT_WRL_TestTask, BaseTrust);
public:
	TestTaskImpl()
	{
	}

	STDMETHODIMP Run(IBackgroundTaskInstance *taskInstance) override
	{
		OutputDebugStringW(L"TestTask run");
		ComPtr<IBackgroundTaskDeferral> deferral;
		taskInstance->GetDeferral(
			deferral.GetAddressOf()
			);

		unsigned int progress;
		for (progress = 0; progress <= 100; progress += 10)
		{
			taskInstance->get_Progress(&progress);
		}

		deferral->Complete();
		return S_OK;
	}
};

ActivatableClass(TestTaskImpl);