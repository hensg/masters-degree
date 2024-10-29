from confluent_kafka import Consumer
import time
import sys


def run(consumer_id):
    conf = {
        'bootstrap.servers': "localhost:9092,localhost:9093",
        'group.id': 'pc',
        'group.instance.id': consumer_id,
        'auto.offset.reset': 'earliest', # latest
        'enable.auto.commit': True
    }
    consumer = Consumer(conf)
    try:
        consumer.subscribe(["pageviews"])

        running = True
        while(running):
            msg = consumer.poll(timeout=1.0)
            if msg is not None:
                topic = msg.topic()
                partition = msg.partition()
                offset = msg.offset()
                print("topic=%s, partition=%s, offset=%s, msg=%s" % (topic, partition, offset, msg.value()))
                time.sleep(2)
    finally:
        consumer.close()


if __name__ == '__main__':
    run(sys.argv[1])
