package d1.project.osagent;

import d1.framework.mqttclient.MQTTClient;
import d1.framework.mqttclient.MQTTClientListener;
import d1.framework.mqttclient.Message;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Service;

/**
 * @author liuyi
 */
@Service
public class MQTTService {
    private String ip = "broker的地址";
    private int port = 1883;
    private String username = "user1";
    private String password = "password1";
    private String clientId = "client1";
    private MQTTClient mqttClient;

    public MQTTService() {
        mqttClient = new MQTTClient(new MQTTClientListener() {
            @Override
            public void connected() {
                System.out.println("connected === ");
            }

            @Override
            public void disconnected() {
                System.out.println("disconnected === ");
            }

            @Override
            public void received(Message msg) {
                System.out.println("received === " + msg.getMsg() + " = " + msg.getTopic());
            }
        });
        try {
            mqttClient.connect(clientId, ip, port, username, password);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void publish(String topic, String msg) throws MqttException {
        if (mqttClient.isConnected()) {
            mqttClient.publish(topic, msg);
        }
    }
}
