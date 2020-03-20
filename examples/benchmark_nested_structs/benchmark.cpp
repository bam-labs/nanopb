#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "benchmark.pb.h"

const bool SEND = true;
const bool RECEIVE = false;
const uint32_t ITERATIONS = 1000;

using namespace std;

void print_contents(bool sent, int version, char *cmdkey, bool a1, float a2)
{
    printf("The contents of one packet %s are:\n", (sent)?"sent":"received");
    printf("\n  {\"version\": %d, \"cmdkey\": \"%s\", ", version, cmdkey);
    printf("\"attributes\":{\"a1\": %s, \"a2\": %f}}", (a1)?"true":"false", a2);
    printf("\n\n");
}


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
    print_contents(SEND, o_command.version, o_command.cmdkey,
                   o_command.attributes.a1, o_command.attributes.a2);
    printf("%lu out of %lu available bytes were written:\n\n", message_length,
           o_stream.max_size);
    for (int i = 0; i < sizeof(buffer); i++)
    {
        printf("  %02x", buffer[i]);
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
    print_contents(RECEIVE, i_command.version, i_command.cmdkey,
                   i_command.attributes.a1, i_command.attributes.a2);
    printf("Now let's test 1000 iterations:\n");
    /* run 1000 iterations, track time */
    struct timespec start_spec, end_spec;
    auto start = chrono::high_resolution_clock::now();
    int retval = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_spec);
    for (size_t i = 0; i < ITERATIONS; i++) 
    {
        pb_encode(&o_stream, Command_fields, &o_command);
        pb_decode(&i_stream, Command_fields, &i_command);
    }
    retval = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_spec);
    auto finish = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
    long cpu_duration = static_cast<long>(1e9) * (end_spec.tv_sec - start_spec.tv_sec) + end_spec.tv_nsec - start_spec.tv_nsec;
    /* print metrics */
    printf("\n  Total wall clock time: %lld nanoseconds\n", duration);
    printf("  Average wall clock time per iteration: %0.2f nanoseconds\n",
           static_cast<float>(duration)/ITERATIONS);
    printf("\n  Total CPU time : %ld nanoseconds\n", cpu_duration);
    printf("  Average CPU time per iteration: %0.2f nanoseconds\n",
           static_cast<float>(cpu_duration)/ITERATIONS);
    return 0;
}

