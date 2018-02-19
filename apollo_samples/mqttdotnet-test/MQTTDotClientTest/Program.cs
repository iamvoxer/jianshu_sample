using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Packets;
using MQTTnet.Protocol;
using System;
using System.Text;
using System.Threading.Tasks;

namespace MQTTDotClientTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            // Create a new MQTT client.
            var factory = new MqttFactory();
            var mqttClient = factory.CreateMqttClient();

            // Create TCP based options using the builder.
            var options = new MqttClientOptionsBuilder()
                .WithClientId("dotnetclient")
                .WithTcpServer("你的Apollo所在服务器ip", 61613)
                .WithCredentials("admin", "password")
                .Build();
            //连接断了后重新连
            mqttClient.Disconnected += async (s, e) =>
            {
                Console.WriteLine("### DISCONNECTED FROM SERVER ###");
                await Task.Delay(TimeSpan.FromSeconds(5));

                try
                {
                    await mqttClient.ConnectAsync(options);
                }
                catch
                {
                    Console.WriteLine("### RECONNECTING FAILED ###");
                }
            };
            mqttClient.ApplicationMessageReceived += (s, e) =>
            {
                //接受到消息并处理
                Console.WriteLine("### RECEIVED APPLICATION MESSAGE ###");
                Console.WriteLine($"+ Topic = {e.ApplicationMessage.Topic}");
                Console.WriteLine($"+ Payload = {Encoding.UTF8.GetString(e.ApplicationMessage.Payload)}");
                Console.WriteLine($"+ QoS = {e.ApplicationMessage.QualityOfServiceLevel}");
                Console.WriteLine($"+ Retain = {e.ApplicationMessage.Retain}");
                Console.WriteLine();
            };
            mqttClient.Connected += async (s, e) =>
            {
                Console.WriteLine("### CONNECTED WITH SERVER ###");
                //连接成功后马上订阅消息,主题是topic3
                await mqttClient.SubscribeAsync(new TopicFilterBuilder().WithTopic("topic3").Build());
                Console.WriteLine("### SUBSCRIBED ###");
                //并马上publish一个消息，主题是topic1
                var message = new MqttApplicationMessageBuilder()
                .WithTopic("topic1")
                .WithPayload("Message from dotnet client")
                .WithAtMostOnceQoS()
                .Build();
                await mqttClient.PublishAsync(message);
            };
            //建立连接
            mqttClient.ConnectAsync(options);
            Console.ReadLine();
        }
    }
}
