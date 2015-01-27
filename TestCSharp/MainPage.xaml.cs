using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Data;
using DataBinding;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace TestCSharp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage
    {
        public MainPage()
        {
            InitializeComponent();

            DependencyPropertyChangedHelper.SubscribeToEvent(this);

            var list = new List<Number>();
            for (var i = 0; i < 10000; i++)
            {
                list.Add(new Number(i));
            }

            SetItems(list);
        }

        async void SetItems(List<Number> list)
        {
            await Task.Delay(1000);
            var textblocks = new VariableSizedWrapGrid();
            foreach (var number in list)
            {
                textblocks.Children.Add(new TextBlock { Text = number.Value.ToString() });
            }
            Content = textblocks;
        }
    }
}
