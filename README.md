# Messaging App
It is a console-based online messaging app for Windows systems made using C++ and WinSock2 library. For now it supports messaging over local network. It is _possible_ to send messages to a device outside of local network, but it requires setting up port forwarding and depends on both networks' infrastructure, so this topic *won't be covered* in this README file.

## Usage
After launching the app you can choose if you want to host a chat room or join an existing one. To make the choice type `host` or `join` in the console window.

After choosing `host` you will need to wait for a connection request.
After choosing `join` you'll be asked to enter an IPv4 address of the host you want to connect with. If the host is active it'll automatically open a new chat room.

## Sending messages
To send a message type in what you want to send in the console window and press ENTER. The message will be transfered to the computer you're currently connected to and display at the top of the window.
