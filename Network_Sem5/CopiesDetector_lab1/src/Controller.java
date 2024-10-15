import java.io.IOException;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Controller {
    private Datagram datagram;
    private Observer observer;
    public Controller(String[] args) throws IOException {
        datagram = new Datagram();
        setupDatagram(datagram, args, InputResolver.resolve(args));
        observer = new Observer();
    }

    private void setupDatagram(Datagram datagram, String[] args, String ipVer) throws IOException {
        datagram.addr = InetAddress.getByName(args[0]);
        datagram.ipVer = ipVer;
        datagram.port = Integer.parseInt(args[1]);
        datagram.msg = "Hello".getBytes();
        datagram.socket = new MulticastSocket(datagram.port);
    }
    public void start() throws IOException, InterruptedException {
        datagram.socket.joinGroup(datagram.addr);

        Sender sender = new Sender();
        Listener listener = new Listener();
        Updater updater = new Updater();

        Thread senderThread = new Thread(sender);
        Thread listenerThread = new Thread(listener);
        Thread updaterThread = new Thread(updater);

        senderThread.start();
        listenerThread.start();
        updaterThread.start();

        senderThread.join();
        listenerThread.join();
        updaterThread.join();

        datagram.socket.leaveGroup(datagram.addr);
        datagram.socket.close();
    }

    class Sender implements Runnable {
        @Override
        public void run() {
            while(true) {
                try {
                    MessageHandler.SendMessage(datagram);
                    Thread.sleep(1000);
                } catch (IOException | InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

    class Listener implements Runnable {
        @Override
        public void run() {
            while(true) {
                try {
                    Datagram recivedDatagram = MessageHandler.handleMessage(datagram);
//                    System.out.println("111111111");
                    observer.updateLiveCopies(recivedDatagram);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

    class Updater implements Runnable {
        @Override
        public void run() {
            while(true) {
                observer.removeDeadCopies();
                observer.printLiveCopies();
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

}
