using Windows.UI.Xaml.Data;

namespace Test
{
    public abstract class Class 
    {
        public int Value { get; set; }

        public abstract int GetValue();
    }

    public class ClassDerived : Class
    {
        public override int GetValue()
        {
            return Value;
        }
    }
}
