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

	for (size_t k = 0; k < 1; k++)
	{
		auto counter = 0;
		for (float i = 0.0f; i < 1920.0f; i += 30.0f)
		{
			for (float j = 0.0f; j < 1080.0f; j += 15.0f)
			{
				auto txt = ref new TextBlock();
				txt->Text = counter.ToString();
				counter++;
				Canvas::SetLeft(txt, i);
				Canvas::SetTop(txt, j);
				Lst->Children->Append(txt);
			}
		}
	}
}
