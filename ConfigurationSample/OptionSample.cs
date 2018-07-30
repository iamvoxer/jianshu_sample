using Microsoft.Extensions.Options;

namespace ConfigurationSample
{
    public class OptionSample : IOptions<OptionSample>
    {
        public OptionSample Value => this;
        public string key1{get;set;}
        public string key2{get;set;}
        public int key3{get;set;}
        public bool key4{get;set;}
        
    }
}