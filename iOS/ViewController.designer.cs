// WARNING
//
// This file has been generated automatically by Xamarin Studio from the outlets and
// actions declared in your storyboard file.
// Manual changes to this file will not be maintained.
//
using Foundation;
using System;
using System.CodeDom.Compiler;
using UIKit;

namespace iOS
{
	[Register ("ViewController")]
	partial class ViewController
	{
		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UITextField Left { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UILabel Result { get; set; }

		[Outlet]
		[GeneratedCode ("iOS Designer", "1.0")]
		UITextField Right { get; set; }

		[Action ("UIButton21_TouchUpInside:")]
		[GeneratedCode ("iOS Designer", "1.0")]
		partial void UIButton21_TouchUpInside (UIButton sender);

		void ReleaseDesignerOutlets ()
		{
			if (Left != null) {
				Left.Dispose ();
				Left = null;
			}
			if (Result != null) {
				Result.Dispose ();
				Result = null;
			}
			if (Right != null) {
				Right.Dispose ();
				Right = null;
			}
		}
	}
}
