using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;
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

            var items = new List<Number>
                              {
                                  new LocalNumber(0),
                                  new LocalNumber(1),
                                  new LocalNumber(2),
                                  new LocalNumber(3),
                                  new LocalNumber(4),
                                  new LocalNumber(5),
                                  new LocalNumber(6),
                                  new LocalNumber(7),
                                  new LocalNumber(8),
                                  new LocalNumber(),
                              };
            lst.ItemsSource = items;
            ChangeValue(items[5]);
        }

        async void ChangeValue(Number n)
        {
            await Task.Delay(3000);
            n.Value = 100;
        }

        private class LocalNumber : Number
        {
            public LocalNumber() { }
            public LocalNumber(int value) : base(value) { }
        }
    }
}
