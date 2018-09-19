using System;
using System.Collections.Generic;
using System.Text;

namespace D1.Framework.DoSMS
{
    public interface IDoSMS
    {
        bool Send( string code, string mobile,string templateId);
    }
}
