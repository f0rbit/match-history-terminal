#include <iostream>
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/stringbuffer.h"
#include <string.h>
using namespace rapidjson;
using namespace std;

int main() {
  const char* json = "{\"firstName\":\"Sean\",\"lastName\":\"Brown\",\"empId\":21453,\"department\":\"Computer Sc.\"}";

  // Parse the JSON string into DOM
  Document mydoc;
  mydoc.Parse(json);
  mydoc.AddMember("key","value",mydoc.GetAllocator());

  assert(mydoc["empId"].GetInt() == 21453);

  // DOM to string
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  mydoc.Accept(writer);

  // Print the output
  std::cout << buffer.GetString() << std::endl;


}