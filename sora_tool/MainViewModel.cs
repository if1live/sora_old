using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.ComponentModel;
using PropertyTools.DataAnnotations;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Media;

using SoraToolLibrary;
using TestLibrary;

namespace sora_tool
{
    /*
    public class TestSimpleTypes : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

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

    }
     * */

    public class MainViewModel
    {
        object Model { get; set; }
        public object SelectedItem { get; set; }
        public MainViewModel()
        {
            //Model = new TestSimpleTypes();
            Model = new TestSimpleTypes2();
            SelectedItem = Model;
        }

        //public MaterialModel(RunaView glView)
        //
        //Model = new Material(glView);
        //this.SelectedMaterial = Model;
        //}       
    }
}
