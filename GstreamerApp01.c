#include <stdio.h>
#include <gst/gst.h>


int main(int argc, char* argv[])
{
    GstElement* pipeline = NULL;
    GstBus* bus = NULL;
    GstMessage* msg = NULL;
    int retval = 0;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Build the pipeline */
    pipeline = gst_parse_launch("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    if (pipeline == NULL) {
        printf("Error (pipeline), the pipeline returned null, about to exit...\n");
        retval = 1;
        goto general_error;
    }

    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    if (bus == NULL) {
        printf("Error (bus), the bus returned null, about to exit...\n");
        retval = 2;
        goto general_error;
    }

    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    if (msg == NULL) {
        printf("Error (msg), the message returned null, about to exit...\n");
        retval = 3;
        goto general_error;
    } 

    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
        printf("Error GMT(msg), the message ie ERROR, about to exit...\n");
        retval = 4;
        goto general_error;
    }

    /* Free resources */
    gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

general_error:

    return retval;
}