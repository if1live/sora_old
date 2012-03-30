namespace TestLibrary
{
    using System;

    using System;
    using System.Windows;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;

    using PropertyTools.DataAnnotations;
    using System.ComponentModel;

    public class TestSimpleTypes2 : INotifyPropertyChanged
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
}