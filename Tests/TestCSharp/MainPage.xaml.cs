using System.Linq;
using System.Threading.Tasks;

namespace TestCSharp
{
    public class TestClass
    {
        public TestClass(int i)
        {
            Str = i.ToString();
        }

        public string Str { get; private set; }
    }

    public sealed partial class MainPage
    {
        public MainPage()
        {
            InitializeComponent();
            SetItems();
        }

        private async void SetItems()
        {
            var items = await Task.Run(() => Enumerable.Range(0, 4500).Select(i => new TestClass(i)).ToList());
            Lst.ItemsSource = items;
        }
    }
}
