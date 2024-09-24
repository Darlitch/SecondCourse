import java.net.UnknownHostException;

public class Controller {
    private final String ipAddr;
    public Controller(String ipAddr) {
        this.ipAddr = ipAddr;
    }

    public void Start() throws UnknownHostException {
        System.out.println(InputResolver.Resolve(ipAddr));
    }
}
