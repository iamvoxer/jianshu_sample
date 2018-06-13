using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Hprose.Client;
using Hprose.Common;
using Hprose.IO;

namespace hprose_client
{
    class Program
    {
        static void Main(string[] args)
        {
            HproseHttpClient client = new HproseHttpClient(" http://localhost:2012/");
            client.KeepAlive = true;
            Console.WriteLine(client.Invoke("Hello",new Object[]{"bbbb"}));
            Console.WriteLine(client.Invoke("hello",new Object[]{"cccc"}));
            Console.ReadLine();
        }
    }
}
