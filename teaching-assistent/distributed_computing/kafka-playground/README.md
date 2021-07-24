# Instruções

"kafka/": link simbólico que contém o código para a execução de um servidor Kafka.
"kafka-python-producer/": contém o código para a execução de um produtor Kafka escrito em Python
"kafka-python-consumer/": contém o código para a execução de um consumidor Kafka escrito em Python


## Zookeeper

O Kafka faz uso do Zookeper para manter as configurações de controle do cluster.


Iniciando o Zookeeper:
```
$ cd kafka/
$ bin/zookeeper-server-start.sh config/zookeeper.properties &
```
O comando acima deve iniciar o Zookeeper na porta 2181, você pode verificar se ele está rodando com o comando `netstat -l | grep 2181`.


## Kafka Server

Iniciando um nó de Kafka Server:
```
$ cd kafka/
$ bin/kafka-server-start.sh config/server.properties &
```
O comando acima deve iniciar um nó de Kafka server na porta 9092.

## Kafka Topic

Com o servidor Kafka rodando, iremos criar um tópico que irá armazenar "page_views" dos usuários que estão acessando as páginas de um site por exemplo.
```
$ cd kafka/
$ bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --topic page_views --partitions 5 --replication-factor 1
```
## Kafka Producer

Ver `kafka-python-producer/README.md`.

## Kafka Consumer

Ver `kafka-python-consumer/README.md`.
