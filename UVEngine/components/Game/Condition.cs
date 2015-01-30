using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Threading.Tasks;
using UVEngineNative;

namespace UVEngine
{
    public class Condition
    {
        static public bool cmp(double a, double b, int type)
        {
            switch (type)
            {
                case 1:
                    return a == b;
                case 2:
                    return a > b;
                case 3:
                    return a >= b;
                case 4:
                    return a < b;
                case 5:
                    return a <= b;
                case 6:
                    return a != b;
            }
            return false;
        }
        static public bool JudgeCondition(string se, VAR_NUM[] num)
        {
            string a = "", b = "";
            int type = 0;// =/==:1,>:2,>=:3,<:4,<=:5,!=/<>:6
            for (int i = 0, j = 0; i < se.Length; i++)
            {
                if (j == 0) a += se[i];
                else if (j == 2) b += se[i];
                if (se.Length > i + 1 && (se[i + 1] == '>' || se[i + 1] == '<' || se[i + 1] == '=' || se[i + 1] == '!'))
                {
                    j = 2;
                    if (se.Length > i + 2 && (se[i + 2] == '=' || se[i + 2] == '>'))
                    {
                        i++;
                    }
                    i++;
                }
            }

            if (se.Contains("="))
            {
                if (se.Contains(">"))
                    type = 3;
                else if (se.Contains("<"))
                    type = 5;
                else if (se.Contains("!"))
                    type = 6;
                else
                    type = 1;
            }
            else if (se.Contains("<"))
            {
                if (se.Contains(">"))
                    type = 6;
                else
                    type = 4;
            }
            else if (se.Contains(">"))
            {
                type = 2;
            }

            if (a[0] == '%')
            {
                if (b[0] == '%')
                {
                    double value_A = 0, value_B = 0;
                    try
                    {
                        value_A = num[int.Parse(a.Remove(0, 1))].GetValue();

                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == a.Remove(0,1))
                            {
                                value_A = (double)def.VAR;
                            }
                        }

                    }
                    try
                    {
                        value_B = num[int.Parse(b.Remove(0, 1))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 1))
                            {
                                value_B = (double)def.VAR;
                            }
                        }

                    }
                    return cmp(value_A, value_B, type);
                }
                else if (b[0] == '-' && b[1] == '%')
                {
                    double value_A = 0, value_B = 0;
                    try
                    {
                        value_A = num[int.Parse(a.Remove(0, 1))].GetValue();

                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == a.Remove(0, 1))
                            {
                                value_A = (double)def.VAR;
                            }
                        }

                    }
                    try
                    {
                        value_B = -num[int.Parse(b.Remove(0, 2))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 2))
                            {
                                value_B = -(double)def.VAR;
                            }
                        }
                    }
                    return cmp(value_A, value_B, type);
                }
                else
                {
                    return cmp(num[int.Parse(a.Remove(0,1))].GetValue(), double.Parse(b), type);
                }
            }
            else if (a[0] == '-' && a[1] == '%')
            {
                if (b[0] == '%')
                {
                    double value_A = 0, value_B = 0;
                    try
                    {
                        value_A = -num[int.Parse(a.Remove(0, 2))].GetValue();

                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == a.Remove(0, 2))
                            {
                                value_A = -(double)def.VAR;
                            }
                        }

                    }
                    try
                    {
                        value_B = num[int.Parse(b.Remove(0, 1))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 1))
                            {
                                value_B = (double)def.VAR;
                            }
                        }

                    }
                    return cmp(value_A, value_B, type);
                }
                else if (b[0] == '-' && b[1] == '%')
                {
                    double value_A = 0, value_B = 0;
                    try
                    {
                        value_A = -num[int.Parse(a.Remove(0, 2))].GetValue();

                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == a.Remove(0, 2))
                            {
                                value_A = -(double)def.VAR;
                            }
                        }

                    }
                    try
                    {
                        value_B = -num[int.Parse(b.Remove(0, 2))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 2))
                            {
                                value_B = -(double)def.VAR;
                            }
                        }
                    }
                    return cmp(value_A, value_B, type);
                }
                else
                {
                    return cmp(num[int.Parse(a.Remove(0, 1))].GetValue(), double.Parse(b), type);
                }




            }
            else
            {
                if (b[0] == '%')
                {
                    double value_B = 0;
                    try
                    {
                        value_B = num[int.Parse(b.Remove(0, 1))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 1))
                            {
                                value_B = (double)def.VAR;
                            }
                        }

                    }
                    return cmp(double.Parse(a), value_B, type);
                }
                else if (b[0] == '-' && b[1] == '%')
                {
                    double value_B = 0;
                    try
                    {
                        value_B = -num[int.Parse(b.Remove(0, 2))].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == b.Remove(0, 2))
                            {
                                value_B = -(double)def.VAR;
                            }
                        }
                    }
                    return cmp(double.Parse(a), value_B, type);
                }
                else
                {
                    return cmp(double.Parse(a), double.Parse(b), type);
                }
            }
        }
    }
}
