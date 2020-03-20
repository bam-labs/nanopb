#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <typeinfo>
#include "benchmark.pb.h"

using namespace std;

int main()
{
    /* This is the buffer where we will store our message. */
    uint8_t buffer[32] = {};
    size_t message_length;
    bool status;

    /* populate the message fields */
    pb_ostream_t o_stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    Command o_command = Command_init_zero;
    o_command.version = 2;
    strncpy(o_command.cmdkey, "articulate head", sizeof(o_command.cmdkey));
    o_command.attributes.a1 = true;
    o_command.attributes.a2 = 3.14;
    o_command.has_attributes =true;
    /* write the message to the buffer */
    status = pb_encode(&o_stream, Command_fields, &o_command);
    message_length = o_stream.bytes_written;
    /* check message status */
    if (!status)
    {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&o_stream));
        return 1;
    }
    /* Print contents of sent message */
    printf("The contents of one packet sent are:\n");
    printf("\n{\"version\": %d, \"cmdkey\": \"%s\", ", o_command.version, o_command.cmdkey);
    printf("\"attributes\":{\"a1\": %s, \"a2\": %f}}\n\n", (o_command.attributes.a1)?"true":"false", o_command.attributes.a2);
    printf("%lu out of %lu available bytes written\n\n", message_length, o_stream.max_size);
    for (int i = 0; i < sizeof(buffer); i++)
    {
        printf("%02x ", buffer[i]);
    }
    printf("\n\n");
    /* create receiving object */
    Command i_command = Command_init_zero;
    pb_istream_t i_stream = pb_istream_from_buffer(buffer, message_length);
    /* read message from buffer */
    status = pb_decode(&i_stream, Command_fields, &i_command);
    /* check message status */
    if (!status)
    {
        printf("decoding failed: %s\n", PB_GET_ERROR(&o_stream));
        return 1;
    }
    /* Print contents of received message */
    printf("The contents of one packet received are:\n\n");
    printf("{\"version\": %d, \"cmdkey\": \"%s\", ", i_command.version, i_command.cmdkey);
    printf("\"attributes\":{\"a1\": %s, \"a2\": %f}}\n\n", (i_command.attributes.a1)?"true":"false", i_command.attributes.a2);
    printf("Now let's test 1000 iterations:\n");
    /* run 1000 iterations, track time */
    struct timespec start_spec, end_spec;
    const unsigned int iterations = 1000;
    auto start = chrono::high_resolution_clock::now();
    int retval = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_spec);
    for (size_t i = 0; i < iterations; i++) 
    {
        pb_encode(&o_stream, Command_fields, &o_command);
        pb_decode(&i_stream, Command_fields, &i_command);
    }
    retval = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_spec);
    auto finish = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    long cpu_duration = static_cast<long>(1e9) * (end_spec.tv_sec - start_spec.tv_sec) + end_spec.tv_nsec - start_spec.tv_nsec;
    /* print metrics */
    printf("    Wall clock time: %lld nanoseconds\n", duration);
    printf("        Average per iteration: %lld nanoseconds\n", duration/iterations);
    printf("    CPU time : %ld nanoseconds\n", cpu_duration);
    printf("        Averahge pr iteration: %ld nanoseconds\n", cpu_duration/iterations);
    return 0;
}

