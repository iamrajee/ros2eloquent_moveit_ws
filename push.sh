#!/usr/bin/expect -f

set A [lindex $argv 0]
set B [lindex $argv 1]

proc parseArgv {{A_ "updated"} {B_ "."}} {
	variable ::A $A_
	variable ::B $B_
}
parseArgv {*}$argv

spawn git add $B; interact
spawn git commit -m $A; interact
spawn git push origin master; expect "Username:"; send "iamrajee\n"; expect "password:"; send "Raj@114232340\n"; interact
# exec clear
# puts \033\[2J
puts -nonewline \033\[2J
