# Messaging App
It is a console-based online messaging app for Windows systems made using C++ and WinSock2 library. For now it supports messaging over local network. It is _possible_ to send messages to a device outside of local network, but it requires setting up port forwarding and depends on both networks' infrastructure, so this topic *won't be covered* in this README file.

## Technicalities
The application uses the WinSock2 library, making it only compatible with Windows.
A created chat room can connect two users, where one is a host, taking the role of the "server" and the other is a "client". The connection is realised on the local network through TCP/IP and IPv4 addresses.

## Usage
After launching the app you can choose if you want to host a chat room or join an existing one. To make the choice type `host` or `join` in the console window.

After choosing `host` you will need to wait for a connection request.
After choosing `join` you'll be asked to enter a private IPv4 address of the host you want to connect with. If the host is active it'll automatically open a new chat room.

## Sending messages
To send a message type in what you want to send in the console window and press ENTER. The message will be transferred to the computer you're currently connected to and displayed at the top of the window.

## Exiting the program
You can quit at any moment by typing in `\q`. If you're the person quitting, you will be disconnected. If the person you're talking with quits you will get notified.
