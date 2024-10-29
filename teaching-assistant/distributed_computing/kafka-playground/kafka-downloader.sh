#!/usr/bin/bash
set -u
set -x

KAFKA_VERSION=2.8.0
SCALA_VERSION=2.13
FULL_VERSION=${SCALA_VERSION}-${KAFKA_VERSION}

curl https://downloads.apache.org/kafka/${KAFKA_VERSION}/kafka_${FULL_VERSION}.tgz --output kafka_${FULL_VERSION}.tgz

tar zxvf kafka_${FULL_VERSION}.tgz 

rm kafka_${FULL_VERSION}.tgz 

ln -fs kafka_${FULL_VERSION} kafka
