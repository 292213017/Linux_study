#!/bin/bash
#Program:
#	This program is show you full name in the scen

PATH=/bin:/sbin:/usr/bin:/usr/local/bin:/usr/local/sbin
export PATH
read -p "Please input your first name" first_name
read -p "Please input your last name" last_name
echo -e "\n your full name is ${first_name} ${last_name}"
