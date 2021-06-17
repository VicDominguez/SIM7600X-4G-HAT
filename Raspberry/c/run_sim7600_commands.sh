#! /bin/bash
#
#   Choose a command for configuration of SIM7600E 4G Hat
#
#   In this version, only necessary commands are implemented   
#              J. Stallmann                   27.02.20
#set -x


# - - - - - - - - - - - -    F u n k t i o n s  - - - - - - - - - - - - 

# Define Log-File
LOGFILE=run_sim.log

RUN_1() {
    echo "qmicli -d /dev/cdc-wdm0 --dms-get-operating-mode" >> $LOGFILE
    qmicli -d /dev/cdc-wdm0 --dms-get-operating-mode|tee -a $LOGFILE
}

RUN_2() {
    echo "qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='online'" >> $LOGFILE
    qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='online'|tee -a $LOGFILE
}

RUN_m() {
    echo "qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='low-power'" >> $LOGFILE
    qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='low-power'|tee -a $LOGFILE
}

RUN_3() {
    echo "qmicli -d /dev/cdc-wdm0 --nas-get-signal-strength" >> $LOGFILE
    echo "qmicli -d /dev/cdc-wdm0 --nas-get-home-network" >> $LOGFILE
    qmicli -d /dev/cdc-wdm0 --nas-get-signal-strength|tee -a $LOGFILE
    qmicli -d /dev/cdc-wdm0 --nas-get-home-network|tee -a $LOGFILE
}


	# autoconnect commands
RUN_4() {
	qmicli -d /dev/cdc-wdm0 --wds-get-autoconnect-settings
}

RUN_5() {
	qmicli -d /dev/cdc-wdm0 --wds-stop-network=disable-autoconnect
}



	# manually manipulate raw_ip file
RUN_6() {
	echo "more /sys/class/net/wwan0/qmi/raw_ip" >> $LOGFILE
	#ifdown -v wwan0
	ip link set wwan0 down
	echo 'Y'|sudo tee /sys/class/net/wwan0/qmi/raw_ip
	more /sys/class/net/wwan0/qmi/raw_ip | tee -a $LOGFILE
	#ip link set wwan0 up
}


	# starting provided network script (no IPV setting ?) 
RUN_7() {
	qmi-network /dev/cdc-wdm0 start
}

	# starting provided network script (IPV4 setting) 
RUN_8() {
	qmi-network-IPV4 /dev/cdc-wdm0 start
}

	# checking network settings (IPV4 orIPV6)
RUN_9() {
	qmicli -d /dev/cdc-wdm0 --wds-get-current-settings
}

	# checking SIM card status
RUN_a() {
	qmicli -d /dev/cdc-wdm0 --uim-get-card-status
}

	# stop network by script
RUN_b() {
	qmi-network /dev/cdc-wdm0 stop
}

	# network status by script
RUN_c() {
	qmi-network /dev/cdc-wdm0 status
}

	# show packet service status (CID = 20)
RUN_d() {
	qmicli -d /dev/cdc-wdm0 --wds-get-packet-service-status --client-cid=20 --client-no-release-cid --device-open-proxy
}

	# start network by command (IPV4)
RUN_e() {
#	qmicli -d /dev/cdc-wdm0 --device-open-proxy \
#             --wds-start-network="apn=internet.t-mobile,username=t-mobile,password=tm,\
#             ip-type=4" --client-no-release-cid  
	echo "qmicli -p -d /dev/cdc-wdm0 --device-open-net='net-raw-ip|net-no-qos-header' \
           --wds-start-network=\"apn=internet.t-mobile,username=t-mobile,password=tm,\
            ip-type=4\" --client-no-release-cid" >> $LOGFILE
	qmicli -p -d /dev/cdc-wdm0 --device-open-net='net-raw-ip|net-no-qos-header' \
           --wds-start-network="apn=internet.t-mobile,username=t-mobile,password=tm,ip-type=4" --client-no-release-cid | tee -a $LOGFILE
}


	# start network by command (IPV6)
RUN_h() {
	qmicli -d /dev/cdc-wdm0 --device-open-proxy \
             --wds-start-network="apn=internet.v6.telekom,username=telekom,password=tm, \
             ip-type=6" --client-no-release-cid
}

	# start udhcpc client
RUN_f() {
	# running in foreground
	#udhcpc -f -n -i wwan0
	echo "udhcpc -i wwan0" >> $LOGFILE
	udhcpc -i wwan0 |tee -a $LOGFILE
}

	# show IP address and routing
RUN_g() {
	echo "ip a s wwan0" >> $LOGFILE
	ip a s wwan0 | tee -a $LOGFILE
	echo "ip r s" >> $LOGFILE
	ip r s | tee -a $LOGFILE
	echo "ifconfig wwan0" >> $LOGFILE
	ifconfig wwan0 | tee -a $LOGFILE
	echo "route -n" >> $LOGFILE
	route -n | tee -a $LOGFILE
}

        # Link up and down
RUN_i() {
	echo "ip link set wwan0 up" >> $LOGFILE
	ip link set wwan0 up
	#ifup -v wwan0
	#ifquery -v wwan0
}

RUN_j() {
	echo "ip link set wwan0 down" >> $LOGFILE
	ip link set wwan0 down
	#ifdown -v wwan0
}

RUN_v() {
    rm $LOGFILE
}

RUN_x() {
    exit
}


# - - - - - - - - - - - -    M a i n   - - - - - - - - - - - - 
clear
echo ""
echo "     W a v e s h a r e     S I M 7 6 0 0 E  -  4G    H a t"
echo ""
#echo "Wähle ein Kommando aus"

while true; do
	eval unset a${last_number}
	echo "============================================================="
	echo "1    ${a1}show operating mode"
	echo "2    ${a2}switch on controller"
	echo "m    ${am}switch off controller"
	echo "3    ${a3}show signal strength and nas network"
#	echo "4    ${a4}show autoconnect settings"
#	echo "5    ${a5}disable autoconnect mode"
	echo "6    ${a6}change raw_ip file manually (leaving wwlan0 off)"
	echo ""
#	echo "7    ${a7}start network by qmi-network (no IP-type)"
#	echo "8    ${a8}start network by My-qmi-network (IPV4)"
	echo "e    ${ae}start network by command (IP-type=4)"
#	echo "h    ${ah}start network by command (IP-type=6)"
	echo "9    ${a9}show current network settings (IPV4 or IPV6)"
	echo "a    ${aa}show SIM card status"
	echo ""
	echo "b    ${ab}stop network by qmi-network"
#	echo "c    ${ac}show network status by qmi-network"
#	echo "d    ${ad}show packet service status (CID = 20)"
	echo ""
	echo "f    ${af}start udhcp-Client for IP-address "
	echo "g    ${ag}show IP address and routing"
	echo ""
	echo "i    ${ai}bring link wwan0 up"
	echo "j    ${aj}bring link wwan0 down"
	echo "============================================================="
	echo "v    ${av}Clear Log-FIle (run_sim.log)"
	echo ""
	echo "x    Exit"
	echo ""

	last_number=$number
#	echo $last_number

       read -p "Wähle ein oder mehrere Kommandos aus (getrennt durch Leerzeichen)  : " number
        for i in ${number[@]}
        do
                RUN_$i
		sleep 3
        done

        eval a${i}="\=\>\ "


	sleep 3
done
