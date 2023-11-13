#include <iostream>
#include <unistd.h>
#include <zmq.hpp>

int main() {
  zmq::context_t context(2);
  zmq::socket_t socket(context, zmq::socket_type::rep);
  socket.bind("tcp://*:5555");

  while (true) {
    zmq::message_t request;

    // Wait for next request from client.
    socket.recv(request, zmq::recv_flags::none);
    std::cout << "Received Hello" << std::endl;

    // Do some work.
    sleep(1);

    zmq::message_t reply(5);
    memcpy(reply.data(), "World", 5);
    socket.send(reply, zmq::send_flags::none);
  }

  return 0;
}
