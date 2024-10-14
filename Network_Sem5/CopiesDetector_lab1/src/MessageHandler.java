import java.io.IOException;
import java.net.DatagramPacket;

public class MessageHandler {
    public static String handleMessage(Datagram datagram) {

    }

    public static void SendMessage(Datagram datagram) throws IOException {
        DatagramPacket packet = new DatagramPacket(datagram.msg, datagram.msg.length, datagram.addr, datagram.port);
        datagram.socket.send(packet);
    }
}
