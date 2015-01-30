using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UVEngine
{
    public class Save
    {
        public GameState[] gs;
        public Save()
        {
            gs = new GameState[GamePage.savenumber];
            for (int i = 0; i < gs.Length; i++)
            {
                gs[i] = new GameState();
            }
        }
    }
}
