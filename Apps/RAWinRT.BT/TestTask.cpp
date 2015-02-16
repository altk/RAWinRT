#include "pch.h"
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include "RAWinRT.BT.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;
using namespace ABI::RAWinRT::BT;
using namespace ABI::Windows::ApplicationModel::Background;

class TestTaskImpl : public RuntimeClass < ABI::Windows::ApplicationModel::Background::IBackgroundTask >
{
	InspectableClass(RuntimeClass_RAWinRT_BT_TestTask, BaseTrust);
public:
	STDMETHODIMP Run(ABI::Windows::ApplicationModel::Background::IBackgroundTaskInstance *taskInstance) override
	{
		ComPtr<IBackgroundTaskDeferral> deferral;
		taskInstance->GetDeferral(
			deferral.GetAddressOf()
			);

		deferral->Complete();
		return S_OK;
	}
};

ActivatableClass(TestTaskImpl);