﻿using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ClientTCP
{
    class Program
    {
        static void Main(string[] args)
        {           
            const string ip = "127.0.0.1";
            const int port = 8080;

            var tcpEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);
            var tcpSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
           
            Console.WriteLine("Enter message: ");
            var message = Console.ReadLine();
            
            var data = Encoding.UTF8.GetBytes(message);
            tcpSocket.Connect(tcpEndPoint);
            tcpSocket.Send(data);

            var buffer = new byte[1024];
            var answer = new StringBuilder();

            while (true)
            {
                var size = tcpSocket.Receive(buffer);
                if (size == 0)
                {
                    break;
                }

                answer.Append(Encoding.UTF8.GetString(buffer, 0, size));
            }

            Console.WriteLine(answer.ToString());
            tcpSocket.Shutdown(SocketShutdown.Both);
            tcpSocket.Close();
        }
    }
}
