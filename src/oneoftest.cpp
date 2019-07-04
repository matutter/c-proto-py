#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <fstream>

#include "pb.h"
#include "pb_decode.h"
#include "oneof.pb.h"

#define CAST_ARG(_arg) ({ \
  __typeof(_arg) __x = (_arg); \
  void** __vx = reinterpret_cast<void**>(&__x); \
  __vx; })

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;

typedef vector<uint8_t> bytes_t;

bool decode_bytes(pb_istream_s* stream, const pb_field_t* field, void** arg) {
  bytes_t* bytes = reinterpret_cast<bytes_t*>(*arg);
  bytes->clear();
  bool decode_ok = true;
  while( decode_ok && stream->bytes_left > 0 ) {
    uint8_t abyte = 0;
    decode_ok = pb_read(stream, &abyte, sizeof(uint8_t));
    if ( decode_ok ) {
      bytes->push_back(abyte);
    }
  }
  return decode_ok;
}

bool decode_string(pb_istream_s* stream, const pb_field_t* field, void** arg) {
  vector<uint8_t> bytes;
  bool decode_ok = decode_bytes(stream, field, CAST_ARG(&bytes));
  if ( decode_ok ) {
    string* s = reinterpret_cast<string*>(*arg);
    s->replace(s->begin(), s->end(), bytes.begin(), bytes.end());
    printf("s=%s, ptr=%p\n", s->c_str(), s);
  }
  return decode_ok;
}

int main(void) {
  string filepath = "data/message1.pb";
  cout << "oneoftest: " << filepath << endl;

  // Read file into vector and init pb_istream_t
  ifstream ifstream(filepath, std::ios::in | std::ios::binary);
  vector<uint8_t> buffer((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
  pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());

  string top_sfield;
  string sub1_text;

  TopMsg msg = TopMsg_init_zero;
  msg.sfield.arg = &top_sfield;
  msg.sfield.funcs.decode = decode_string;
  msg.payload.sub1.text.arg = &sub1_text;
  msg.payload.sub1.text.funcs.decode = decode_string;
  cout << "which_payload: " << (int)msg.which_payload << endl;

  msg.which_payload = TopMsg_sub1_tag;
  printf("top sfield=%p, sub1 test=%p\n", msg.sfield.arg,msg.payload.sub1.text.arg);
  bool decode_ok = pb_decode(&stream, TopMsg_fields, &msg);
  if ( decode_ok ) {
    cout << "OK: top.sfield=" << top_sfield << ", sub1.text=" << sub1_text << endl;

    if ( msg.type == MSG_TYPE_MSG_TYPE_1 ) {
      cout << "Expecting: MSG_TYPE_MSG_TYPE_1 " << (int)msg.which_payload << endl;
    } else if( msg.type == MSG_TYPE_MSG_TYPE_2 ) {
      cout << "Expecting: MSG_TYPE_MSG_TYPE_2 " << (int)msg.which_payload << endl;
    } else {
      cout << "Expecting: ??? " << (int)msg.type << endl;
    }

    // which payload is set to 40 now - so can we have it
    // decode only SubMsg1_fields? ... nope.
    stream = pb_istream_from_buffer(buffer.data(), buffer.size());
    decode_ok = pb_decode(&stream, SubMsg1_fields, &msg);
    if (decode_ok) {
      cout << "OK: top.sfield=" << top_sfield << ", sub1.text=" << sub1_text << endl;
    }
  } else {
    cout << "ERROR: " << PB_GET_ERROR(&stream) << endl;
  }

  return 0;
};