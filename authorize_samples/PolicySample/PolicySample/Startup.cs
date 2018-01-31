using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using Microsoft.AspNetCore.Authorization;

namespace PolicySample
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddMvc();
            //添加二种认证策略，一种以12岁为界限，一种是18岁
            services.AddAuthorization(options =>
            {
                options.AddPolicy("Adult1", policy =>
                    policy.Requirements.Add(new AdultPolicyRequirement(12)));
                options.AddPolicy("Adult2", policy =>
                    policy.Requirements.Add(new AdultPolicyRequirement(18)));
            });
            //添加策略验证handler
            services.AddSingleton<IAuthorizationHandler, AdultAuthorizationHandler>();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseMvc();
        }
    }
}
