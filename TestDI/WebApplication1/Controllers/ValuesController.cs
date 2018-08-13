using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Options;

namespace WebApplication1.Controllers
{
    [Route("api/[controller]")]
    public class ValuesController : Controller
    {
        public ValuesController(Interface1 i1, Interface2 i2, Interface3 i3,IServiceProvider provider)
        {
            Console.WriteLine(i1.function1());
            Console.WriteLine(i2.function2());
            Console.WriteLine(i3.function3());
            var test = provider.GetService<Interface3>();
            Console.WriteLine(test.function3());
        }
        // GET api/values
        [HttpGet]
        public IEnumerable<string> Get()
        {
          
            return new string[] { "value1", "value2" };
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }

        // POST api/values
        [HttpPost]
        public void Post([FromBody]string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
