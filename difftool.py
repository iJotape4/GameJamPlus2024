#!/usr/bin/python


## DESCRIPTION:
##  This script is an utility that simplifies the UE-diff-tool usage via git.
##
## HOW IT WORKS:
##  Git provides the possibility to use thirdparty tools to DIFF and MERGE the files.
##  Unreal provides both tools for the binary `.uasset`.
##  By following the SETUP section, you will be able to use the `git difftool -t UE_Diff`
##  to compare two versions using the unreal-difftool.
##
## REQUIREMENTS:
##  - Python
##  - Git
##
## SETUP:
##  1. Copy this script into the project folder.
##  2. It's necessary to setup the GIT difftool & mergetool by opening the `.gitconfig` file
##     (In Windows it's generally located inside `C:\Users\{The User Name}\.gitconfig`);
##  3. Add the following config:
##     ```
##     [difftool "UE_Diff"]
##          cmd = C:/Users/{The User Name}/git-ue-diff.bat "$REMOTE" "$LOCAL"
##          binary = true
##     
##     [mergetool "UE_Merge"]
##          cmd = C:/Users/{The User Name}/git-ue-diff.bat "$REMOTE" "$LOCAL" "$BASE" "$MERGED"
##          binary = true
##     ```
##  4. Alongside the `.gitconfig` file, create a file named `git-ue-diff.bat`, then add the following code:
##     ```
##     py %cd%\difftool.py %*
##     ```
##
## USAGE:
##  This script is executed by GIT when running `git difftool -t UE_Diff` or `git mergetool -t UE_Merge`
##
## NOTES:
##  You don't need to specify the project dir or the unreal engine installation dir, because this script will fetch these.
##  Note, if unreal is not installed inside a common directory, you can provide the editor path by defining the environment variable  `UE5.1_EDITOR_PATH` (Change the unreal version accordingly).
##  This script works only on Windows, at the moment.


import sys
import os
import json
import subprocess

args = sys.argv[1:]
is_windows = os.name == 'nt'
pwd = os.getcwd()

if is_windows:
	# Get Disk letter
	assert pwd[1] == ":", "The first argument should be a valid path; in Windows we expect that it starts with the disk letter `C:\`."
	disk_letter = pwd[0:1]

	# In windows it's necessary to convert the GIT paths
	# to windows like paths.
	for i in range(len(args)):
		if args[i][0] == "-":
			# This is not a path
			continue

		# Convert to an absolute path
		if args[i][0:2] != disk_letter + ":":
			args[i] = os.path.join(pwd, args[i])

		# Fix the path
		args[i] = os.path.abspath(args[i])

# Now let's find the *.uproject file inside pwd
uproject_file_path = ""
for root, _, files in os.walk(os.getcwd()):
    for file in files:
        if file.endswith(".uproject"):
            uproject_file_path = os.path.join(root, file)
            break
    if uproject_file_path:  # Break out of the outer loop if found
        break


assert uproject_file_path != "", "The `git difftool.py` should be executed from the UE project dir."

# Read the associated unreal engine version.
engine_association = ""
with open(uproject_file_path, "r") as uproject:
	uproject_json = json.loads(uproject.read())
	engine_association = uproject_json["EngineAssociation"]
	uproject_json = None

assert uproject_file_path != "", "The `git difftool.py` was unable to find the `EngineAssociation` from the uproject file: " + uproject_file_path

# Now it's time to find the Unreal Engine Editor binary location
ue_editor_path = ""
ue_search_paths = []
env_var_name = "UE" + engine_association + "_EDITOR_PATH"
if is_windows:
	if env_var_name in os.environ:
		# The path is provided via environment path
		ue_editor_path = os.environ[env_var_name]
	else:
		# Search the engine inside common directories.
		ue_search_paths = [os.environ["ProgramFiles"], os.environ["ProgramFiles(x86)"]]
		for p in ue_search_paths:
			ue_editor_path = os.path.join(p, "Epic Games", "UE_" + engine_association, "Engine", "Binaries", "Win64", "UnrealEditor.exe")
			if os.path.exists(ue_editor_path):
				# Found
				break
			else:
				# Keep searching
				ue_editor_path = ""
			
assert ue_editor_path != "", "Unable to locate the unreal engine editor. If you installed UE to a custom directory, provide the path by defining the environment variable `" + env_var_name + "`. Binary searched in:" + str(ue_search_paths)

# Quick print
print("--- Momento Games Unreal Engine diff tool executor. ---")
print("Unreal Engine version:", engine_association)
print("Located editor path:", ue_editor_path)
print("UProject path:", uproject_file_path)
print("Difftool arguments:")
for a in args:
	print("    ", a)

if "-h" in args:
	# Execute the -h ureal difftool!
	print("~ Execute -h UE difftool ~")

	proc = subprocess.Popen([ue_editor_path, uproject_file_path, "-diff", "-h"])
	proc.wait()

else:
	# Execute the ureal difftool!
	print("~ Execute UE difftool ~")

	proc = subprocess.Popen([ue_editor_path, uproject_file_path, "-diff"] + args)
	proc.wait()

print("Difftool executed.")