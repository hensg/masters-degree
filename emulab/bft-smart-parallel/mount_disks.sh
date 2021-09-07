#!/bin/bash

disks=`lsblk | grep ^sd | grep -v sda | awk '{print $1}'` 
i=0

for disk in disks
do
    sudo mkdir /checkpoint$i/ 
    sudo chmod 777 /checkpoint$i/	
    sudo mkfs.ext4 /dev/$disk
    sudo mount /dev/$disk /checkpoint$i/
    sudo mkdir /checkpoint$i/states/
    sudo mkdir /checkpoint$i/metadata/
    sudo chmod 777 -R /checkpoint$i/
    $i = $i + 1
done
