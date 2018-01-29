#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

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

  if ( argc != 3 ) {
    cerr << "Usage: " << argv[ 0 ] << " HOST PORT" << endl;
    return EXIT_FAILURE;
  }

  string host { argv[ 1 ] }, port { argv[ 2 ] };

  /* Look up the server's address */
  cerr << "Looking up " << host << ":" << port << endl;
  Address server( host, port );
  cerr << "Done. Found " << server.to_string() << endl;

  /* create a TCP socket */
  UDPSocket socket;

  /* connect to the server */
  cerr << "Connecting...";
  socket.connect( server );
  cerr << "done." << endl;

  //  const auto interval = chrono::microseconds( 10 );
  
  string packet_contents = "x";

  iovec io_vec { packet_contents.data(), packet_contents.length() };
  vector<mmsghdr> io_references;
  for ( unsigned int i = 0; i < 1000; i++ ) {
    msghdr packet;
    packet.msg_iov = &io_vec;
    packet.msg_iovlen = 1;
    io_references.push_back( { packet, 0 } );
  }

  while ( true ) {
    SystemCall( "sendmmsg",
		sendmmsg( socket.fd_num(),
			  &io_references.at( 0 ),
			  io_references.size(),
			  0 ) );
    /*
    auto next_outgoing_datagram = chrono::steady_clock::now() + interval;  
    
    this_thread::sleep_until( next_outgoing_datagram );
    */
  }
}
