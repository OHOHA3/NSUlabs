import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.xbill.DNS.Name;
import org.xbill.DNS.Type;
import org.xbill.DNS.lookup.LookupSession;

import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.channels.spi.SelectorProvider;
import java.util.Iterator;

@Slf4j
@RequiredArgsConstructor
public class Socks5Proxy implements Runnable {
    private final byte NO_METHODS = 0x00;
    private final byte SUCCESS = 0x00;
    private final byte RESERVED = 0x00;
    private final byte BY_IPV4 = 0x01;
    private final byte CONNECT = 0x01;
    private final byte BY_DOMAIN = 0x03;
    private final byte V5 = 0x05;
    private final int bufferSize = 8192;
    private final String host;
    private final int port;
    LookupSession lookupSession;

    private static class Attachment {
        ByteBuffer in;
        ByteBuffer out;
        SelectionKey peer;
        boolean readHeader = false;
    }

    @Override
    public void run() {
        try {
            log.info("Server started");
            lookupSession = LookupSession.defaultBuilder().build();
            Selector selector = SelectorProvider.provider().openSelector();
            ServerSocketChannel serverChannel = ServerSocketChannel.open();
            serverChannel.configureBlocking(false);
            serverChannel.socket().bind(new InetSocketAddress(host, port));
            serverChannel.register(selector, serverChannel.validOps());

            while (selector.select() > -1) {
                Iterator<SelectionKey> iterator = selector.selectedKeys().iterator();
                while (iterator.hasNext()) {
                    SelectionKey key = iterator.next();
                    iterator.remove();
                    if (key.isValid()) {
                        try {
                            if (key.isAcceptable()) {
                                accept(key);
                            } else if (key.isConnectable()) {
                                connect(key);
                            } else if (key.isReadable()) {
                                read(key);
                            } else if (key.isWritable()) {
                                write(key);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                            close(key);
                        }
                    }
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
            throw new IllegalStateException(e);
        }
    }

    private void accept(SelectionKey key) throws IOException {
        SocketChannel newChannel = ((ServerSocketChannel) key.channel()).accept();
        newChannel.configureBlocking(false);
        newChannel.register(key.selector(), SelectionKey.OP_READ);
    }

    private void read(SelectionKey key) throws IOException {
        SocketChannel channel = (SocketChannel) key.channel();
        Attachment attachment = (Attachment) key.attachment();
        if (attachment == null) {
            key.attach(attachment = new Attachment());
            attachment.in = ByteBuffer.allocate(bufferSize);
        }
        if (channel.read(attachment.in) < 1) {
            close(key);
        } else if (attachment.peer == null) {
            readHeader(key);
        } else {
            attachment.peer.interestOps(attachment.peer.interestOps() | SelectionKey.OP_WRITE);
            key.interestOps(key.interestOps() ^ SelectionKey.OP_READ);
            attachment.in.flip();
        }
    }

    private void readHeader(SelectionKey key) throws IOException {
        Attachment attachment = (Attachment) key.attachment();
        byte[] header = attachment.in.array();
        if (header[0] != V5) {
            log.error("Invalid version of protocol");
            close(key);
            return;
        }
        if (header[2] != NO_METHODS) {
            log.error("Another commands are not supported");
            close(key);
            return;
        }

        if (!attachment.readHeader) {
            attachment.out = ByteBuffer.wrap(new byte[]{header[0], header[2]});
            key.interestOps(key.interestOps() ^ SelectionKey.OP_READ | SelectionKey.OP_WRITE);
            attachment.readHeader = true;
        } else {
            SocketChannel channelForSite = SocketChannel.open();
            channelForSite.configureBlocking(false);

            switch (header[3]) {
                case BY_IPV4 -> {
                    byte[] address = new byte[]{header[4], header[5], header[6], header[7]};
                    int portForSite = (((0xFF & header[8]) << 8) + (0xFF & header[9]));
                    channelForSite.connect(new InetSocketAddress(InetAddress.getByAddress(address), portForSite));
                    
                    StringBuilder addrString = new StringBuilder();
                    for (byte addr : address) {
                        addrString.append(addr < 0 ? 256 + addr : addr);
                        addrString.append(".");
                    }
                    addrString.deleteCharAt(addrString.length() - 1);
                    log.info("connection to " + addrString + ":" + portForSite);
                }
                case BY_DOMAIN -> {
                    String domain = new String(header, 5, header[4]);
                    int portForSite = (((0xFF & header[5 + header[4]]) << 8) + (0xFF & header[5 + header[4] + 1]));
                    Name domainName = Name.fromString(domain);
                    lookupSession.lookupAsync(domainName, Type.A).toCompletableFuture()
                            .thenAccept(rec -> {
                                byte[] address = rec.getRecords().get(0).rdataToWireCanonical();
                                try {
                                    channelForSite.connect(new InetSocketAddress(InetAddress.getByAddress(address), portForSite));
                                } catch (IOException e) {
                                    throw new RuntimeException(e);
                                }
                            });
                    log.info("connection to " + domain + ":" + portForSite);
                }
                default -> {
                    log.error("Another type of following address are not supported");
                    close(key);
                    return;
                }
            }

            SelectionKey peerKey = channelForSite.register(key.selector(), SelectionKey.OP_CONNECT);
            key.interestOps(0);
            attachment.peer = peerKey;
            Attachment peerAttachment = new Attachment();
            peerAttachment.peer = key;
            peerKey.attach(peerAttachment);
            attachment.readHeader = false;
        }
        attachment.in.clear();
    }

    private void write(SelectionKey key) throws IOException {
        SocketChannel channel = (SocketChannel) key.channel();
        Attachment attachment = (Attachment) key.attachment();

        if (channel.write(attachment.out) == -1) {
            close(key);
        } else if (attachment.out.remaining() == 0) {
            if (attachment.peer == null) {
                if (attachment.readHeader) {
                    attachment.out.clear();
                    key.interestOps(key.interestOps() ^ SelectionKey.OP_WRITE | SelectionKey.OP_READ);
                } else {
                    close(key);
                }
            } else {
                attachment.out.clear();
                attachment.peer.interestOps(attachment.peer.interestOps() | SelectionKey.OP_READ);
                key.interestOps(key.interestOps() ^ SelectionKey.OP_WRITE);
            }
        }
    }

    private void connect(SelectionKey key) throws IOException {
        SocketChannel channel = (SocketChannel) key.channel();
        Attachment attachment = (Attachment) key.attachment();
        channel.finishConnect();

        byte[] addr = ((InetSocketAddress) channel.getLocalAddress()).getAddress().getAddress();
        int prt = ((InetSocketAddress) channel.getLocalAddress()).getPort();
        byte[] bytePrt = new byte[]{(byte) (prt >> 8), (byte) (0XFF & prt)};
        byte[] header = new byte[]{V5, SUCCESS, RESERVED, CONNECT, addr[0], addr[1], addr[2], addr[3], bytePrt[0], bytePrt[1]};

        attachment.in = ByteBuffer.allocate(bufferSize);
        attachment.in.put(header).flip();
        attachment.out = ((Attachment) attachment.peer.attachment()).in;
        ((Attachment) attachment.peer.attachment()).out = attachment.in;
        attachment.peer.interestOps(SelectionKey.OP_WRITE | SelectionKey.OP_READ);
        key.interestOps(0);
    }

    private void close(SelectionKey key) throws IOException {
        log.info("Connection closed");
        key.cancel();
        key.channel().close();
        SelectionKey peerKey = ((Attachment) key.attachment()).peer;
        if (peerKey != null) {
            ((Attachment) peerKey.attachment()).peer = null;
            if ((peerKey.interestOps() & SelectionKey.OP_WRITE) == 0) {
                ((Attachment) peerKey.attachment()).out.flip();
            }
            peerKey.interestOps(SelectionKey.OP_WRITE);
        }
    }
}