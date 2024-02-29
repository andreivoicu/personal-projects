# Notification Application

## Functionality

The application simulates a notification board used for various announcements. The application supports two types of users:

- Listeners: These users can log in with their ID and subscribe to different topics.
- Broadcasters: These users can send messages on different topics, and their messages will be delivered to the users subscribed to those topics.


## Overview

The application can be broken down into three main components:

- UDP Clients:
  - Send messages related to a topic to the server.
- TCP Clients:
  - Connect to the server using a unique ID.
  - Subscribe/unsubscribe to topics.
  - Receive messages related to the subscribed topics from the server and print them.
- The Server:
  - Accepts both UDP and TCP connections.
  - Receives topic messages via UDP and stores them internally.
  - Sends topic messages via TCP to subscribed users.

## Usage

### Server

To start the server, run
```bash
./server <PORT>
```

Displayed messages on the server

- New client <ID_CLIENT> connected from IP:PORT
- Client <ID_CLIENT> disconnected

### TCP Clients

To start a TCP client, run
```bash
./subscriber <ID_CLIENT> <IP_SERVER> <PORT_SERVER>
```

After starting the client, in order to subscribe to a topic, run
```bash
subscribe <TOPIC> <SAVE>
```
The value of \<SAVE\> will be either 0 or 1
Consider the following scenario: user Andrei connects, subscribes to the 'football' topic, and disconnects. While Andrei is disconnected, the server receives a message on the topic 'football'. If Andrei set the \<SAVE\> value to 1 when subscribing to the topic, when he reconnects, he will receive every message related to the 'football' topic that was sent while he was offline. If the \<SAVE\> value was set to 0, he will not receive the messages sent while he was disconnected.

Displayed messages on a TCP Client
- Subscribed to topic.
- Unsubscribed from topic.
- ```<IP_CLIENT_UDP>:<PORT_CLIENT_UDP> - <TOPIC> - <DATA_TYPE> - <MESSAGE_VALUE>```