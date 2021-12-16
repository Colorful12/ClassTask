**サーバー側のコンパイル
gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c CreateTCPServerSocket.c AcceptTCPConnection.c  -o TCPEchoServer

**クライアント側のコンパイル
gcc TCPEchoClient.c DieWithError.c popup.c -o TCPEchoClient

**サーバー側の実行例
./TCPEchoServer 5000

**クライアント側の実行例
./TCPEchoClient 192.168.11.15  5000
./TCPEchoClient 127.0.0.1  5000