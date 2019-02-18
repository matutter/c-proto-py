#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "mymessage.pb.h"

bool decode_request_query(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
  printf("Enter: decode_request_query\n");

  uint8_t buffer[1024] = {0};
  (*arg) = calloc(1, 1024);
  
  /* We could read block-by-block to avoid the large buffer... */
  if (stream->bytes_left > 1024 - 1)
      return false;
  
  if (!pb_read(stream, (*arg), stream->bytes_left)) {

    return false;
  }
  
  /* Print the string, in format comparable with protoc --decode.
    * Format comes from the arg defined in main().
    */
  //printf((char*)*arg, buffer);
  
  return true;
}

bool encode_request_query(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
  char* query = (char*) * arg;

  printf("Enter: encode_request_query (arg = %s)\n", query);
  
  if (!pb_encode_tag_for_field(stream, field))
      return false;
  
  return pb_encode_string(stream, (uint8_t*)query, strlen(query));
}

int demo(void) {
    /* This is the buffer where we will store our message. */
    uint8_t buffer[512];
    size_t message_length;
    bool status;
    
    /* Encode our message */
    {
        /* Allocate space on the stack to store the message data.
         *
         * Nanopb generates simple struct definitions for all the messages.
         * - check out the contents of simple.pb.h!
         * It is a good idea to always initialize your structures
         * so that you do not have garbage data from RAM in there.
         */

        char* query_data = "select * from docs;";
        MsgRequest message = MsgRequest_init_zero;
        message.query.funcs.encode = &encode_request_query;
        message.query.arg = query_data;
        
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        /* Fill in the lucky number */
        message._id = 13;
        char* name = "query #1;";
        strncpy(message.name, name, strlen(name));
        message.name[strlen(name) - 1] = '\0';


        printf("Calling: pb_encode_delimited\n");
        /* if (!pb_encode_delimited(&stream, MsgRequest_fields, &message))
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        } */
   
        printf("Calling: pb_encode\n");
        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, MsgRequest_fields, &message);
        message_length = stream.bytes_written;
        
        /* Then just check for any errors.. */
        if (!status)
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1;
        } else {
          printf("Encoding success\n");
        }
    }
    
    /* Now we could transmit the message over network, store it in a file or
     * wrap it to a pigeon's leg.
     */

    /* But because we are lazy, we will just decode it immediately. */
    
    {
        /* Allocate space for the decoded message. */
        MsgRequest message = MsgRequest_init_zero;
        message.query.funcs.decode = decode_request_query;
        message.query.arg = "stringvalue: \"%s\"\n";
        
        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
        
        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, MsgRequest_fields, &message);
        
        /* Check for errors... */
        if (!status) {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1;
        }
        
        /* Print the data contained in the message. */
        printf("id: %d, name: %s, query: %s\n", (int)message._id, message.name, (char*)message.query.arg);
    }
    
    return 0;
}

void* read_file(char const* filepath, int * data_len) {
    char *source = NULL;
    FILE *fp = fopen(filepath, "r");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { return NULL; }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { return NULL; }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            (*data_len) = (int)newLen;
            printf("Read %s (%lu bytes)\n", filepath, newLen);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0';
            }
        }
        fclose(fp);
    }
    return source;
}

int main(int argc, const char * argv[]) {

    if ( argc >= 2 ) {
        if (strncmp(argv[1], "demo", 4) == 0) {
            return demo();
        } else {
            int data_len = 0;
            void* data = read_file(argv[1], &data_len);
            MsgRequest message = MsgRequest_init_zero;
            message.query.funcs.decode = decode_request_query;
            message.query.arg = "stringvalue: \"%s\"\n";
            
            /* Create a stream that reads from the buffer. */
            pb_istream_t stream = pb_istream_from_buffer(data, data_len);
            
            /* Now we are ready to decode the message. */
            int status = pb_decode(&stream, MsgRequest_fields, &message);
            
            /* Check for errors... */
            if (!status) {
                printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
                return 1;
            }
            
            /* Print the data contained in the message. */
            printf("id: %d, name: %s, query: %s\n", (int)message._id, message.name, (char*)message.query.arg);
        }
    } else {
        printf("Please use 'demo' or path to MsgRequest pb ...\n");
        return 1;
    }
}

