using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Server.Kestrel.Core.Internal.Http;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MiddlewareSample
{
    public class OtherMiddleware
    {
        private readonly RequestDelegate next;
        public OtherMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext context /* other scoped dependencies */)
        {

            //这个例子只是修改一下response的header
            context.Response.OnStarting(state => {
                var httpContext = (HttpContext)state;
                httpContext.Response.Headers.Add("test1", "testvalue1" );
                return Task.FromResult(0);
            }, context);


            await next(context);
            
        }
    }
}
