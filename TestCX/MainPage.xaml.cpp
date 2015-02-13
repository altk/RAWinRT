//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include <ppltasks.h>
#include "memoryapi.h"
#include "MainPage.xaml.h"
#include <vector>
#include "collection.h"

using namespace std;
using namespace TestCX;
using namespace Concurrency;
using namespace TestComponentWRL;
using namespace Platform::Collections;
using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml;

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Local\\MyFileMappingObject");

MainPage::MainPage()
{
	InitializeComponent();
}
