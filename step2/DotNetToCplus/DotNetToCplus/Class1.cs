﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;  // Marshalを使うのに必要

namespace DotNetToCplus
{
    public class Class1
    {
        [DllExport]
        public static int Count(IntPtr stringPtr)
        {
            var str = Marshal.PtrToStringAuto(stringPtr);    // IntPtrをstring型に変換
            return str?.Length ?? 0;                         // intやdoubleなどのプリミティブはそのままreturnでOK!!
        }

        [DllExport]
        public static IntPtr Through(IntPtr stringPtr)
        {
            var str = Marshal.PtrToStringAuto(stringPtr);    // IntPtrをstring型に変換
            return Marshal.StringToHGlobalAuto(str);         // string型をIntPtrに変換してreturn
        }

        [DllExport]
        public static IntPtr GetString()
        {
            return Marshal.StringToHGlobalAuto("hoge fuga piyo");    // string型をIntPtrに変換してreturn
        }
        [DllExport]
        public static int Sum(int a, int b)
        {
            return a + b;
        }
    }
}
