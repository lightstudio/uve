using Microsoft.Phone.Controls;
using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Resources;

using UVEngineNative;

namespace UVEngine
{
    public class PNGTools
    {
        public static void PNGImageResolution(Stream stream, ref double width, ref double height)
        {
            try
            {
                if (stream != null)
                {
                    
                    BitmapImage image = new BitmapImage();
                    image.SetSource(stream);
                    stream.Position = 0;
                    byte[] header = new byte[8];
                    stream.Read(header, 0, header.Length);
                    if (header[0] == 0x89 &&
                        header[1] == 0x50 && // P
                        header[2] == 0x4E && // N
                        header[3] == 0x47 && // G
                        header[4] == 0x0D && // CR
                        header[5] == 0x0A && // LF
                        header[6] == 0x1A && // EOF
                        header[7] == 0x0A)   // LF
                    {
                        byte[] buffer = new byte[16];
                        stream.Read(buffer, 0, buffer.Length);
                        Array.Reverse(buffer, 8, 4);
                        Array.Reverse(buffer, 12, 4);

                        width = BitConverter.ToInt32(buffer, 8);
                        height = BitConverter.ToInt32(buffer, 12);

                        return;
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                width = 0;
                height = 0;
            }
        }
    }
    public class JPGTools
    {
        public static void getJpgSize(FileStream fs, out Size JpgSize)
        {
            JpgSize = new Size(0, 0);
            int rx = 0;
             try
             {
                 if (fs != null)
                 {
//                     Stream F_Stream = fs;
                     
                     int ff = fs.ReadByte();
                     int type = fs.ReadByte();
                     if (ff != 0xff || type != 0xd8)
                     {
//                         F_Stream.Close();
//                         fs.Close();
                         return;
                     }
                     long ps = 0;
                     do
                     {
                         do
                         {
                             ff = fs.ReadByte();
                             if (ff < 0) 
                             {
//                                 F_Stream.Close();
//                                 fs.Close();
                                 return;
                             }
                         } while (ff != 0xff);

                         do
                         {
                             type = fs.ReadByte();
                         } while (type == 0xff);

                         ps = fs.Position;
                         switch (type)
                         {
                             case 0x00:
                             case 0x01:
                             case 0xD0:
                             case 0xD1:
                             case 0xD2:
                             case 0xD3:
                             case 0xD4:
                             case 0xD5:
                             case 0xD6:
                             case 0xD7:
                                 break;
                             case 0xc0:
                                 ps = fs.ReadByte() * 256;
                                 ps = fs.Position + ps + fs.ReadByte() - 2; 

                                 fs.ReadByte();
                                 
                                 JpgSize.Height = fs.ReadByte() * 256;
                                 JpgSize.Height = JpgSize.Height + fs.ReadByte();
                                 
                                 JpgSize.Width = fs.ReadByte() * 256;
                                 JpgSize.Width = JpgSize.Width + fs.ReadByte();
                                 
                                 if (rx != 1 && rx < 3) rx = rx + 1;
                                 break;
                             case 0xe0: 
                                 ps = fs.ReadByte() * 256;
                                 ps = fs.Position + ps + fs.ReadByte() - 2; 

                                 fs.Seek(5, SeekOrigin.Current); 
                                 fs.Seek(2, SeekOrigin.Current); 
                                 int units = fs.ReadByte(); 

                                 
                                 
                                 
                                 if (rx != 2 && rx < 3) rx = rx + 2;
                                 break;

                             default: 
                                 ps = fs.ReadByte() * 256;
                                 ps = fs.Position + ps + fs.ReadByte() - 2;
                                 break;
                         }
                         if (ps + 1 >= fs.Length) 
                         {
//                             fs.Close();
//                             fs.Close();
                             return;
                         }
                         fs.Position = ps;
                     } while (type != 0xda);
//                     F_Stream.Close();
//                     fs.Close();
                     
                     return;
                 }
             }
             catch (Exception e)
             {
                 MessageBox.Show(e.Message);
             }
        }


    }
    public class Alpha
    {
        static public void AlphaBlend(FileStream fs, ref Image PictureBox) ///请勿在未释放资源的情况下重复调用方法，否则会引发System.IO异常
        {

            Size jpgSize = new Size();
            JPGTools.getJpgSize(fs, out jpgSize);
//            BitmapImage bi = new BitmapImage();
//            bi.SetSource(fs);
//            PictureBox.Source = bi;
            PictureBox.Height = jpgSize.Height * GamePage.zoom;
            PictureBox.Width = jpgSize.Width * GamePage.zoom;
            fs.Position = 0;
            WriteableBitmap bitmapImage = new WriteableBitmap(PictureBox, null);
            bitmapImage.SetSource(fs);
            WriteableBitmap final = new WriteableBitmap(bitmapImage.PixelWidth / 2, bitmapImage.PixelHeight);
            Image imgBase = new Image();
            imgBase.Source = bitmapImage;
            imgBase.Stretch = Stretch.None;
            WriteableBitmap unAlpha = new WriteableBitmap(bitmapImage.PixelWidth / 2, bitmapImage.PixelHeight);
            unAlpha.Render(imgBase, null);
            unAlpha.Invalidate();
            WriteableBitmap alpha = new WriteableBitmap(bitmapImage.PixelWidth / 2, bitmapImage.PixelHeight);
            TranslateTransform translate = new TranslateTransform();
            translate.X = -bitmapImage.PixelWidth / 2;//在绘制到位图中之前应用到元素的变换
            alpha.Render(imgBase, translate);//在位图中呈现元素
            alpha.Invalidate();//请求重绘整个位图
            if (GamePage.useNative)
            {
                int[] temp = ImageToolkitNative.UnAlpha(alpha.Pixels, unAlpha.Pixels, bitmapImage.PixelWidth, bitmapImage.PixelHeight);
                for (int i = 0; i < temp.Length; i++)
                {
                    final.Pixels[i] = temp[i];
                }
            }
            else
            {
                for (int i = 0; i < bitmapImage.PixelHeight; i++)
                {
                    for (int k = 0; k < bitmapImage.PixelWidth / 2; k++)
                    {
                        uint gray = 0;
                        int temp;
                        int access = i * bitmapImage.PixelWidth / 2 + k;
                        uint r, g, b;
                        temp = unAlpha.Pixels[access];
                        gray = (uint)alpha.Pixels[access];
                        gray <<= 8;
                        gray >>= 8;
                        r = (uint)gray >> 16;
                        g = (uint)(gray << 16) >> 24;
                        b = (uint)(gray << 24) >> 24;
                        gray = (r * 30 + g * 59 + b * 11 + 50) / 100;
                        gray = 255 - gray;
                        gray <<= 24;
                        final.Pixels[access] = (int)gray | (temp & 16777215);
                    }
                }
            }
            PictureBox.Source = final;
            alpha = null;
            bitmapImage = null;
            unAlpha = null;
            final = null;
            GC.Collect();
            fs.Close();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="fs"></param>
        /// <param name="pos">开始的位置</param>
        /// <param name="size">坐标</param>
        /// <param name="PictureBox"></param>
        static public void CutImage(FileStream fs, Size pos, Size size, ref Image PictureBox)
        {
            
            Image img = PictureBox;
            WriteableBitmap bitmapImage = new WriteableBitmap(PictureBox, null);
            Image imgBase = new Image();
            bitmapImage.SetSource(fs);
            bitmapImage.Invalidate();
            imgBase.Source = bitmapImage;
            imgBase.Stretch = Stretch.None;
            WriteableBitmap final = new WriteableBitmap((int)size.Width, (int)size.Height);
            TranslateTransform translate = new TranslateTransform();
            translate.X = -pos.Width;
            translate.Y = -pos.Height;
            final.Render(imgBase, translate);
            final.Invalidate();
            PictureBox.Source = final;
        }

    }
}