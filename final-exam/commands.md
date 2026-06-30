# commands.md — exact commands I ran, per part

> Paste the **real** commands you ran, in order, in the fenced blocks below. Graded for
> command competency and is your defence if any output is questioned. One block per part.
> Delete the hint comments and replace with your actual commands.

## Part A — Threads, Mapping & Signals

```bash
cd ~/OS-SE-P20240045/final-exam/partA_threads
nano thread_demo.c
cat thread_demo.c
gcc -pthread -o thread_demo thread_demo.c
./thread_demo

# capture 1:1 user->kernel (LWP) mapping while alive
nano thread_demo.c          # temporarily changed sleep(1) -> sleep(5) in worker()
gcc -pthread -o thread_demo thread_demo.c
./thread_demo &
ps -eLf | grep thread_demo
ps -eLf | grep thread_demo > thread_map.txt
cat thread_map.txt
nano thread_demo.c          # changed sleep(5) back to sleep(1)
gcc -pthread -o thread_demo thread_demo.c

# signal_demo: compile, run, catch interactive interrupt
nano signal_demo.c
gcc -o signal_demo signal_demo.c
./signal_demo
# Ctrl+C sent to running process to trigger SIGINT
```

## Part B — Permissions, Special Bits & ACLs

```bash
cd ~/OS-SE-P20240045/final-exam/partB_security
mkdir -p permtest/shared
touch permtest/private.txt
chmod 600 permtest/private.txt
chmod 711 permtest/shared
ls -l permtest/private.txt > perm_report.txt
ls -ld permtest/shared >> perm_report.txt
stat permtest/private.txt >> perm_report.txt
stat permtest/shared >> perm_report.txt
cat perm_report.txt

# setgid + sticky bit on owned directory
mkdir teamdir
chmod 2775 teamdir
chmod +t teamdir
ls -ld teamdir

# setuid demo binary
nano setuid_demo.c
gcc -o setuid_demo setuid_demo.c
chmod u+s setuid_demo
ls -l setuid_demo
./setuid_demo

# ACL entry on the private file
which setfacl getfacl
setfacl -m u:$(whoami):r-- permtest/private.txt
getfacl permtest/private.txt
getfacl permtest/private.txt >> perm_report.txt
cat perm_report.txt
```

## Part C — Bash Scripting, PATH & Safe Scanning

```bash
cd ~/OS-SE-P20240045/final-exam/partC_scripting
mkdir -p scripts
mkdir -p ~/bin

nano ~/bin/greeter
chmod +x ~/bin/greeter
greeter                      # fails here, ~/bin not yet on PATH
export PATH="$HOME/bin:$PATH"
echo 'export PATH="$HOME/bin:$PATH"' >> ~/.bashrc
greeter                      # works now
echo "$PATH" > path_report.txt
which greeter >> path_report.txt
cat path_report.txt

nano ~/bin/collector
chmod +x ~/bin/collector
mkdir -p ~/collector_test/d1 ~/collector_test/d2
echo "Readable file 1" > ~/collector_test/d1/file1.txt
echo "Readable file 2" > ~/collector_test/d2/file2.txt
echo "Secret stuff" > ~/collector_test/d1/locked.txt
chmod 000 ~/collector_test/d1/locked.txt
mkdir -p ~/OS-SE-P20240045/final-exam/partC_scripting
collector

cp ~/bin/greeter ~/OS-SE-P20240045/final-exam/partC_scripting/scripts/greeter
cp ~/bin/collector ~/OS-SE-P20240045/final-exam/partC_scripting/scripts/collector
```

## Part D — Race Condition & flock

```bash
cd ~/OS-SE-P20240045/final-exam/partD_secure
mkdir -p scripts
echo 150 > stock.txt
cat stock.txt

nano ~/bin/buy_vault_token
chmod +x ~/bin/buy_vault_token
buy_vault_token alice 5
cat stock.txt
cat sales.log
buy_vault_token bob -3
buy_vault_token "" 2
echo 150 > stock.txt

nano ~/bin/swarm
chmod +x ~/bin/swarm

# expose the race: temporarily removed flock wrapper from buy_vault_token
nano ~/bin/buy_vault_token
swarm
swarm
swarm
swarm
nano observations.txt        # recorded final stock: 139, 143, 137, 135

# patch: restored flock -x 200 exclusive lock in buy_vault_token
nano ~/bin/buy_vault_token
swarm
swarm
swarm                        # all runs now land at Final stock: 100

cp ~/bin/buy_vault_token ~/OS-SE-P20240045/final-exam/partD_secure/scripts/buy_vault_token
cp ~/bin/swarm ~/OS-SE-P20240045/final-exam/partD_secure/scripts/swarm
```

## Part E — Backups & cron

```bash
cd ~/OS-SE-P20240045/final-exam/partE_automation
mkdir -p scripts logs images

# E1
mkdir -p sample_project/src sample_project/docs
echo "print('hello')" > sample_project/src/main.py
echo "Project notes" > sample_project/docs/notes.txt
nano ~/bin/backup_project
chmod +x ~/bin/backup_project
backup_project; sleep 1
backup_project; sleep 1
backup_project; sleep 1
backup_project; sleep 1
backup_project; sleep 1
backup_project
ls -lh ~/OS-SE-P20240045/final-exam/partE_automation/backups
cat ~/OS-SE-P20240045/final-exam/partE_automation/backups/backup.log

# E2
nano ~/bin/timed_job
chmod +x ~/bin/timed_job
touch ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_recurring.log
touch ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_oneshot.log
date
crontab -e
# * * * * * /home/se-pi-sereyvathanak/bin/timed_job /home/se-pi-sereyvathanak/OS-SE-P20240045/final-exam/partE_automation/logs/cron_recurring.log
# 35 14 * * * /home/se-pi-sereyvathanak/bin/timed_job /home/se-pi-sereyvathanak/OS-SE-P20240045/final-exam/partE_automation/logs/cron_oneshot.log
crontab -l
cat ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_recurring.log
cat ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_oneshot.log

# E3
nano ~/bin/backup_exam
chmod +x ~/bin/backup_exam
backup_exam
ls -lh ~/exam-backups
crontab -e
# */2 * * * * /home/se-pi-sereyvathanak/bin/backup_exam
# 0 16 * * * /home/se-pi-sereyvathanak/bin/backup_exam
crontab -l
ls -lh ~/exam-backups

cp ~/bin/backup_project ~/OS-SE-P20240045/final-exam/partE_automation/scripts/backup_project
cp ~/bin/timed_job ~/OS-SE-P20240045/final-exam/partE_automation/scripts/timed_job
cp ~/bin/backup_exam ~/OS-SE-P20240045/final-exam/partE_automation/scripts/backup_exam

# consolidate evidence
{
  echo "=== crontab -l ==="
  crontab -l
  echo ""
  echo "=== cron_recurring.log ==="
  cat ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_recurring.log
  echo ""
  echo "=== cron_oneshot.log ==="
  cat ~/OS-SE-P20240045/final-exam/partE_automation/logs/cron_oneshot.log
  echo ""
  echo "=== ~/exam-backups listing ==="
  ls -lh ~/exam-backups
} > ~/OS-SE-P20240045/final-exam/partE_automation/cron_report.txt
```