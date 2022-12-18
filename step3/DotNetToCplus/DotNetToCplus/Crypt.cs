using System.Security.Cryptography;
using System.Globalization;

namespace DotNetToCplus
{
    public class Crypt
    {

        static byte[] zeroIV = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        public byte[] HEX2Bytes(string hex)
        {
            if (hex.Length % 2 != 0)
            {
                throw new System.ArgumentException(System.String.Format(CultureInfo.InvariantCulture,
                                                          "The binary key cannot have an odd number of digits: {0}", hex));
            }

            byte[] HexAsBytes = new byte[hex.Length / 2];
            for (int index = 0; index < HexAsBytes.Length; index++)
            {
                string byteValue = hex.Substring(index * 2, 2);
                HexAsBytes[index] = byte.Parse(byteValue, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            }

            return HexAsBytes;
        }

        public byte[] Encrypt(int[] type, byte[] key, byte[] iv, byte[] input)
        {
            var aesAlg = new AesManaged
            {
                KeySize = type[0],    // 128,
                Key = key,
                BlockSize = type[1], // 128,
                Mode = (CipherMode)type[2],       // CBC  1  ECB  2 OFB  3  CFB  4 CTS  5
                Padding = (PaddingMode)type[3],   // PaddingMode.Zeros,
                //IV = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
                IV = iv
            };

            ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);
            return encryptor.TransformFinalBlock(input, 0, input.Length);
        }

 
        public byte[] Decrypt(int[] type, byte[] key, byte[] iv, byte[] input)
        {
            if (type.Length >= 4)
            {
                var aesAlg = new AesManaged
                {
                    KeySize = type[0],    // 128,
                    Key = key,
                    BlockSize = type[1], // 128,
                    Mode = (CipherMode)type[2],       // CBC  1  ECB  2 OFB  3  CFB  4 CTS  5
                    Padding = (PaddingMode)type[3],   // PaddingMode.Zeros,
                                         //IV = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
                    IV = iv
                };


                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                return decryptor.TransformFinalBlock(input, 0, input.Length);
            }
            else
            {
                return null;
            }
        }
    }
}
