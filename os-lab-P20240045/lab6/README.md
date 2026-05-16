# OS Lab 6 Submission — Linux Security, Users, Groups & File Permissions

- **Student Name:** Pi sereyvathanak
- **Student ID:** p20240045

---

## Task Output Files

Make sure all of the following files are present in your `lab6/` folder:

- [ ] `task1_users.txt`
- [ ] `task2_groups.txt`
- [ ] `task3_permissions.txt`
- [ ] `task3_stat_output.txt`
- [ ] `task4_special_bits.txt`
- [ ] `task5_acl.txt`
- [ ] `security_lab/whoami_suid.c`

---

## Screenshots

Insert your screenshots below.

### Screenshot 1 — Task 1: User Creation
Show `cat task1_users.txt` confirming both `dev_alice` and `dev_bob` accounts exist.
<!-- Insert your screenshot below: -->
![user creation](images/task1_user_creation.png)

---

### Screenshot 2 — Task 1: User Modification
Show the updated `/etc/passwd` entry for `dev_alice` with the GECOS comment field.
<!-- Insert your screenshot below: -->
![user modify](images/task1_user_modify.png)

---

### Screenshot 3 — Task 2: Group Setup
Show `cat task2_groups.txt` with group membership for both users.
<!-- Insert your screenshot below: -->
![group setup](images/task2_group_setup.png)

---

### Screenshot 4 — Task 2: Multiple Group Membership
Show `id dev_alice` confirming membership in both `devteam` and `auditors`.
<!-- Insert your screenshot below: -->
![multi group](images/task2_multi_group.png)

---

### Screenshot 5 — Task 3: Directory Permissions
Show `cat task3_permissions.txt` with `drwxrwx---` on the project directory.
<!-- Insert your screenshot below: -->
![dir permissions](images/task3_dir_permissions.png)

---

### Screenshot 6 — Task 3: Access Denied
Show the `Permission denied` error when `temp_user` tries to access the project directory.
<!-- Insert your screenshot below: -->
![access denied](images/task3_access_denied.png)

---

### Screenshot 7 — Task 4: setgid Bit
Show the directory listing with `s` in the group execute position, and `bob_file.txt` inheriting the `devteam` group.
<!-- Insert your screenshot below: -->
![setgid](images/task4_setgid.png)

---

### Screenshot 8 — Task 4: Sticky Bit
Show the `t` bit in the directory listing and the `Operation not permitted` error when `dev_bob` tries to delete `dev_alice`'s file.
<!-- Insert your screenshot below: -->
![sticky bit](images/task4_sticky_bit.png)

---

### Screenshot 9 — Task 4: setuid Bit
Show `ls -l whoami_suid` with `s` in the owner execute position and the program's UID output.
<!-- Insert your screenshot below: -->
![setuid](images/task4_setuid.png)

---

### Screenshot 10 — Task 5: ACL Directory
Show `getfacl /opt/techcorp/devproject` with the `auditors` ACE.
<!-- Insert your screenshot below: -->
![acl dir](images/task5_acl_dir.png)

---

### Screenshot 11 — Task 5: ACL Access Test
Show `dev_alice` successfully accessing the file and `temp_user` being denied.
<!-- Insert your screenshot below: -->
![acl test](images/task5_acl_test.png)

---

### Screenshot 12 — Task 5: ACL Output File
Show `cat task5_acl.txt` with the full ACL entries.
<!-- Insert your screenshot below: -->
![acl output](images/task5_acl_output.png)

---

## Answers to Lab Questions

1. **What is the difference between `userdel` and `userdel -r`?**
   userdel removes the user account from the system (deletes the entry from /etc/passwd, /etc/shadow, and /etc/group) but leaves the user's home directory and files intact on disk. userdel -r does everything userdel does, but also removes the user's home directory (e.g. /home/dev_alice) and their mail spool. Use -r when you want a clean removal with no leftover files, and omit it when you need to preserve the user's data for archiving or transfer.

2. **Why is it safer to use `visudo` instead of directly editing `/etc/sudoers`?**
   visudo locks the /etc/sudoers file while editing (preventing simultaneous edits) and, most importantly, validates the syntax before saving. If you introduce a typo directly editing /etc/sudoers with a normal text editor and save, the sudo system can break entirely — potentially locking every user, including root, out of elevated privileges. visudo catches syntax errors and refuses to save an invalid file, so the system always has a working sudoers configuration.

3. **What happens when a `setgid` directory contains files created by different users? What benefit does this provide for team collaboration?**
   In a setgid directory, any new file or subdirectory created inside automatically inherits the directory's group (e.g. devteam) instead of the creating user's primary group. So even if dev_alice and dev_bob have different primary groups, all files they create inside the shared directory will belong to devteam. This means every team member can read and write each other's files without manually running chown or chgrp — group-based collaboration becomes seamless and consistent.

4. **What limitation of standard Unix permissions does the ACL system solve?**
   Standard Unix permissions only allow one owner, one group, and one "others" category per file. This means you can't grant specific access to a third user or a second group without opening permissions up to everyone. ACLs (Access Control Lists) solve this by letting you define fine-grained, per-user and per-group rules on a single file or directory. For example, you can grant auditors read-only access to a directory while devteam has full read/write — something impossible to express cleanly with traditional chmod alone.
