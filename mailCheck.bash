#!/bin/bash
curl -f -s "https://duapp2.drexel.edu/webtms_du/app?component=courseDetails2&page=CourseList&service=direct&sp=ZH4sIAAAAAAAAADWLOw7CMBAFlyA%2BNaInF8DGSKGhBFGlQeQCS7yKguzg2BtIxYm4GnfAKOKV82beH5gEDyvSndCeejKi9iyedGUbhEZGUZC3MGyUwDiHGZZc1JYYlvkNHyhDa%2BQPBEbr9jnMOSaHu47GYjAMNpW8sK%2Bb6v8fKZQtvCDpnWOYbjcqU1kMTmhMeu7QRylV2Vrtvq1QxdGkAAAA&sp=SB&sp=SACCT&sp=S16218&sp=S110&sp=2" | grep -q "CLOSED"

found=$(echo $?)

if [[ $found == 1 ]] ; then
	echo "OH SHIT 283 IS OPEN RUN QUICK FASTER HERE LOOK ITS THE CRN: 41762" | mail -s "URGENT: CS 283 IS NOW OPEN" damore1405@gmail.com
elif [[ $found == 2 ]] ; then
	echo "Hmm, something seems to be bunked up in your class checker thing" | mail -s "WARNING: something's wrong" damore1405@gmail.com
#When its 12PM on the dot, give me an update to make sure things are working
elif [[ $found == 0 && $(date +"%H")  == 12 && $(date +"%M") == 0 ]] ; then
	echo "All is well, and the class is still closed :(" | mail -s "OK: Working" damore1405@gmail.com
fi
