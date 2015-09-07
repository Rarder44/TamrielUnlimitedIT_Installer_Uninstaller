using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreaListaFileDaCopiare
{
    class PathService
    {

        public static string RemoveHeadSlashes(string s)
        {
            while (s.StartsWith("\\") || s.StartsWith("/"))
                s = s.Remove(0, 1);
            return s;
        }
        public static string RemoveTailSlashes(string s)
        {
            while (s.EndsWith("\\") || s.EndsWith("/"))
                s = s.Substring(0, s.Length - 1);
            return s;

        }
        public static string ComprimePath(params string[] arguments)
        {
            if (arguments.Length == 0)
                return "";
            else
            {
                string pre = RemoveTailSlashes(arguments[0]).Trim();
                string post = ComprimePath(arguments.RemoveIndexElement(0)).Trim();
                if (post == "")
                {
                    return RemoveHeadSlashes(pre);
                }
                post = RemoveHeadSlashes(post);
                return pre + "\\" + post;
            }
        }

        public static string[] SplitPathToArray(string Path)
        {
            return Path.Split('\\', '/');
        }
        

        public static string AbsoluteToRelative(string Absolute,string BasePath)
        {
            string[] a = SplitPathToArray(Absolute);
            string[] b = SplitPathToArray(BasePath);
            string[] n = new string[a.Length-b.Length];

            if (b.Length > a.Length)
                return null;

            for(int i=0;i<b.Length;i++)
                if (a[i] != b[i])
                    return null;

            int j = 0;
            for (int i = b.Length; i < a.Length; i++)
                n[j++] = a[i];

            return ComprimePath(n);
        }
        public static List<string> AbsoluteToRelative(List<string> Absolutes, string BasePath)
        {
            List<string> l = new List<string>();
            foreach(string s in Absolutes)
                l.Add(AbsoluteToRelative(s, BasePath));
            return l;
        }

        public static string StripSlash(string Path)
        {
            return Path.Replace("\\", "\\\\");
        }


    }
}
