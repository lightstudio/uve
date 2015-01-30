using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UVEngine
{
    public class StringToolkit
    {
        public static void CutParam(string un, char cutby, out string[] param)
        {
            bool IsIn = false;
            string[] temppara = new string[100];
            for (int i = 0; i < temppara.Length; i++)
            {
                temppara[i] = "";
            }
            int paramcount = 0;
            for (int i = 0; i < un.Length; i++)
            {
                if (!IsIn && un[i] == '\"')
                {
                    IsIn = true;
                }
                else if (IsIn && un[i] == '\"')
                {
                    IsIn = false;
                }

                if (!IsIn && un[i] == cutby)
                {
                    paramcount++;

                }
                else
                {
                    temppara[paramcount] += un[i];
                }
            }
            param = new string[paramcount + 1];
            for (int i = 0; i < param.Length; i++)
            {
                param[i] = temppara[i];
            }
        }
        public static string GetBefore(string input,char before)
        {
            string returned = "";
            for (int i = 0; i < input.Length; i++)
            {
                if (input[i] != before)
                {
                    returned += input[i];
                }
                else break;
            }
            return returned;
        }
        public static string GetInside(string input,char inside)
        {
            string returned = "";
            bool IsIn = false;
            for (int i = 0; i < input.Length; i++)
            {
                if (!IsIn&&input[i] == inside)
                {
                    IsIn = true;
                }
                else if (IsIn && input[i] == inside)
                {
                    return returned;
                }
                else if (IsIn)
                {
                    returned += input[i];
                }
            }
            return returned;
        }
        public static string GetBetween(string input, char left, char right)
        {
            string returned = "";
            bool IsIn = false;
            for (int i = 0; i < input.Length; i++)
            {
                if (IsIn)
                {
                    if (input[i] == right) return returned;
                    else returned += input[i];
                }
                else
                {
                    if (input[i] == left) IsIn = true;

                }
            }
            return returned;
        }
        #region 表达式分析
        public static string CalcStr(string input,VAR_STR[] var_str,VAR_NUM[] var_num)
        {
            string returned = "";
            string[] temp;
            CutParam(input, '+', out temp);
            for (int i = 0; i < temp.Length && temp[i] != ""; i++)
            {
                if (temp[i][0] == '$')
                {
                    temp[i] = temp[i].Remove(0, 1);
                    try
                    {
                        returned += var_str[int.Parse(temp[i])].GetVar();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (def.IsString && def.def == temp[i])
                            {
                                returned += def.VAR;
                            }
                        }

                    }
                }
                else if (temp[i][0] == '%')
                {
                    temp[i] = temp[i].Remove(0, 1);
                    try
                    {
                        returned += var_num[int.Parse(temp[i])].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == temp[i])
                            {
                                returned += def.VAR;
                            }
                        }
                    }
                }
                else if (temp[i][0] == '-'&&temp[i][1]=='%')
                {
                    temp[i] = temp[i].Remove(0, 2);
                    try
                    {
                        returned += -var_num[int.Parse(temp[i])].GetValue();
                    }
                    catch
                    {
                        foreach (DEF def in GamePage.script.defas)
                        {
                            if (!def.IsString && def.def == temp[i])
                            {
                                returned += "-" + def.VAR;
                            }
                        }
                    }
                }
                else
                {
                    if (temp[i][0] == '\"') returned += GetInside(temp[i], '\"');
                    else returned += temp[i];

                }
            }
            return returned;
        }
        #endregion
    }
}
