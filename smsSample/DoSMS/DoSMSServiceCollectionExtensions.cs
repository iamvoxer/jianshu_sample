using D1.Framework.DoSMS;
using System;



namespace Microsoft.Extensions.DependencyInjection
{
    public static class DoSMSServiceCollectionExtensions
    {
        public static IServiceCollection AddUpassSMS(this IServiceCollection services,Action<SMSUpassOptions> options=null)
        {
            if (options!=null)
                services.Configure<SMSUpassOptions>(options);
            services.AddSingleton<IDoSMS, DoSMSUpass>();
            return services;
        }
    }
}
