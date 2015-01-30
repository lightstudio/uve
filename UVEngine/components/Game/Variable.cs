using System;
using System.Windows.Controls;

namespace UVEngine
{
    public class VAR_NUM
    {
        private double value = 0;
        public VAR_NUM(double var)
        {
            this.value = var;
        }
        public double GetValue()
        {
            return this.value;
        }
        public void ShowValue(TextBlock txb)
        {
            txb.Text = txb.Text + Convert.ToString(this.value);
        }
    }
    public class VAR_STR
    {
        private string value = "";
        public VAR_STR(string var)
        {
            this.value = var;
        }
        public string GetVar()
        {
            return this.value;
        }
        public void ShowVar(TextBlock txb)
        {
            txb.Text = txb.Text + this.value;
        }
    }
    public class VAR_BOOL
    {
        private bool value;
        public VAR_BOOL(bool bl)
        {
            this.value = bl;
        }
        public bool GetBool()
        {
            return this.value;
        }
    }

}