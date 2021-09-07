#!/bin/bash

sudo apt update
apt install openjdk-8-jre-headless

for i in {0..1}
do
	sudo mkdir /checkpoint$i/ 
	sudo chmod 777 /checkpoint$i/	
done

sudo mkfs.ext4 /dev/sdb
sudo mount /dev/sdb /checkpoint0/
sudo mkfs.ext4 /dev/sdc
sudo mount /dev/sdc /checkpoint1/
sudo mkfs.ext4 /dev/sdd

for j in {0..1}
do
	sudo mkdir /checkpoint$j/states/
	sudo mkdir /checkpoint$j/metadata/
	sudo chmod 777 -R /checkpoint$j/
done
repid=$1
export repid 
