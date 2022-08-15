using System;
using System.Runtime.InteropServices;
using System.Text;

namespace NativeCLib
{
    public static class WebFB
    {
        // [DllImport("libCLib", EntryPoint = "clib_add")] public static extern int Add(int left, int right);
        // [DllImport("libCLib", EntryPoint = "clib_sub")] public static extern int Sub(int left, int right);
        // [DllImport("libCLib", EntryPoint = "clib_str", CharSet = CharSet.Ansi)] public static extern IntPtr CStr();
        // Marshal.PtrToStringAnsi(CLib.CStr(string)) <- IntPtr to Str

        // WebFB Constructors
        [DllImport("libCLib", EntryPoint = "create")] public static extern IntPtr Init();

        [DllImport("libCLib", EntryPoint = "createParams")] 
        public static extern IntPtr InitParams([MarshalAs(UnmanagedType.LPStr)] string IP, [MarshalAs(UnmanagedType.LPStr)] string Port); 

        // WebFB Destructor
        [DllImport("libCLib", EntryPoint = "destruct")] public static extern void destruct(IntPtr wfb);

        //WebFB Setters
        [DllImport("libCLib", EntryPoint = "getWFBErr")] public static extern UInt32 getErr(IntPtr wfb);
    }
}
