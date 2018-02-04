using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc.Controllers;
using Microsoft.AspNetCore.Mvc.Filters;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Threading.Tasks;

namespace CustomAttributeSample
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = true)]
    public class PermissionCheckAttribute : AuthorizeAttribute
    {
        public string Id { get; set; }
        public int Operation { get; set; }
        public PermissionCheckAttribute() : base("PermissionCheck")
        {

        }
    }

    public class PermissionCheckPolicyRequirement : IAuthorizationRequirement
    {
        //Add any custom requirement properties if you have them
        public PermissionCheckPolicyRequirement()
        {

        }
    }
    public class PermissionCheckPolicyHandler : AttributeAuthorizationHandler<PermissionCheckPolicyRequirement, PermissionCheckAttribute>
    {
        protected override Task HandleRequirementAsync(AuthorizationHandlerContext authoriazationContext,
            PermissionCheckPolicyRequirement requirement, IEnumerable<PermissionCheckAttribute> attributes)
        {
            var context = authoriazationContext.Resource as AuthorizationFilterContext;
            foreach (var permissionAttribute in attributes)
            {
                this.checkPermission(context, permissionAttribute.Id, permissionAttribute.Operation);
            }
            authoriazationContext.Succeed(requirement);
            return Task.FromResult<object>(null);
        }

        private void checkPermission(AuthorizationFilterContext context, string _Id, int _Operation)
        {
            if (_Operation > 0)
            {
                if (_Id != "user1")
                {
                    throw new Exception("不具备操作权限");
                }
            }
            else
            {
                //dosomething
            }
            return;
        }
    }
}