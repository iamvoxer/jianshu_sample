using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Server.Kestrel.Core.Internal.Http;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MiddlewareSample
{
    public class AuthorizeMiddleware
    {
        private readonly RequestDelegate next;
        public AuthorizeMiddleware(RequestDelegate next)
        {
            this.next = next;
        }

        public async Task Invoke(HttpContext context /* other scoped dependencies */)
        {

            //以下代码都不是必须的，只是展示一些使用方法，你可以选择使用

            //设置httpcontext的一些key和value，用于在整个http请求过程中分享数据
            context.Items.Add("aaa", "bbb");

            //判断当前http请求的url是否处理
            if (context.Request.Path.StartsWithSegments("/api"))
            {
                //dosomething
            }

            //token类型是bearer类型
            if (context.Request.Headers.ContainsKey("Authorization"))
            {
                var header = context.Request.Headers as FrameRequestHeaders;

                var value = header.HeaderAuthorization.First<string>(v => v.ToLower().StartsWith("bearer "));

                if (value == null)
                {
                    //dosomething
                }
                string _token = value.ToString().Substring(7);
            }
            //获取cookie
            var cookie = context.Request.Cookies["BAIDUD"];
            if (cookie == null)
            {
                //dosomething
            }

            //从http的url参数获取一个值
            var value1 = context.Request.Query.FirstOrDefault(u => u.Key == "accesstoken");
            if (value1.Value.Count <= 0)
            {
                //dosomething
            }
            //这个例子只是修改一下response的header
            context.Response.OnStarting(state => {
                var httpContext = (HttpContext)state;
                httpContext.Response.Headers.Add("test2", "testvalue2");
                return Task.FromResult(0);
            }, context);
            await next(context);
            
        }
    }
}
