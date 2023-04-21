#include "benchmark/Benchmark.h"
#include "codec/PNGDecoder.h"
#include "Server.h"
#include "fileUtil.h"
#include "Client.h"
#include <iostream>

using namespace suite;

void runTest(std::string dir);

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Syntax: %s <directory>\n", argv[0]);
    exit(1);
  }

  std::string dir = argv[1];

  try {
    runTest(dir);
  } catch (rdr::Exception &e) {
    std::cerr << e.str() << std::endl;
  }
}

void runTest(std::string dir)
{

  Benchmark* b = new Benchmark(dir, new PNGDecoder());
  std::vector<std::string> files = readDirectory(dir);
  if(!files.size())
    throw rdr::Exception("no files in directory");

  int width = 1920;
  int height = 1200;
  Server *server = new Server(width,height);
  b->runBenchmark(server);

  std::cout << "Benchmark " << dir << " took "  
    << b->getTime() << " seconds.\n";

  #if _DEBUG
  // // Copy sc OutStream to cc InStream
  Client *client = new Client(width,height);
  client->copyOutStreamToInStream(server->out);

  // Decode input & dump to file
    try {
      while (true) {
        client->processMsg();
      }
    } catch (rdr::EndOfStream &e) {
    } catch (rdr::Exception &e) {
      fprintf(stderr, "Failed to run rfb file: %s\n", e.str());
      exit(1);
    }
    delete client;
  #endif

  delete server;
  delete b;
}
