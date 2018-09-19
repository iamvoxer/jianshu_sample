using Microsoft.Extensions.Options;
using System;
using System.Collections.Generic;
using System.Text;

namespace D1.Framework.DoSMS
{


    public class SMSUpassOptions : IOptions<SMSUpassOptions>
    {
        public SMSUpassOptions Value => this;
        public string sid { get; set; }

        public string token { get; set; }
        
        public string appid { get; set; }
    }
}
