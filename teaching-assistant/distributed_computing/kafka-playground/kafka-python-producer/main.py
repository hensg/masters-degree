from confluent_kafka import Producer

import socket
import time

def callback(err, msg):
    if err is not None:
        print("Failed to deliver message: %s:%s" % (str(msg), str(err)))
    else:
        print("Message produced: %s" % (str(msg.value())))

def run():
    try:
        conf = {
            'bootstrap.servers': "localhost:9092,localhost:9093",
            'client.id': socket.gethostname()
        }
        producer = Producer(conf)

        while(True):
            user_id = 1
            value = 'view_time=%d,user_id=%d,page_id=%s' % (int(time.time()), user_id, '/asdj')

            producer.produce(topic="pageviews", key=(str(time.time())), value=value, callback=callback)
            producer.poll(1)
            time.sleep(1)
    finally:
        producer.flush()

if __name__ == '__main__':
    run()
