using System;
using System.Globalization;
using UIKit;
using XamarinShared;

namespace iOS
{
    public partial class ViewController : UIViewController
    {
        public ViewController(IntPtr handle) : base(handle)
        {
        }

        partial void UIButton21_TouchUpInside(UIButton sender)
        {
            var left = int.Parse(Left.Text);
            var right = int.Parse(Right.Text);
            var result = CLib.Add(left, right).ToString(CultureInfo.InvariantCulture);
            Result.Text = result;
        }
    }
}