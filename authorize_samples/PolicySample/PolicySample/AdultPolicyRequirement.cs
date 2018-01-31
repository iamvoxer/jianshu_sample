using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Controllers;
using Microsoft.AspNetCore.Mvc.Filters;
using Microsoft.AspNetCore.Server.Kestrel.Core.Internal.Http;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Threading.Tasks;

namespace PolicySample
{
    public class AdultPolicyRequirement : IAuthorizationRequirement
    {
        public int Age { get; }
        public AdultPolicyRequirement(int age)
        {
            //年龄限制
            this.Age = age;
        }
    }
    public class AdultAuthorizationHandler : AuthorizationHandler<AdultPolicyRequirement>
    {
        protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, AdultPolicyRequirement requirement)
        {
            //获取当前http请求的context对象
            var mvcContext = context.Resource as AuthorizationFilterContext;
            //以下代码都不是必须的，只是展示一些使用方法，你可以选择使用

            //设置httpcontext的一些key和value，用于在整个http请求过程中分享数据
            mvcContext.HttpContext.Items.Add("aaa", "bbb");

            //判断当前http请求的controller对象是否是某个特定对象的子类
            var descriptor = mvcContext?.ActionDescriptor as ControllerActionDescriptor;
            if (descriptor != null)
            {
                TypeInfo typeinfo = descriptor.ControllerTypeInfo;
                bool issub = typeinfo.IsSubclassOf(typeof(Controller));
                var actionName = descriptor.ActionName;
                var ctrlName = descriptor.ControllerName;
            }

            //token类型是bearer类型
            if (mvcContext.HttpContext.Request.Headers.ContainsKey("Authorization"))
            {
                var header = mvcContext.HttpContext.Request.Headers as FrameRequestHeaders;

                var value = header.HeaderAuthorization.First<string>(v => v.ToLower().StartsWith("bearer "));

                if (value == null)
                {
                    //dosomething
                }
                string _token = value.ToString().Substring(7);
            }
            //获取cookie
            var cookie = mvcContext.HttpContext.Request.Cookies["BAIDUD"];
            if (cookie == null)
            {
                //dosomething
            }

            //从http的url参数获取一个值
            var value1 = mvcContext.HttpContext.Request.Query.FirstOrDefault(u => u.Key == "accesstoken");
            if (value1.Value.Count <= 0)
            {
                //dosomething
            }

            //
            var age = mvcContext.HttpContext.Request.Query.FirstOrDefault(u => u.Key == "age");
            if (age.Value.Count <= 0|| Convert.ToInt16(age.Value[0]) < requirement.Age)
            { 
                context.Fail();
            }
            else
            {
                //通过验证，这句代码必须要有
                context.Succeed(requirement);
            }


            return Task.CompletedTask;
        }
    }
}
