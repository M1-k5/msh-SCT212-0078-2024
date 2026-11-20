# Test Plan

Tested the shell with the required test cases and a few extra ones I came up with.

 Input----  expected behaviour ------actual result 
 (blank line) ----- New prompt, no error ----- Works, just gives a new prompt
 echo hello world ----- Prints text; status 0 ---- Prints hello world, status 0 
 ls /nonexistent----- Error from ls; non-zero status---- Error printed, status 2
 nosuchcommand ----- Error message; status 127 ---- "execvp failed", status 127
 cd /tmp then pwd ---- Prints /tmp ----- Changed dir, pwd shows /tmp 
 sleep 3 & ----- Prompt returns at once; PID shown ---- Prompt returned immediately, PID printed 
 echo hi > f.txt then cat f.txt ----- File contains hi ---- f.txt created with "hi" inside 
 sleep 100 then Ctrl-C ---- Sleep stops; shell survives; status 130----- Sleep stopped, status updated to 130, shell survived 
 Ctrl-D at the prompt ------ Shell exits cleanly ----- Exited with no errors 
# My Own Tests
 Input ---- Expected behaviour ----- Actual result 
 cd` with no args ---- Changes to $HOME ---- Went back to home dir 
 help` ---- Shows built-in commands ---- Printed the help message 
 echo test > out.txt then cat out.txt ---- Writes to file and reads it --- Works perfectly 
 sleep 2 & then ls ---- Background runs while ls executes ----- Backgrounded and ran ls immediately 
