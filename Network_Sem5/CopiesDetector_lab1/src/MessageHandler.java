import javax.xml.crypto.Data;
import java.io.IOException;
import java.net.DatagramPacket;

public class MessageHandler {
    public static Datagram handleMessage(Datagram datagram) throws IOException {
        Datagram recivedDatagram = new Datagram();
        byte[] data = new byte[1024];
        DatagramPacket packet = new DatagramPacket(data, data.length);
        datagram.socket.receive(packet);
        recivedDatagram.addr = packet.getAddress();
        recivedDatagram.port = packet.getPort();
//        System.out.println(recivedDatagram.port);
        recivedDatagram.msg = packet.getData();
        return recivedDatagram;
    }

    public static void SendMessage(Datagram datagram) throws IOException {
//        datagram.socket.joinGroup(datagram.addr);
        DatagramPacket packet = new DatagramPacket(datagram.msg, datagram.msg.length, datagram.addr, datagram.port);
        datagram.socket.send(packet);
//        datagram.socket.leaveGroup(datagram.addr);
    }
}
