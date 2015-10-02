using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using UVEngine2_1.Classes;

namespace UVEngine2_1.Pages.GameList
{
    public partial class GameList : PhoneApplicationPage
    {
        private string SelectedGameHash { get; set; }
        private Classes.GameList CurrentGames { get; set; }
        public GameList()
        {
            SelectedGameHash = "null";
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CurrentGames = GameRuntime.DebugGames();
            Selector.Games = CurrentGames;
        }
    }
}