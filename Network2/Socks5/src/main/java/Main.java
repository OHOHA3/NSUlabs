public class Main {
    public static void main(String[] args) {
        Socks5Proxy server = new Socks5Proxy("127.0.0.1",1080);
        server.run();
    }
}
