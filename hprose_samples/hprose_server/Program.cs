using System;
using System.Collections.Generic;
using Hprose.IO;
using Hprose.Server;

namespace hprose_server
{
    class TestService
    {
        public string Hello(string name)
        {
            return "Hello " + name + "!";
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            HproseHttpListenerServer server = new HproseHttpListenerServer("http://localhost:2012/");
            TestService ts = new TestService();
            server.Add("Hello", ts);
            server.IsCrossDomainEnabled = true;
            //server.CrossDomainXmlFile = "crossdomain.xml";
            server.Start();
            Console.WriteLine("Server started.");
            Console.ReadLine();
            Console.WriteLine("Server stopped.");
        }
    }
}
