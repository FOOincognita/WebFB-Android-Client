using System.Globalization;
using Android.App;
using Android.Widget;
using Android.OS;
using NativeCLib;
using System.Runtime.InteropServices;
using Android.Views;
using System.Text;
using System;

namespace Droid
{
    [Activity(Label = "Test App", MainLauncher = true, Icon = "@drawable/icon", Theme = "@android:style/Theme.NoTitleBar")]
    public class MainActivity : Activity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            
            var button = FindViewById<Button>(Resource.Id.MyButton);

            button.Click += (sender, args) =>
            {
                IntPtr wfbObj = WebFB.InitParams(420, "InitStrVal");

                string tstStr = Marshal.PtrToStringAnsi(WebFB.getStr(wfbObj)) + "; ";
                WebFB.setStr(wfbObj, "NewStrVal");

                tstStr += Marshal.PtrToStringAnsi(WebFB.getStr(wfbObj));


                var left = int.Parse(FindViewById<EditText>(Resource.Id.left).Text);
                var right = int.Parse(FindViewById<EditText>(Resource.Id.right).Text);
                FindViewById<TextView>(Resource.Id.result).Text = tstStr; //CLib.Sub(left, right)

                /*
                catch 
                { // SHOW DIALOG BOX ON EXCEPTION
                    // Theme Chnage??
                    AlertDialog.Builder dialog = new AlertDialog.Builder(this);
                    AlertDialog alert = dialog.Create();
                    alert.SetTitle("Invalid Input");
                    alert.SetMessage("Please Enter Integer In Each Container");
                    alert.SetButton("OK", (c, ev) =>
                    {
                        // Ok button click task  
                    });
                    alert.Show();
                }
                */
                    
            };
        }
    }
}

