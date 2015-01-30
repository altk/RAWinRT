#pragma once

namespace TestComponentCX
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class TestClass sealed
	{
	private:
		int32_t _int;
		Platform::String^ _str;
	public:
		TestClass(){}

		property int32_t Int
		{
			int32_t get(){ return _int; }
			void set(int32_t value)
			{
				_int = value;
			}
		}

		property Platform::String^ Str
		{
			Platform::String^ get(){ return _str; }
			void set(Platform::String^ value)
			{
				_str = value;
			}
		}
	};
}