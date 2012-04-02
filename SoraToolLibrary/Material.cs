using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using PropertyTools.DataAnnotations;
using System.ComponentModel;
using System.IO;
using System.Collections;
using System.Collections.Generic;

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

            //model list
            Models = new List<string> {
                "sphere",
                "cube",
                "teapot",
                "TrefoilKnot",
                "Torus",
                "KleinBottle",
                "MobiusStrip",
            };
            this.ModelName = "sphere";
        }
        [Category("Material|Ambient")]
        public bool UseAmbient
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kAmbientColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kAmbientColor); }
        }
        [VisibleBy("UseAmbient")]
        public Color AmbientColor
        {
            get
            {
                Byte[] colorData = new Byte[3];
                glView.GetAmbientColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetAmbientColor(value.R, value.G, value.B); }
        }

        public bool UseAmbientMap
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kAmbientMap); }
            set { glView.SetShaderFlag(value, ShaderFlag.kAmbientMap); }
        }

        [Height(double.NaN, 0, 160)]
        [VisibleBy("UseAmbientMap")]
        public BitmapImage AmbientMapImage { get; set; }

        private string ambientMapPath;
        [VisibleBy("UseAmbientMap")]
        [FilePath("Images files|*.png;*.jpg", ".png")]
        public string AmbientMapPath
        {
            get
            {
                return this.ambientMapPath;
            }
            set
            {
                this.ambientMapPath = value;
                Uri uri = new Uri(this.ambientMapPath);
                BitmapImage bitmap = new BitmapImage(uri);
                this.AmbientMapImage = bitmap;

                //get file extension
                FileInfo info = new FileInfo(value);
                string ext = info.Extension;
                ext = ext.ToLower();
                glView.SetAmbientMapPath(value, ext);
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
                Byte[] colorData = new Byte[3];
                glView.GetDiffuseColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetDiffuseColor(value.R, value.G, value.B); }
        }
        //[VisibleBy("UseDiffuse")]
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

                //get file extension
                FileInfo info = new FileInfo(value);
                string ext = info.Extension;
                ext = ext.ToLower();
                glView.SetDiffuseMapPath(value, ext);
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
                Byte[] colorData = new Byte[3];
                glView.GetSpecularColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetSpecularColor(value.R, value.G, value.B); }
        }
        [VisibleBy("UseSpecular")]
        [Slidable(1, 200, 45, 1)]
        public float SpecularShiness
        {
            get { return glView.GetSpecularShininess(); }
            set { glView.SetSpecularShininess(value); }
        }
        //[VisibleBy("UseSpecular")]
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

                //get file extension
                FileInfo info = new FileInfo(value);
                string ext = info.Extension;
                ext = ext.ToLower();
                glView.SetSpecularMapPath(value, ext);
            }
        }

        [Category("Light|Light")]
        public Color AmbientLight
        {
            get
            {
                Byte[] colorData = new Byte[3];
                glView.GetLightAmbientColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetLightAmbientColor(value.R, value.G, value.B); }
        }
        public Color DiffuseLight
        {
            get
            {
                Byte[] colorData = new Byte[3];
                glView.GetLightDiffuseColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetLightDiffuseColor(value.R, value.G, value.B); }
        }
        public Color SpecularLight
        {
            get
            {
                Byte[] colorData = new Byte[3];
                glView.GetLightSpecularColor(ref colorData);
                Color color = ByteArrayToColor(colorData);
                return color;
            }
            set { glView.SetLightSpecularColor(value.R, value.G, value.B); }
        }
        public bool MoveLight
        {
            get { return glView.IsLightMove(); }
            set { glView.SetLightMove(value); }
        }

        [Browsable(false)]
        public List<string> Models { get; set; }

        private string modelName;
        [Category("Model|Model")]
        [ItemsSourceProperty("Models")]
        public string ModelName 
        {
            get { return modelName; }
            set
            {
                modelName = value;
                glView.SetModel(value);
            }
        }

        private Color ByteArrayToColor(Byte[] arr)
        {
            Color color = new Color();
            color.R = arr[0];
            color.G = arr[1];
            color.B = arr[2];
            color.A = 255;
            return color;
        }
    }
}