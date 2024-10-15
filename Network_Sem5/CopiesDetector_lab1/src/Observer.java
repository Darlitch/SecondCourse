import java.util.Hashtable;
import java.util.Map;

public class Observer {
    private Map<String, Long> liveCopies;
    private int timeout = 5000;

    public Observer() {
        liveCopies = new Hashtable<String, Long>();
    }

    // ДОДЕЛАТЬ ФЛАГИ
    public void updateLiveCopies(Datagram datagram) {
        String key = datagram.addr.toString() + ":" + Integer.toString(datagram.port);
        liveCopies.put(key, System.currentTimeMillis());
    }

    public void removeDeadCopies() {
        long currTime = System.currentTimeMillis();
        for (String key : liveCopies.keySet()) {
            if (currTime - liveCopies.get(key) > timeout) {
                liveCopies.remove(key);
            }
        }
    }

    public void printLiveCopies() {
        if (liveCopies.isEmpty()) {
            System.out.println("No live copies");
            return;
        }
        for (String key : liveCopies.keySet()) {
            System.out.println(key);
        }
    }
}
