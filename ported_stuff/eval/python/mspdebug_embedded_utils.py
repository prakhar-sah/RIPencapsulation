### Back-end third party user interface library for mspdebug ###
### Written by Harrison Williams (Virginia Tech FoRTE Research group) ###

#!/usr/bin/python3

import subprocess
import os
import time
import signal

MSPDEBUG = "mspdebug tilib --embedded"
VERBOSE = True
TRIALS = 3
DEBUG = False  # Outputs each command and wait for user to press enter
NUM_BYTES = 512

# Blocks until mspdebug sends the "\ready" string
def waitForReady(proc, verbose=True):
  line = proc.stdout.readline()
  if verbose:
    print(line)
  while b"\\ready" not in line:
    line = proc.stdout.readline()
    if verbose:
      print(line)

# Blocks until mspdebug sends the "\busy" string
def waitForBusy(proc, verbose=True):
  line = proc.stdout.readline()
  if verbose:
    print(line)
  while b"\\busy" not in line:
    line = proc.stdout.readline()
    if verbose:
      print(line)

# Returns a string representation of a binary file
def readBinaryFile(filename):
  with open(filename, "rb") as f:
    data = f.read()
  bString = ""
  for r in range(NUM_BYTES):
    bString += format(data[r], "08b")
  return bString

# Assumes that mspdebug is ready before sending
# And blocks until mspdebug has sent the ready signal
def sendCmdWaitReady(proc, cmd, verbose=True):
  proc.stdin.write(cmd.encode() + b'\n')
  proc.stdin.flush() # "close" stdin to indicate to mspdebug that our input is done
  waitForReady(proc, verbose)
  if DEBUG:
    print("Sent: " + cmd)
    input("Press enter to continue")

# Send command, but wait for "\busy" instead of "\wait"
# TODO: combine with sendCmd
def sendCmdWaitBusy(proc, cmd, verbose=True):
  proc.stdin.write(cmd.encode() + b'\n')
  proc.stdin.flush()
  waitForBusy(proc, verbose)
  if DEBUG:
    print("Sent: " + cmd)
    input("Press enter to continue")

# Send command and don't wait for any response
def sendCmdNonBlocking(proc, cmd, verbose=True):
  proc.stdin.write(cmd.encode() + b'\n')
  proc.stdin.flush()
  if DEBUG:
    print("Sent: " + cmd)
    input("Press enter to continue")

def mspdebugOpen(mspd_args=""):
  proc = subprocess.Popen(MSPDEBUG + " " + mspd_args,
          shell=True,
          stdin=subprocess.PIPE,
          stdout=subprocess.PIPE,
          stderr=subprocess.STDOUT)
  return proc
