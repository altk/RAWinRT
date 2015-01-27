using Windows.UI.Xaml.Data;

namespace Test
{
    public class Class<T>
    {

        public T Value { get; set; }
    }

    public class Derived : Class<int>
    {
    }
}
