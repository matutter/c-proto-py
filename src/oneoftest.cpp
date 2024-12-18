#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <fstream>

#include "pb.h"
#include "pb_common.h"
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

const pb_msgdesc_t* decode_unionmessage_type(pb_istream_t *parent_stream) {
  pb_wire_type_t wire_type;
  uint32_t tag;
  bool eof;

  pb_istream_t stream = PB_ISTREAM_EMPTY;
  if ( !pb_make_string_substream(parent_stream, &stream) ) {
    cout << "Failed to make substream" << endl;
    return nullptr;
  }

  while (pb_decode_tag(&stream, &wire_type, &tag, &eof)) {
    if (wire_type == PB_WT_STRING) {
      pb_field_iter_t iter;
      if (pb_field_iter_begin(&iter, TopMsg_fields, NULL) &&
          pb_field_iter_find(&iter, tag)) {
          return iter.submsg_desc;
      }
    }
    
    pb_skip_field(&stream, wire_type);
  }
  
  return nullptr;
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
  msg.sub1.text.arg = &sub1_text;
  msg.sub1.text.funcs.decode = decode_string;
  msg.sub2.text.arg = &sub1_text;
  msg.sub2.text.funcs.decode = decode_string;

  cout << "bytes left: " << stream.bytes_left << endl;
  //const pb_msgdesc_t *type = decode_unionmessage_type(&stream);
  cout << "bytes left: " << stream.bytes_left << endl;

  bool decode_ok = pb_decode(&stream, TopMsg_fields, &msg);
  if ( decode_ok ) {
    cout << "OK: top.sfield=" << top_sfield << ", sub1.text=" << sub1_text << endl;
  } else {
    cout << "ERROR: " << PB_GET_ERROR(&stream) << endl;
  }

  return 0;
};