import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * @author buter
 */
public class EventBus {

    private volatile static EventBus instance;
    private final HashMap<String, List<IEvent>> dict = new HashMap<>();

    private EventBus() {

    }

    public static EventBus getInstance() {
        if (instance == null) {
            synchronized (EventBus.class) {
                if (instance == null) {
                    instance = new EventBus();
                }
            }
        }
        return instance;
    }

    public void on(String message, IEvent userListener) {
        synchronized (EventBus.class) {
            if (!dict.containsKey(message)) {
                dict.put(message, new ArrayList<>());
            }
            List<IEvent> listeners = dict.get(message);
            listeners.add(userListener);
        }
    }

    public void fire(String message, Object data) {
        if (dict.containsKey(message)) {
            List<IEvent> listeners = dict.get(message);
            listeners.forEach(listener -> listener.invoke(message, data));
        }
    }

}
