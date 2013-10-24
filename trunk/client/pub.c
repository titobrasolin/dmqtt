#define TCPCONFIG 1

#define DCRTCP_VERBOSE

#define TCP_VERBOSE_PENDING
#define TCP_VERBOSE_DUPACK

#define MQTT_DEBUG
#define MQTT_VERBOSE 3

#use dcrtcp.lib
#use mqtt.lib

#define M2MIO_USERNAME         ""
#define M2MIO_PASSWORD         ""
#define M2MIO_DOMAIN           ""
#define M2MIO_DEVICE_TYPE      ""
#define M2MIO_DEVICE_ID        ""
#define M2MIO_BROKER_HOSTNAME  "m2m.eclipse.org"
#define M2MIO_BROKER_PORT      1883

#define ARGC 3
#define ARGV "hello.program", "hello world", "life is grand!"

#define RCVBUFSIZE 1024
uint8_t packet_buffer[RCVBUFSIZE];

mqtt_broker_handle_t broker;
int main(int argc, char* argv[]) {
   auto int packet_length;
   auto uint16_t msg_id, msg_id_rcv;

   sock_init_or_exit(1);
   mqtt_init(&broker, NULL);
   // mqtt_init_auth(&broker, M2MIO_USERNAME, M2MIO_PASSWORD);
   init_socket(&broker, M2MIO_BROKER_HOSTNAME, M2MIO_BROKER_PORT);

   // >>>>> CONNECT
   mqtt_connect(&broker);
   // <<<<< CONNACK
/*
   packet_length = read_packet(broker.socket_info, packet_buffer, sizeof(packet_buffer));
   if (packet_length < 0) {
      fprintf(stderr, "Error(%d) on read packet!\n", packet_length);
      return -1;
   }

   if(MQTTParseMessageType(packet_buffer) != MQTT_MSG_CONNACK) {
      fprintf(stderr, "CONNACK expected!\n");
      return -2;
   }

   if(packet_buffer[3] != 0x00) {
      fprintf(stderr, "CONNACK failed!\n");
      return -2;
   }
*/
   // >>>>> PUBLISH QoS 0
   //printf("Publish: QoS 0\n");
   //mqtt_publish(&broker, "public/myexample/example", "Test libemqtt message.", 0);
   // >>>>> PUBLISH QoS 1
   printf("Publish: QoS 1\n");
   mqtt_publish_with_qos(&broker, "hello/emqtt", "Example: QoS 1", 0, 1, &msg_id);
   // <<<<< PUBACK
   packet_length = read_packet(broker.socket_info, packet_buffer, sizeof(packet_buffer));
   if(packet_length < 0) {
      fprintf(stderr, "Error(%d) on read packet!\n", packet_length);
      return -1;
   }

   if(MQTTParseMessageType(packet_buffer) != MQTT_MSG_PUBACK) {
      fprintf(stderr, "PUBACK expected!\n");
      return -2;
   }

   msg_id_rcv = mqtt_parse_msg_id(packet_buffer);
   if(msg_id != msg_id_rcv) {
      fprintf(stderr, "%d message id was expected, but %d message id was found!\n", msg_id, msg_id_rcv);
      return -3;
   }

   while (1) {
      tcp_tick(NULL);
   }
}