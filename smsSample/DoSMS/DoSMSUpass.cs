
using Microsoft.Extensions.Options;
using Newtonsoft.Json;
using System;
using System.IO;
using System.Net.Http;
using System.Text;

namespace D1.Framework.DoSMS
{
    public class DoSMSUpass : IDoSMS
    {
        private SMSUpassOptions options;
        public DoSMSUpass(IOptions<SMSUpassOptions> _options)
        {
            options = _options.Value;
            if (string.IsNullOrEmpty(options.sid))
            {
                throw new Exception("配置文件中未定义sid");
            }
            if (string.IsNullOrEmpty(options.token))
            {
                throw new Exception("配置文件中未定义token");
            }
            if (string.IsNullOrEmpty(options.appid))
            {
                throw new Exception("配置文件中未定义appid");
            }
        }
        
        private string url = "https://open.ucpaas.com/ol/sms/sendsms";
        public bool Send(string _code, string _mobile, string _templateId)
        {
            if (string.IsNullOrEmpty(_code))
            {
                throw new Exception("验证码不能为空");
            }
            if (string.IsNullOrEmpty(_mobile))
            {
                throw new Exception("手机号不能为空");
            }
            if (string.IsNullOrEmpty(_templateId))
            {
                throw new Exception("模板id不能为空");
            }
            
            using (HttpClient client = new HttpClient())
            {
                var obj =new 
                {
                    sid=options.sid,
                    token = options.token,
                    appid=options.appid,
                    templateid=_templateId,
                    param=_code,
                    mobile=_mobile
                };
                HttpContent content = new StringContent(JsonConvert.SerializeObject(obj), Encoding.UTF8, "application/json");
                using (HttpResponseMessage responseMessage = client.PostAsync(url, content).Result)
                {
                    Byte[] resultBytes = responseMessage.Content.ReadAsByteArrayAsync().Result;
                    string result = Encoding.UTF8.GetString(resultBytes);
                    SendResult sr = JsonConvert.DeserializeObject<SendResult>(result);
                    if (!sr.code.Equals("000000"))
                        throw new Exception("发送短信失败:" + result);
                }
            }
            return true;
        }
        class SendResult
        {
            
            public string code { get; set; }
            public string count { get; set; }
            public string create_date { get; set; }
            public string mobile { get; set; }
            public string msg { get; set; }
            public string smsid { get; set; }
            public string uid { get; set; }
        }
    }

}
