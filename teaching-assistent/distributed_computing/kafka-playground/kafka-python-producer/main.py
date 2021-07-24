from confluent_kafka import Producer
import socket
import time

conf = {
    'bootstrap.servers': "localhost:9092",
    'client.id': socket.gethostname()
}

def callback(err, msg):
    if err is not None:
        print("Failed to deliver message: %s:%s" % (str(msg), str(err)))
    else:
        print("Message produced: %s" % (str(msg.value())))

def run():
    try:
        producer = Producer(conf)

        while(True):
            user_id = 1
            value = 'view_time=%d,user_id=%d' % (int(time.time()), user_id)

            producer.produce(topic="page_views", key="page_id", value=value, callback=callback)
            producer.poll(1)
            time.sleep(1)
    finally:
        producer.flush()

if __name__ == '__main__':
    run()
