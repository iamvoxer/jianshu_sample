import com.google.common.eventbus.AsyncEventBus;
import com.google.common.eventbus.Subscribe;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * 测试自定义的eventbus和guava 的eventbus
 *
 * @author buter
 */
public class Test {
    public static void main(String[] args) {
        new Test1().fireEvent();
    }
}

class Test1 {
    public void fireEvent() {
        //1.自己实现的eventbus使用方式
        Test2 test2 = new Test2();
        Test3 test3 = new Test3();
        EventBus.getInstance().on("Event1", test2);

        EventBus.getInstance().fire("Event1", "data1");

        //2.google guava库的eventbus使用方式
        //2.1同步消息中心
        com.google.common.eventbus.EventBus eventBus = new com.google.common.eventbus.EventBus();
        Test4 test4 = new Test4();
        eventBus.register(test4);
        eventBus.post("Event2");
        eventBus.post(new EventObject("Event3", "Event3Name"));
        //2.2异步消息中心
        ThreadPoolExecutor executor = new ThreadPoolExecutor(10, 20, 5, TimeUnit.SECONDS, new ArrayBlockingQueue<>(10), new ThreadPoolExecutor.AbortPolicy());
        AsyncEventBus asyncEventBus = new AsyncEventBus(executor);
        asyncEventBus.register(new Test4());
        asyncEventBus.post("Event4");
        asyncEventBus.post(new EventObject("Event5", "Event5Name"));
    }
}

class Test2 implements IEvent {
    @Override
    public void invoke(String message, Object data) {
        System.out.println(Thread.currentThread() + " test2接收到消息，主题是" + message + ",内容是" + data);
        try {
            //做一些耗时的距离
            Thread.sleep(1000);
        } catch (InterruptedException exception) {
            exception.printStackTrace();
        }
    }
}

class Test3 {
    public Test3() {
        EventBus.getInstance().on("Event1", new IEvent() {
            @Override
            public void invoke(String message, Object data) {
                System.out.println(Thread.currentThread() + " test3接收到消息，主题是" + message + ",内容是" + data);
                try {
                    //做一些耗时的距离
                    Thread.sleep(5000);
                } catch (InterruptedException exception) {
                    exception.printStackTrace();
                }
            }
        });
    }
}

class EventObject {
    private String id;
    private String name;

    public EventObject(String id, String name) {
        this.id = id;
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "EventObject{" +
                "id='" + id + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}

class Test4 {
    @Subscribe
    public void invoke(String event) {
        //函数名随意，参数只能一个
        System.out.println(Thread.currentThread() + " test4接收到guava实现的string消息，内容是" + event);
        try {
            //做一些耗时的距离
            Thread.sleep(1000);
        } catch (InterruptedException exception) {
            exception.printStackTrace();
        }
    }

    @Subscribe
    public void invoke(EventObject event) {
        //函数名随意，参数只能一个
        System.out.println(Thread.currentThread() + " test4接收到guava实现的object消息，内容是" + event.toString());
        try {
            //做一些耗时的距离
            Thread.sleep(1000);
        } catch (InterruptedException exception) {
            exception.printStackTrace();
        }
    }
}