using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using PropertyTools.DataAnnotations;
using System.ComponentModel;

using sora;

namespace SoraToolLibrary
{
    public class Material : INotifyPropertyChanged
    {
#pragma warning disable 67
        public event PropertyChangedEventHandler PropertyChanged;
#pragma warning restore 67

        RunaView glView;
        public Material(RunaView glView)
        {
            this.glView = glView;
        }

        [Category("Simple|ConstColor")]
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

        [Category("Simple|Texture")]
        public bool UseTexture
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kTexture); }
            set { glView.SetShaderFlag(value, ShaderFlag.kTexture); }
        }

        [Height(double.NaN, 0, 160)]
        [VisibleBy("UseTexture")]
        public BitmapImage TextureImage { get; set; }

        private string texturePath;
        [VisibleBy("UseTexture")]
        [FilePath("Images files|*.png;*.jpg", ".png")]
        public string TexturePath
        {
            get
            {
                return this.texturePath;
            }
            set
            {
                this.texturePath = value;
                Uri uri = new Uri(this.texturePath);
                BitmapImage bitmap = new BitmapImage(uri);
                this.TextureImage = bitmap;
                //TODO 
            }
        }


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


        [Height(double.NaN, 0, 160)]
        [VisibleBy("UseDiffuseMap")]
        public BitmapImage DiffuseMapImage { get; set; }

        private string diffuseMapPath;
        [VisibleBy("UseDiffuseMap")]
        [FilePath("Images files|*.png;*.jpg", ".png")]
        public string DiffuseMapPath
        {
            get
            {
                return this.diffuseMapPath;
            }
            set
            {
                this.diffuseMapPath = value;
                Uri uri = new Uri(this.diffuseMapPath);
                BitmapImage bitmap = new BitmapImage(uri);
                this.DiffuseMapImage = bitmap;
                //TODO 
            }
        }


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
        [Height(double.NaN, 0, 160)]
        [VisibleBy("UseSpecularMap")]
        public BitmapImage SpecularMapImage { get; set; }

        private string specularMapPath;
        [VisibleBy("UseSpecularMap")]
        [FilePath("Images files|*.png;*.jpg", ".png")]
        public string SpecularMapPath
        {
            get
            {
                return this.specularMapPath;
            }
            set
            {
                this.specularMapPath = value;
                Uri uri = new Uri(this.specularMapPath);
                BitmapImage bitmap = new BitmapImage(uri);
                this.SpecularMapImage = bitmap;
                //TODO 
            }
        }

        [Category("Light|Light")]
        public Color AmbientLight { get; set; }
        public Color DiffuseLight { get; set; }
        public Color SpecularLight { get; set; }
        public bool MoveLight { get; set; }

        [Category("Model|Model")]
        public string ModelName { get; set; }
    }
}