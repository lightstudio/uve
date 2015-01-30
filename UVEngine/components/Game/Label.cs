using System;

namespace UVEngine
{
    public class Label
    {
        public int Label_LineCount;
        public string Label_String;
        static public void GetLabel(Script script, ref Label[] lab_final)
        {
            Label[] lab_temp = new Label[1000];
            for (int i = 0; i < lab_temp.Length; i++)
            {
                lab_temp[i] = new Label();

            }
            int lab_count = 0;
            for (int i = 0; i < script.scriptmain.Length && lab_count < lab_temp.Length; i++)
            {
                if (script.scriptmain[i][0] == '*')
                {
                    lab_temp[lab_count].Label_LineCount = i;
                    lab_temp[lab_count].Label_String = script.scriptmain[i].Remove(0, 1);
                    lab_count++;
                }
            }
            lab_final = new Label[lab_count];
            for (int i = 0; i < lab_count; i++)
            {
                lab_final[i] = lab_temp[i];

            }
        }
        static public void GotoLabel(String Label_Str, Label[] lab, ref int linecount)
        {
            int i = 0, j = 0;
            for (; i < lab.Length && j == 0; i++)
            {
                if (Label_Str.Remove(0, 1) == lab[i].Label_String)
                    j = 1;
            }
            i--;
            if (j == 0) throw new Exception("Label [" + Label_Str + "] Not Found", null);
            else linecount = lab[i].Label_LineCount;
        }

    }
}