using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Animation;
using UVEngine2_1.Classes;

namespace UVEngine2_1.Controls.GameSelector
{
    public partial class GameSelector : UserControl
    {
        public readonly DependencyProperty GamesProperty = DependencyProperty.Register("Games", typeof (GameList),
            typeof (GameSelector),
            new PropertyMetadata(GamesPropertyChangedCallback));

        public GameSelector()
        {
            InitializeComponent();
        }

        public GameList Games
        {
            get { return (GameList) GetValue(GamesProperty); }
            set { SetValue(GamesProperty, value); }
        }

        private static void GamesPropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (!(sender is GameSelector)) return;
            var gameSelector = (GameSelector) sender;
            gameSelector.Selector.DataContext = arg.NewValue;
        }

        private void GameSelected(object sender, SelectionChangedEventArgs e)
        {
            var selectedGame = (Game) (Selector.SelectedItem);
            foreach (var game in Games.Games.Where(game => game.IsSelected))
            {
                game.IsSelected = false;
            }
            selectedGame.IsSelected = true;
        }
    }
}