#!/usr/bin/expect -f

set A [lindex $argv 0]
set B [lindex $argv 1]

proc parseArgv {{A_ "da"} {B_ "db"}} { #function to set default argument value in expect
	variable ::A $A_
	variable ::B $B_
}
parseArgv {*}$argv

spawn echo $A; interact
spawn echo $B; interact
