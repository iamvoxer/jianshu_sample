using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WebApplication1
{
    public class Class1 : Interface1
    {
        public string function1()
        {
            return this.ToString()+ ":" + this.GetHashCode();
        }
    }
    public class Class2 : Interface2
    {
        public string function2()
        {
            return this.ToString() +":"+ this.GetHashCode();
        }
    }
    public class Class3 : Interface3
    {
        public string function3()
        {
            return this.ToString() + ":" + this.GetHashCode();
        }
    }
}
