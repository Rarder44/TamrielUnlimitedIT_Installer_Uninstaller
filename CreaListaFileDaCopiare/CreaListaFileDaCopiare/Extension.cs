using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreaListaFileDaCopiare
{

    static class Extensions
    {
        static public  string[] RemoveIndexElement(this string[] arr,int index)
        {
            if (index < arr.Length)
            {
                if(arr.Length==1)
                    return new string[0];
                string[] n = new string[arr.Length - 1];
                int j = 0;
                for (int i = 0; i < arr.Length; i++)
                    if (i != index)
                        n[j++] = arr[i];
                
                return n;
            }
           
            return null;
        }
    }

}
