//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include <ppltasks.h>
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

MainPage::MainPage()
{
	InitializeComponent();

	create_task([]()
	{
		vector<TestClass^> vec;
		for (size_t i = 0; i < 4500; i++)
		{
			vec.emplace_back(ref new TestClass(i));
		}
		return vec;
	}, task_continuation_context::use_current())
		.then([this](vector<TestClass^> vec)
	{
		auto binding = ref new Binding();
		binding->Path = ref new PropertyPath(L"Int");
		for (auto i : vec)
		{
			auto txt = ref new TextBlock();
			txt->Height = 15;
			txt->Width = 30;
			txt->DataContext = i;
			txt->SetBinding(TextBlock::TextProperty, binding);
			Lst->Items->Append(txt);
		}
		/*Lst->ItemsSource = ref new Vector<TestClass^>(std::move(vec));*/
	}, task_continuation_context::use_current());
}
