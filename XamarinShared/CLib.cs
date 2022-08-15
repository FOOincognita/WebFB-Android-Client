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
        [DllImport("libCLib", EntryPoint = "createParams")] public static extern IntPtr InitParams(int num, [MarshalAs(UnmanagedType.LPStr)] string str); ///!!!!!!

        // WebFB Getters
        [DllImport("libCLib", EntryPoint = "getWFBNum")] public static extern int getNum(IntPtr wfbPtr);
        [DllImport("libCLib", EntryPoint = "getWFBStr", CharSet = CharSet.Ansi)] public static extern IntPtr getStr(IntPtr wfbPtr);

        //WebFB Setters
        [DllImport("libCLib", EntryPoint = "setWFBNum")] public static extern void setNum(IntPtr wfbPtr, int num);
        [DllImport("libCLib", EntryPoint = "setWFBStr")] public static extern void setStr(IntPtr wfbPtr, [MarshalAs(UnmanagedType.LPStr)] string str); ///!!!!!!
    }
}
