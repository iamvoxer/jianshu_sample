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

namespace WebApplication1
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
            services.AddSingleton<Interface1, Class1>();
            services.AddTransient<Interface2, Class2>();
            services.AddScoped<Interface3, Class3>();

            var provider = services.BuildServiceProvider();
            var class1 = provider.GetService<Interface1>();
            Console.WriteLine(class1.function1());
            TestScope(provider);
            TestScope(provider);
        }
        private void TestScope(IServiceProvider provider)
        {
            using (var scope1 = provider.CreateScope())
            {
                var p = scope1.ServiceProvider;

                var scopeobj1 = p.GetService<Interface3>();
                var transient1 = p.GetService<Interface2>();
                var singleton1 = p.GetService<Interface1>();

                var scopeobj2 = p.GetService<Interface3>();
                var transient2 = p.GetService<Interface2>();
                var singleton2 = p.GetService<Interface1>();

                Console.WriteLine(
                    $"scope1: { scopeobj1.function3() }," +
                    $"transient1: {transient1.function2()}, " +
                    $"singleton1: {singleton1.function1()}");

                Console.WriteLine($"scope2: { scopeobj2.function3() }, " +
                    $"transient2: {transient2.function2()}, " +
                    $"singleton2: {singleton2.function1()}");
            }
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
