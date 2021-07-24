from confluent_kafka import Consumer
import time

conf = {
    'bootstrap.servers': "localhost:9092",
    'group.id': 'sample',
    'auto.offset.reset': 'earliest', # latest
    'enable.auto.commit': True
}

def run():
    consumer = Consumer(conf)
    try:
        consumer.subscribe(["page_views"])

        running = True
        while(running):
            msg = consumer.poll(timeout=1.0)
            if msg is not None:
                topic = msg.topic()
                partition = msg.partition()
                offset = msg.offset()
                print("topic=%s, partition=%s, offset=%s, msg=%s" % (topic, partition, offset, msg.value()))
    finally:
        consumer.close()


if __name__ == '__main__':
    run()
