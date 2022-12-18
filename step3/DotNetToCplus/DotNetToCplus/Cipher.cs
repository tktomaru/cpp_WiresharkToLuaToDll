using System;
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


        unsafe
        public static int AES(int encdec, int* type, byte* _key, byte* _iv, byte* _hiradata)
        {
            Crypt wd = new Crypt();

            //平文の文字列
            //string enc_str = "30314030303030303030303030303030";
            //暗号化した後の値
            //string dec_str = "AD780E95C370A53CC7C0E2D279848A77";
            //byte[] decdata = wd.HEX2Bytes(dec_str);

            // 鍵
            //string key_str = "07010203040506020701020304050602";
            //byte[] key = wd.HEX2Bytes(key_str);
            // IV
            //string iv_str =  "00000000000000000000000000000000";
            //byte[] iv = wd.HEX2Bytes(iv_str);

            int[] types = new int[7];
            types[0] = type[0];//128; // KeySize
            types[1] = type[1];//128; // BlockSize
            types[2] = type[2];//1;   // CBC  1  ECB  2 OFB  3  CFB  4 CTS  5
            types[3] = type[3];//1;   // ANSIX923 4 ISO10126 5 None 1 PKCS7 2 Zeros 3
            types[4] = type[4];//16;  // key length
            types[5] = type[5];//16;  // iv length
            types[6] = type[6];//16;  // data length

            byte[] key = new byte[types[4]];
            byte[] iv = new byte[types[5]];
            byte[] decdata = new byte[types[6]];

            for (int ii = 0; ii < types[4]; ii++)
            {
                key[ii] = _key[ii];
            }
            for (int ii = 0; ii < types[5]; ii++)
            {
                iv[ii] = _iv[ii];
            }
            for (int ii = 0; ii < types[6]; ii++)
            {
                decdata[ii] = _hiradata[ii];
            }

            if (encdec == 1)
            {
                var dest = wd.Encrypt(types, key, iv, decdata);
                for (int ii = 0; ii < types[6]; ii++)
                {
                    _hiradata[ii] = dest[ii];
                }
            }
            else
            {
                var dest = wd.Decrypt(types, key, iv, decdata);
                for (int ii = 0; ii < types[6]; ii++)
                {
                    _hiradata[ii] = dest[ii];
                }
            }
            
            

            return 1;
        }
        [DllExport]
        unsafe
        public static int AES_ENC(int* type, byte* _key, byte* _iv, byte* _hiradata)
        {
            return AES(1, type, _key, _iv, _hiradata);
        }

        [DllExport]
        unsafe
        public static int AES_DEC(int *type, byte *_key, byte *_iv, byte *_hiradata)
        {
            return AES(0, type, _key, _iv, _hiradata);
        }
    }
}
