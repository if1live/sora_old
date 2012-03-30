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
using PropertyTools.DataAnnotations;

namespace sora_tool
{
   
    /// <summary>
    /// Interaction logic for RunaWindow.xaml
    /// </summary>
    public partial class RunaWindow : Window
    {
        RunaView glView = null;

        public RunaWindow()
        {
            InitializeComponent();
            //glView = new RunaView();
            //MaterialModel model = new MaterialModel(glView);
            MainViewModel model = new MainViewModel();
            DataContext = model;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            OpenGLView child1 = new OpenGLView(glView);
            windowsFormsHost1.Child = child1;
        }

        private void FileExitClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
