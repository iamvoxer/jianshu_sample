using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace SignService.Controllers
{
    [Route("api/uploadsign")]
    public class ValuesController : Controller
    {
        // GET api/values
        [HttpGet]
        public string Get()
        {
            return "";
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }

        // POST api/values
        [HttpPost]
        public string Post()
        {
            Signature sign = new Signature();
            sign.m_strSecId = "用自己的secid";
            sign.m_strSecKey = "用自己的seckey";
            sign.m_qwNowTime = Signature.GetIntTimeStamp();
            sign.m_iRandom = new Random().Next(0, 1000000);
            sign.m_iSignValidDuration = 3600 * 24 * 2;
            return sign.GetUploadSignature();
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
