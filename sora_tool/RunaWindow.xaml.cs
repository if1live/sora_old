using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using sora;
using System.ComponentModel;

namespace sora_tool
{
    public class Material
    {
        RunaView glView;
        public Material(RunaView glView)
        {
            this.glView = glView;
        }

        [Category("ConstColor")]
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

        [Category("Texture")]
        public bool UseTexture 
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kTexture); }
            set { glView.SetShaderFlag(value, ShaderFlag.kTexture); }
        }

        [Category("Ambient")]
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

        [Category("Diffuse")]
        public bool UseDiffuse
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kDiffuseColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kDiffuseColor); }
        }
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
        public bool UseDiffuseMap
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kDiffuseMap); }
            set { glView.SetShaderFlag(value, ShaderFlag.kDiffuseMap); }
        }

        [Category("Specular")]
        public bool UseSpecular
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kSpecularColor); }
            set { glView.SetShaderFlag(value, ShaderFlag.kSpecularColor); }
        }
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
        public bool UseSpecularMap
        {
            get { return glView.IsEnabledShaderFlag(ShaderFlag.kSpecularMap); }
            set { glView.SetShaderFlag(value, ShaderFlag.kSpecularMap); }
        }
    }

    public class MaterialModel
    {
        public Material SelectedMaterial { get; set; }
        public MaterialModel(RunaView glView)
        {
            this.SelectedMaterial = new Material(glView)
            {
                ConstColor = Colors.White,
                AmbientColor = Colors.Red,
                DiffuseColor = Colors.White,
                SpecularColor = Colors.Gray,
            };

            //runa view에서 얻어서 쓰기
        }       
    }
    /// <summary>
    /// Interaction logic for RunaWindow.xaml
    /// </summary>
    public partial class RunaWindow : Window
    {
        RunaView glView = null;

        public RunaWindow()
        {
            InitializeComponent();
            glView = new RunaView();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            OpenGLView child1 = new OpenGLView(glView);
            windowsFormsHost1.Child = child1;

            this.DataContext = new MaterialModel(glView);
            this.DataContextChanged += RunaWindow_DataContextChanged;
        }

        void RunaWindow_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            System.Console.WriteLine("fds");
        }
    }
}
