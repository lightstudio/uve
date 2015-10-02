using System;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace UVEngine2_1
{
    public partial class MainPage : PhoneApplicationPage
    {
        // 构造函数
        public MainPage()
        {
            InitializeComponent();

            // 用于本地化 ApplicationBar 的示例代码
            //BuildLocalizedApplicationBar();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            NavigationService.Navigate(new Uri("/Pages/GameList/GameList.xaml", UriKind.Relative));
        }
    }
}