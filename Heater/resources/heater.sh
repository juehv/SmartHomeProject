#!/bin/bash
# Runs one Heater Cycle

EX_DIR=/root
PROG_DIR=/opt/shp
PLAN=${PROG_DIR}/Heizplan.csv
LOG_FILE=/var/log/heater.log

cd $EX_DIR
java -jar ${PROG_DIR}/Heater.jar -l /run/lock/ -p ${PLAN} 1>&2 | tee -a -i ${LOG_FILE}
