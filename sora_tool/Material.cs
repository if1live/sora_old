using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sora;
using System.ComponentModel;
using PropertyTools.DataAnnotations;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Media;

namespace sora_tool
{
    

    public class Material : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        RunaView glView;
        public Material(RunaView glView)
        {
            this.glView = glView;
            //string str = "C:\\devel\\sora\\yuriko\\assets\\texture\\sora.png";
            //Uri uri = new Uri(str);
            //BitmapImage bitmap = new BitmapImage(uri);    
        }

        //[Category("Material|ConstColor")]
        //[Height(double.NaN, 0, 160)]
        public BitmapImage TmpImage { get; set; }
        private string tmp_path;
        [FilePath("Images files|*.png;*.jpg", ".png")]
        public string TmpPath
        {
            get
            {
                return this.tmp_path;
            }
            set
            {
                this.tmp_path = value;
                Uri uri = new Uri(this.tmp_path);
                BitmapImage bitmap = new BitmapImage(uri);
                this.TmpImage = bitmap;
            }
        }

        /*
        public bool UseConstColor 
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kConstColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kConstColor); }
        }

        public Color ConstColor 
        {
            get 
            {
                float[] colorData = new float[3];
                Color color = new Color();
                unsafe
                {
                    fixed (float* ptr = colorData)
                    {
                        glView.GetConstColor(ptr);
                        color.R = (byte)(ptr[0] * 255);
                        color.G = (byte)(ptr[1] * 255);
                        color.B = (byte)(ptr[2] * 255);
                        color.A = 255;
                    }                   

                }
                return color; 
            }
            set
            {
                float r = value.R / 255.0f;
                float g = value.G / 255.0f;
                float b = value.B / 255.0f;
                glView.SetConstColor(r, g, b);
            }
        }

        [Category("Material|Texture")]
        public bool UseTexture 
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kTexture); }
            set { glView.SetShaderFlag(value, ShaderFlag.kTexture); }
        }
        [FilePath(".png", true)]
        [FilterProperty("Filter")]
        [VisibleBy("UseTexture")]
        public string TexturePath { get; set; }

        [Category("Material|Ambient")]
        public bool UseAmbient
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kAmbientColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kAmbientColor); }
        }
        public Color AmbientColor
        {
            get
            {
                float[] colorData = new float[3];
                Color color = new Color();
                unsafe
                {
                    fixed (float* ptr = colorData)
                    {
                        glView.GetAmbientColor(ptr);
                        color.R = (byte)(ptr[0] * 255);
                        color.G = (byte)(ptr[1] * 255);
                        color.B = (byte)(ptr[2] * 255);
                        color.A = 255;
                    }

                }
                return color;
            }
            set
            {
                float r = value.R / 255.0f;
                float g = value.G / 255.0f;
                float b = value.B / 255.0f;
                glView.SetAmbientColor(r, g, b);
            }
        }

        [Category("Material|Diffuse")]
        public bool UseDiffuse
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kDiffuseColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kDiffuseColor); }
        }
        [VisibleBy("UseDiffuse")]
        public Color DiffuseColor
        {
            get
            {
                float[] colorData = new float[3];
                Color color = new Color();
                unsafe
                {
                    fixed (float* ptr = colorData)
                    {
                        glView.GetDiffuseColor(ptr);
                        color.R = (byte)(ptr[0] * 255);
                        color.G = (byte)(ptr[1] * 255);
                        color.B = (byte)(ptr[2] * 255);
                        color.A = 255;
                    }

                }
                return color;
            }
            set
            {
                float r = value.R / 255.0f;
                float g = value.G / 255.0f;
                float b = value.B / 255.0f;
                glView.SetDiffuseColor(r, g, b);
            }
        }
        [VisibleBy("UseDiffuse")]
        public bool UseDiffuseMap
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kDiffuseMap); }
            set { glView.SetShaderFlag(value, ShaderFlag.kDiffuseMap); }
        }
        [FilePath(".png", true)]
        [FilterProperty("Filter")]
        [VisibleBy("UseDiffuseMap")]
        public string DiffuseMapPath { get; set; }

        [Category("Material|Specular")]
        public bool UseSpecular
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kSpecularColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kSpecularColor); }
        }
        [VisibleBy("UseSpecular")]
        public Color SpecularColor
        {
            get
            {
                float[] colorData = new float[3];
                Color color = new Color();
                unsafe
                {
                    fixed (float* ptr = colorData)
                    {
                        glView.GetSpecularColor(ptr);
                        color.R = (byte)(ptr[0] * 255);
                        color.G = (byte)(ptr[1] * 255);
                        color.B = (byte)(ptr[2] * 255);
                        color.A = 255;
                    }

                }
                return color;
            }
            set
            {
                float r = value.R / 255.0f;
                float g = value.G / 255.0f;
                float b = value.B / 255.0f;
                glView.SetSpecularColor(r, g, b);
            }
        }
        [VisibleBy("UseSpecular")]
        [Slidable(1, 200, 45, 1)]
        public float SpecularShiness
        {
            get
            {
                return glView.GetSpecularShininess();
            }
            set
            {
                glView.SetSpecularShininess(value);
            }
        }
        [VisibleBy("UseSpecular")]
        public bool UseSpecularMap
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kSpecularMap); }
            set { glView.SetShaderFlag(value, ShaderFlag.kSpecularMap); }
        }
        [FilePath(".png", true)]
        [FilterProperty("Filter")]
        [VisibleBy("UseSpecularMap")]
        public string SpecularMapPath { get; set; }

        [Category("Light|Light")]
        public Color AmbientLight { get; set; }
        public Color DiffuseLight { get; set; }
        public Color SpecularLight { get; set; }
        public bool MoveLight { get; set; }

        [Category("Model|Model")]
        public string ModelName { get; set; }
         * */
    }
}
