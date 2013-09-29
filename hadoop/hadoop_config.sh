#!/bin/bash
ssh-keygen -t rsa -P ""
cat $HOME/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys
echo $1 > /opt/hadoop/hadoop-1.2.1/conf/masters
echo $1 > /opt/hadoop/hadoop-1.2.1/conf/slaves
