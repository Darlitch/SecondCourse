import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class InputResolver {
    public static int Resolve(String ipAddr) throws UnknownHostException {
        int mode = -1;
        try {
            InetAddress addr = InetAddress.getByName(ipAddr);
            if (addr instanceof Inet4Address) {
               mode = 0;
            } else if (addr instanceof Inet6Address) {
                mode = 1;
            }
        } catch (UnknownHostException ex) {
            throw new UnknownHostException();
        }
        return mode;
    }

}
