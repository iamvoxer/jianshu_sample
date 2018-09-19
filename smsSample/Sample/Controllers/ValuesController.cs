using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using D1.Framework.DoSMS;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Options;

namespace Sample.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ValuesController : ControllerBase
    {
        IDoSMS sms;
        public ValuesController(IDoSMS _sms)
        {
            sms = _sms;
        }
        // GET api/values
        [HttpGet]
        public ActionResult<IEnumerable<string>> Get()
        {
            bool b = sms.Send("88928", "1851111234", "311230");
            return new string[] { "value1", "value2"+b };
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public ActionResult<string> Get(int id)
        {
            return "value";
        }

        // POST api/values
        [HttpPost]
        public void Post([FromBody] string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
