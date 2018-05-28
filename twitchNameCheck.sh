#!/bin/bash
twitchURL="https://passport.twitch.tv/usernames/"
twitchUSERNAME=""
stopfile="/root/.twitch.${twitchUSERNAME}"
emailRECIPIENT=""
emailSENDER=""

if [ ! -e "$stopfile" ]; then

    RESPONSE=$(curl -s -o /dev/null -I -w "%{http_code}" "${twitchURL}${twitchUSERNAME}" 2> /dev/null)

    if [ $RESPONSE -eq 204 ]; then

        touch "$stopfile"
        EMAIL=$(echo -e "To: ${emailRECIPIENT}\nFrom: ${emailSENDER}\nSubject: ${twitchUSERNAME} Now Available\n\nCongrats,\nThe username ${twitchUSERNAME} is now available on twitch!\nSincerly,\nJuf")

        echo "${EMAIL}" | ssmtp "${emailRECIPIENT}"

    fi

fi
