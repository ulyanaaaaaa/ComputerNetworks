using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ServerTCP
{
    class Program
    {
        static void Main(string[] args)
        {
            const string ip = "127.0.0.1";
            const int port = 8080;

            var tcpEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            var tcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            tcpSocket.Bind(tcpEndPoint);
            tcpSocket.Listen(5);

            while (true)
            {
                try
                {
                    var listener = tcpSocket.Accept();
                    var buffer = new byte[256];
                    var size = 0;
                    var data = new StringBuilder();

                    do
                    {
                        size = listener.Receive(buffer);
                        data.Append(Encoding.UTF8.GetString(buffer, 0, size));
                    }
                    while (listener.Available > 0);

                    Console.WriteLine($"Received from client: {data}");
                    string reversedData = ReverseString(data.ToString());
                    Console.WriteLine($"Sent to client: {reversedData}");

                    listener.Send(Encoding.UTF8.GetBytes(reversedData));

                    listener.Shutdown(SocketShutdown.Both);
                    listener.Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        private static string ReverseString(string input)
        {
            char[] charArray = input.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
        } 
    }
}

