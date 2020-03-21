/usr/bin/expect -c 'spawn git add .; interact'
/usr/bin/expect -c 'spawn git commit -m ${1:-updated}"; interact'
/usr/bin/expect -c 'spawn git push origin master; expect "Username:"; send "iamrajee\n"; expect "password:"; send "Raj@114232340\n"; interact'
clear
