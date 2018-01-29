/* Keith Winstein <keithw@cs.stanford.edu>, January 2015 */

#include <thread>
#include <iostream>
#include <chrono>

#include "socket.hh"
#include "util.hh"

#include <sys/socket.h>

using namespace std;

int main( int argc, char *argv[] )
{
  /* check the command-line arguments */
  if ( argc < 1 ) { /* for sticklers */
    abort();
  }

  if ( argc != 2 ) {
    cerr << "Usage: " << argv[ 0 ] << " PORT" << endl;
    return EXIT_FAILURE;
  }

  /* create a TCP socket */
  UDPSocket listening_socket;

  /* "bind" the socket to the user-specified local port number */
  listening_socket.bind( Address( "::0", argv[ 1 ] ) );

  cerr << "Listening on local address: " << listening_socket.local_address().to_string() << endl;

  struct incoming_packet
  {
    char payload[ 2000 ];
    iovec io_vec {};

    incoming_packet() {
      io_vec.iov_base = payload;
      io_vec.iov_len = 2000;
    }
  };

  vector<incoming_packet> incoming_packets( 1000000 );
  vector<mmsghdr> incoming_packet_headers;
  for ( unsigned int i = 0; i < incoming_packets.size(); i++ ) {
    mmsghdr header {};
    header.msg_hdr.msg_iov = &incoming_packets.at( i ).io_vec;
    header.msg_hdr.msg_iovlen = 1;
    incoming_packet_headers.push_back( header );
  }
  
  while ( true ) {
    timespec timeout { 10, 0 };

    int num_received = SystemCall( "recvmmsg",
				   recvmmsg( listening_socket.fd_num(),
					     incoming_packet_headers.data(),
					     incoming_packet_headers.size(),
					     0,
					     &timeout ) );
    cerr << "count: " << num_received << "\n";
  }

  return EXIT_SUCCESS;
}
