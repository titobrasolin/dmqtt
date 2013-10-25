#define DCRTCP_VERBOSE
// #define TCP_VERBOSE_PENDING
// #define TCP_VERBOSE_DUPACK

#define MQTT_DEBUG
#define MQTT_VERBOSE 3

#use mqtt.lib

#define M2MIO_USERNAME         ""
#define M2MIO_PASSWORD         ""
#define M2MIO_DOMAIN           ""
#define M2MIO_DEVICE_TYPE      ""
#define M2MIO_DEVICE_ID        ""
#define M2MIO_BROKER_HOSTNAME  "192.168.0.36"
#define M2MIO_BROKER_PORT      1883

#define RCVBUFSIZE 1024
uint8_t packet_buffer[RCVBUFSIZE];

mqtt_broker_handle_t broker;
void main() {
   auto int packet_length;
   auto uint16_t msg_id, msg_id_rcv;

   sock_init_or_exit(1);
   mqtt_init(&broker, NULL);
   // mqtt_init_auth(&broker, M2MIO_USERNAME, M2MIO_PASSWORD);
   init_socket(&broker, M2MIO_BROKER_HOSTNAME, M2MIO_BROKER_PORT);

   // >>>>> CONNECT
   mqtt_connect(&broker);
   // <<<<< CONNACK
   packet_length = read_packet(broker.socket_info, packet_buffer, sizeof(packet_buffer));
   if (packet_length < 0) {
      fprintf(stderr, "Error(%d) on read packet!\n", packet_length);
      exit(EXIT_FAILURE);
   }
   if(MQTTParseMessageType(packet_buffer) != MQTT_MSG_CONNACK) {
      fprintf(stderr, "CONNACK expected!\n");
      exit(EXIT_FAILURE);
   }
   if(packet_buffer[3] != 0x00) {
      fprintf(stderr, "CONNACK failed!\n");
      exit(EXIT_FAILURE);
   }
   printf("MQTT: Received CONNACK\n");
   // >>>>> PUBLISH QoS 0
   printf("Publish: QoS 0\n");
   mqtt_publish(&broker, "hello/world", "Hello World (QoS 0)", 0);

   // >>>>> PUBLISH QoS 1
   printf("Publish: QoS 1\n");
   mqtt_publish_with_qos(&broker, "hello/world", "Hello World (QoS 1)", 0, 1, &msg_id);
   // <<<<< PUBACK
   packet_length = read_packet(broker.socket_info, packet_buffer, sizeof(packet_buffer));
   if(packet_length < 0) {
      fprintf(stderr, "Error(%d) on read packet!\n", packet_length);
      exit(EXIT_FAILURE);
   }
   if(MQTTParseMessageType(packet_buffer) != MQTT_MSG_PUBACK) {
      fprintf(stderr, "PUBACK expected!\n");
      exit(EXIT_FAILURE);
   }
   msg_id_rcv = mqtt_parse_msg_id(packet_buffer);
   if(msg_id != msg_id_rcv) {
      fprintf(stderr, "%d message id was expected, but %d message id was found!\n", msg_id, msg_id_rcv);
      exit(EXIT_FAILURE);
   }

   while (1) {
      tcp_tick(NULL);
   }
}