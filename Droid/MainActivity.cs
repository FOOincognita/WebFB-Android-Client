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
        IntPtr wfbObj;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            
            var button = FindViewById<Button>(Resource.Id.MyButton);
            var latButton = FindViewById<Button>(Resource.Id.LatButton);
            var destrButton = FindViewById<Button>(Resource.Id.destructButton);

            button.Click += (sender, args) =>
            {
                // var left = int.Parse(FindViewById<EditText>(Resource.Id.left).Text);
                // var right = int.Parse(FindViewById<EditText>(Resource.Id.right).Text);

                string IP;
                string Port;

                try
                {
                    IP = FindViewById<EditText>(Resource.Id.left).Text;
                    Port = FindViewById<EditText>(Resource.Id.right).Text;
                }
                catch
                { // SHOW DIALOG BOX ON EXCEPTION
                    // Theme Chnage??
                    AlertDialog.Builder dialog = new AlertDialog.Builder(this);
                    AlertDialog alert = dialog.Create();
                    alert.SetTitle("Invalid Input");
                    alert.SetMessage("Please Enter Value Into Each Line");
                    alert.SetButton("OK", (c, ev) => { });
                    alert.Show();

                    return;
                }

                wfbObj = WebFB.InitParams(IP, Port);

                FindViewById<TextView>(Resource.Id.result).Text = Convert.ToString(WebFB.getErr(wfbObj)); 
                    
            };
            // Get Lat Button
            latButton.Click += (sender, args) => 
            {
                FindViewById<TextView>(Resource.Id.LatText).Text = Convert.ToString(WebFB.getLat(wfbObj));
            };
            // Close Socket Button
            latButton.Click += (sender, args) => 
            {
                WebFB.destruct(wfbObj);
                FindViewById<TextView>(Resource.Id.closeSockText).Text = "Success";
            };
        }
    }
}

