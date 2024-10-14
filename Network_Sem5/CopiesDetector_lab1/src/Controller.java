import java.io.IOException;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Controller {
    private Datagram datagram;
    public Controller(String[] args) throws IOException {
        datagram = new Datagram();
        setupDatagram(datagram, args, InputResolver.resolve(args));
    }

    private void setupDatagram(Datagram datagram, String[] args, String ipVer) throws IOException {
        datagram.addr = InetAddress.getByName(args[0]);
        datagram.ipVer = ipVer;
        datagram.port = Integer.parseInt(args[1]);
        datagram.msg = "Hello".getBytes();
        datagram.socket = new MulticastSocket(datagram.port);
    }
    public void start() throws IOException {
        datagram.socket.joinGroup(datagram.addr);
    }

}
